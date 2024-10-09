#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#define SALT_SIZE 32
#define NONCE_SIZE 12
#define KEY_SIZE 32
#define TAG_SIZE 16
#define VERSION "\x02"
#define MIN_PASSWORD_LENGTH 8
#define ITERATION_COUNT 1000000

void handle_errors() {
    ERR_print_errors_fp(stderr);
    abort();
}

int base64_encode(const unsigned char* input, int length, char** output) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);

    *output = (*bufferPtr).data;
    return (*bufferPtr).length;
}

int base64_decode(const char* input, int length, unsigned char** output) {
    BIO *bio, *b64;
    int decodeLen = 0;
    int finalLen = 0;

    *output = malloc(length);
    if (!*output) return -1;

    bio = BIO_new_mem_buf(input, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    finalLen = BIO_read(bio, *output, length);
    BIO_free_all(bio);

    return finalLen;
}

int derive_key(const char* password, const unsigned char* salt, unsigned char* key) {
    if (!PKCS5_PBKDF2_HMAC(password, strlen(password), salt, SALT_SIZE, ITERATION_COUNT, EVP_sha256(), KEY_SIZE, key)) {
        handle_errors();
        return 0;
    }
    return 1;
}

char* encrypt_data(const char* data, const char* password) {
    EVP_CIPHER_CTX *ctx;
    unsigned char salt[SALT_SIZE];
    unsigned char key[KEY_SIZE];
    unsigned char nonce[NONCE_SIZE];
    unsigned char tag[TAG_SIZE];
    int len, ciphertext_len;
    unsigned char *ciphertext;
    unsigned char *combined;
    char *encoded;
    int combined_len, encoded_len;

    // Generate random salt and nonce
    if (!RAND_bytes(salt, SALT_SIZE) || !RAND_bytes(nonce, NONCE_SIZE)) {
        handle_errors();
        return NULL;
    }

    // Derive key
    if (!derive_key(password, salt, key)) return NULL;

    // Allocate memory for ciphertext
    ciphertext = malloc(strlen(data) + EVP_MAX_BLOCK_LENGTH);
    if (!ciphertext) return NULL;

    // Create and initialise the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handle_errors();

    // Initialise the encryption operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, nonce))
        handle_errors();

    // Encrypt
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char*)data, strlen(data)))
        handle_errors();
    ciphertext_len = len;

    // Finalise the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handle_errors();
    ciphertext_len += len;

    // Get the tag
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag))
        handle_errors();

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Combine all parts
    combined_len = 1 + SALT_SIZE + NONCE_SIZE + ciphertext_len + TAG_SIZE;
    combined = malloc(combined_len);
    if (!combined) {
        free(ciphertext);
        return NULL;
    }

    memcpy(combined, VERSION, 1);
    memcpy(combined + 1, salt, SALT_SIZE);
    memcpy(combined + 1 + SALT_SIZE, nonce, NONCE_SIZE);
    memcpy(combined + 1 + SALT_SIZE + NONCE_SIZE, ciphertext, ciphertext_len);
    memcpy(combined + 1 + SALT_SIZE + NONCE_SIZE + ciphertext_len, tag, TAG_SIZE);

    // Base64 encode
    encoded_len = base64_encode(combined, combined_len, &encoded);

    // Clean up
    free(ciphertext);
    free(combined);

    return encoded;
}

char* decrypt_data(const char* encrypted_data, const char* password) {
    EVP_CIPHER_CTX *ctx;
    unsigned char *decoded;
    int decoded_len;
    unsigned char salt[SALT_SIZE];
    unsigned char key[KEY_SIZE];
    unsigned char nonce[NONCE_SIZE];
    unsigned char tag[TAG_SIZE];
    unsigned char *ciphertext;
    int ciphertext_len;
    int len;
    unsigned char *plaintext;
    int plaintext_len;
    char *result;

    // Base64 decode
    decoded_len = base64_decode(encrypted_data, strlen(encrypted_data), &decoded);
    if (decoded_len < 0) return NULL;

    // Check version
    if (decoded[0] != 0x02) {
        free(decoded);
        return NULL;
    }

    // Extract components
    memcpy(salt, decoded + 1, SALT_SIZE);
    memcpy(nonce, decoded + 1 + SALT_SIZE, NONCE_SIZE);
    ciphertext_len = decoded_len - (1 + SALT_SIZE + NONCE_SIZE + TAG_SIZE);
    ciphertext = malloc(ciphertext_len);
    if (!ciphertext) {
        free(decoded);
        return NULL;
    }
    memcpy(ciphertext, decoded + 1 + SALT_SIZE + NONCE_SIZE, ciphertext_len);
    memcpy(tag, decoded + decoded_len - TAG_SIZE, TAG_SIZE);

    // Derive key
    if (!derive_key(password, salt, key)) {
        free(decoded);
        free(ciphertext);
        return NULL;
    }

    // Create and initialise the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handle_errors();

    // Initialise the decryption operation
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, nonce))
        handle_errors();

    // Allocate memory for plaintext
    plaintext = malloc(ciphertext_len);
    if (!plaintext) {
        free(decoded);
        free(ciphertext);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    // Decrypt
    if (!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handle_errors();
    plaintext_len = len;

    // Set expected tag value
    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_SIZE, tag))
        handle_errors();

    // Finalise the decryption
    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) <= 0) {
        // Authentication failed
        free(decoded);
        free(ciphertext);
        free(plaintext);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }
    plaintext_len += len;

    // Add null terminator
    result = malloc(plaintext_len + 1);
    if (!result) {
        free(decoded);
        free(ciphertext);
        free(plaintext);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }
    memcpy(result, plaintext, plaintext_len);
    result[plaintext_len] = '\0';

    // Clean up
    free(decoded);
    free(ciphertext);
    free(plaintext);
    EVP_CIPHER_CTX_free(ctx);

    return result;
}

int validate_password(const char* password) {
    return strlen(password) >= MIN_PASSWORD_LENGTH;
}

char* get_valid_password(const char* prompt) {
    char* password = NULL;
    size_t len = 0;
    ssize_t read;

    while (1) {
        printf("%s", prompt);
        read = getline(&password, &len, stdin);
        if (read != -1) {
            // Remove newline character
            if (password[read - 1] == '\n') {
                password[read - 1] = '\0';
            }
            if (validate_password(password)) {
                return password;
            }
        }
        printf("Password must be at least %d characters long.\n", MIN_PASSWORD_LENGTH);
    }
}

int main() {
    char action;
    char *data, *password, *result;

    printf("AES-GCM Encryption/Decryption Tool (Ver.3)\n");

    while (1) {
        printf("Choose action (e: encrypt / d: decrypt / q: quit): ");
        scanf(" %c", &action);
        getchar();  // Consume newline

        if (action == 'q') {
            printf("Session ended.\n");
            break;
        }

        if (action != 'e' && action != 'd') {
            printf("Invalid action. Please choose 'e', 'd', or 'q'.\n");
            continue;
        }

        if (action == 'e') {
            printf("Enter the data to encrypt (Unicode supported): ");
            data = NULL;
            size_t len = 0;
            getline(&data, &len, stdin);
            data[strcspn(data, "\n")] = 0;  // Remove newline

            password = get_valid_password("Enter your password: ");
            result = encrypt_data(data, password);
            if (result) {
                printf("Encrypted data: %s\n", result);
                free(result);
            } else {
                printf("Encryption failed.\n");
            }
            free(data);
            free(password);
        } else if (action == 'd') {
            printf("Enter the base64-encoded data to decrypt: ");
            data = NULL;
            size_t len = 0;
            getline(&data, &len, stdin);
            data[strcspn(data, "\n")] = 0;  // Remove newline

            password = get_valid_password("Enter your password: ");
            result = decrypt_data(data, password);
            if (result) {
                printf("Decrypted data: %s\n", result);
                free(result);
            } else {
                printf("Decryption failed. The data may be corrupted or the password may be incorrect.\n");
            }
            free(data);
            free(password);
        }
    }

    return 0;
}
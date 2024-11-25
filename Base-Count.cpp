#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string input;
    int count_A = 0, count_T = 0, count_C = 0, count_G = 0, count_U = 0;

    do {
        cout << "Enter a sequence of nucleotide bases (A/T/C/G) or 'done' to finish: ";
        cin >> input;

        if (input == "done") {
            break;
        }

        for (char base : input) {
            base = toupper(base);
            switch (base) {
                case 'A':
                    count_A++;
                    break;
                case 'T':
                    count_T++;
                    break;
                case 'C':
                    count_C++;
                    break;
                case 'G':
                    count_G++;
                    break;
                case 'U':
                    count_U++;
                    break;
                default:
                    cout << "Invalid nucleotide base: " << base << endl;
            }
        }

    } while (true);

    cout << "Count of A: " << count_A << endl;
    cout << "Count of T: " << count_T << endl;
    cout << "Count of C: " << count_C << endl;
    cout << "Count of G: " << count_G << endl;
    cout << "Total Base count: " << count_A + count_C + count_G + count_T + count_U;

    return 0;
}


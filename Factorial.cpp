#include <iostream>
using namespace std;

int main() {
    int num;
    unsigned long long factorial = 1;  // Use unsigned long long to handle large values

    // Input the number from the user
    cout << "Enter a positive integer: ";
    cin >> num;

    // Check if the input is valid (non-negative)
    if (num < 0) {
        cout << "Factorial is not defined for negative numbers." << endl;
    } else {
        // Calculate factorial
        for (int i = 1; i <= num; ++i) {
            factorial *= i;
        }

        // Display the result
        cout << "Factorial of " << num << " = " << factorial << endl;
    }

    return 0;
}

#include <iostream>
using namespace std;

int main() {
    int num, originalNum, remainder, sum = 0;

    // Input the number from the user
    cout << "Enter the number to check: ";
    cin >> num;

    // Store the original number for comparison
    originalNum = num;

    // Calculate the sum of cubes of each digit
    while (num != 0) {
        remainder = num % 10;         // Get the last digit
        sum += remainder * remainder * remainder;  // Add the cube of the digit to sum
        num /= 10;                    // Remove the last digit
    }

    // Check if the calculated sum matches the original number
    if (originalNum == sum)
        cout << originalNum << " is an Armstrong number." << endl;
    else
        cout << originalNum << " is not an Armstrong number." << endl;

    return 0;
}

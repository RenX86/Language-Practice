#include <iostream>
#include <cmath>  // For power function, if needed
using namespace std;

int main() {
    double weight, height, bmi;

    // Input: weight and height from the user
    cout << "Enter weight in kilograms: ";
    cin >> weight;
    cout << "Enter height in meters: ";
    cin >> height;

    // Check if inputs are valid (positive values for weight and height)
    if (weight > 0 && height > 0) {
        // Calculate BMI
        bmi = weight / (height * height);
        cout << "The BMI of the person is: " << bmi << endl;
        
        // Optional: Interpretation of BMI
        if (bmi < 18.5)
            cout << "BMI Category: Underweight" << endl;
        else if (bmi >= 18.5 && bmi < 24.9)
            cout << "BMI Category: Normal weight" << endl;
        else if (bmi >= 25 && bmi < 29.9)
            cout << "BMI Category: Overweight" << endl;
        else
            cout << "BMI Category: Obese" << endl;
    } else {
        cout << "Error: Weight and height must be positive values." << endl;
    }

    return 0;
}

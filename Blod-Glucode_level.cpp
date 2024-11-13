#include <iostream>
using namespace std;

int main() {
    // Declare a variable to store the glucose level
    float glucoseLevel;
    
    // Prompt the user to enter the glucose level
    cout << "Enter your blood glucose level (in mg/dL): ";
    cin >> glucoseLevel;
    
    // Determine the condition based on the glucose level
    if (glucoseLevel < 70) {
        cout << "Your glucose level is " << glucoseLevel << " mg/dL. You have hypoglycemia." << endl;
    } 
    else if (glucoseLevel >= 70 && glucoseLevel <= 180) {
        cout << "Your glucose level is " << glucoseLevel << " mg/dL. Your glucose level is normal (Hyperglycemia)." << endl;
    } 
    else {
        cout << "Your glucose level is " << glucoseLevel << " mg/dL. You have diabetes." << endl;
    }

    return 0;
}

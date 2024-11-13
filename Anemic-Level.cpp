#include <iostream>
using namespace std;

int main() {
    // Declare a variable to store the hemoglobin level
    float hemoglobinLevel;
    
    // Prompt the user to enter their hemoglobin level
    cout << "Enter your hemoglobin level (in g/dL): ";
    cin >> hemoglobinLevel;
    
    // Determine the anemia condition based on the hemoglobin level
    if (hemoglobinLevel < 8) {
        cout << "Your hemoglobin level is " << hemoglobinLevel << " g/dL. You have severe anemia." << endl;
    } 
    else if (hemoglobinLevel >= 8 && hemoglobinLevel < 9.6) {
        cout << "Your hemoglobin level is " << hemoglobinLevel << " g/dL. You have moderate anemia." << endl;
    } 
    else if (hemoglobinLevel >= 9.6 && hemoglobinLevel <= 13) {
        cout << "Your hemoglobin level is " << hemoglobinLevel << " g/dL. You have mild anemia." << endl;
    } 
    else if (hemoglobinLevel > 13 && hemoglobinLevel <= 17) {
        cout << "Your hemoglobin level is " << hemoglobinLevel << " g/dL. Your hemoglobin level is normal." << endl;
    } 
    else {
        cout << "Your hemoglobin level is " << hemoglobinLevel << " g/dL. The level is above normal." << endl;
    }

    return 0;
}

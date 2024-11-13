#include <iostream>
#include <cmath>    // for log10 function
using namespace std;

int main() {
    double hydrogenIonConcentration;
    double pH;

    // Input: hydrogen ion concentration from the user
    cout << "Enter the hydrogen ion concentration [H+] in moles per liter: ";
    cin >> hydrogenIonConcentration;

    // Check if the input is valid (positive concentration)
    if (hydrogenIonConcentration > 0) {
        // Calculate pH using the formula pH = -log10([H+])
        pH = -log10(hydrogenIonConcentration);
        cout << "The pH of the solution is: " << pH << endl;
    } else {
        cout << "Error: Hydrogen ion concentration must be a positive value." << endl;
    }

    return 0;
}

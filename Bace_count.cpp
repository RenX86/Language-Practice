#include <iostream>
#include <string>
using namespace std;

int main() {
    string dna_sequence;
    int a_count = 0, c_count = 0, g_count = 0, t_count = 0;

    // Input the DNA sequence
    cout << "Enter the DNA sequence: ";
    cin >> dna_sequence;

    // Calculate the base counts for A, C, G, and T
    for (char base : dna_sequence) {
        switch (base) {
            case 'A':
            case 'a':
                a_count++;
                break;
            case 'C':
            case 'c':
                c_count++;
                break;
            case 'G':
            case 'g':
                g_count++;
                break;
            case 'T':
            case 't':
                t_count++;
                break;
            default:
                cout << "Invalid base: " << base << endl;
                return 1;
        }
    }

    // Calculate the total base count
    int total_bases = a_count + c_count + g_count + t_count;

    // Calculate the average base count (total bases / length of sequence)
    double average_base_count = static_cast<double>(total_bases) / dna_sequence.length();

    // Display the results
    cout << "Total base count: " << total_bases << endl;
    cout << "A count: " << a_count << endl;
    cout << "C count: " << c_count << endl;
    cout << "G count: " << g_count << endl;
    cout << "T count: " << t_count << endl;
    cout << "Average base count: " << average_base_count << endl;

    return 0;
}

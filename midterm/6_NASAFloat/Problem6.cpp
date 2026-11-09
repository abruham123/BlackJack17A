/* 
 * File:   Problem6.cpp
 * Author: Abraham Murillo
 * Purpose: Midterm problem 6 — NASA float hand answers (cout only)
 *          Hand calculations scanned in this folder.
 */

#include <iostream>
using namespace std;

void prntAns();

int main(){
    prntAns();
    return 0;
}

void prntAns(){
    cout << "Problem 6 — Conversion to NASA 4 byte float format" << endl;
    cout << "====================================================" << endl;

    // Part a) Convert to binary, octal, hex
    cout << "\n--- a) Positive values ---" << endl;

    cout << "\n49.1875" << endl;
    cout << "  Binary : 110001.0011" << endl;
    cout << "  Hex    : 31.6" << endl;
    cout << "  Octal  : 61.14" << endl;

    cout << "\n3.07421875" << endl;
    cout << "  Binary : 11.00010011" << endl;
    cout << "  Hex    : 3.13" << endl;
    cout << "  Octal  : 3.046" << endl;

    cout << "\n0.2" << endl;
    cout << "  Binary : .0011" << endl;
    cout << "  Hex    : .3" << endl;
    cout << "  Octal  : .1463 repeating" << endl;

    // Part b) Negatives
    cout << "\n--- b) Negative values ---" << endl;
    cout << "  -49.1875     -> 9951.98125 (base 10 / NASA result)" << endl;
    cout << "  -3.07421875  -> binary 1101.11101101" << endl;
    cout << "  -0.2         -> .998" << endl;

    // Part c) Hex float representations -> decimal
    cout << "\n--- c) Float representations to decimal ---" << endl;
    cout << "  69999902 -> 0.412499" << endl;
    cout << "  69999903 -> 0.412499" << endl;
    cout << "  966667FF -> 0.58750009" << endl;

    cout << "\n(See hand scans and NASA Reverse.pdf in this folder.)" << endl;
}

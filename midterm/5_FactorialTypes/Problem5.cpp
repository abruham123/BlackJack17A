/* 
 * File:   Problem5.cpp
 * Author: Abraham Murillo
 * Purpose: Midterm problem 5 — largest n for n! per primitive type
 *          Values found by multiplying until overflow, then n-1.
 *          This program only prints the final results.
 */

#include <iostream>
using namespace std;

int main(){
    cout << "Largest n for n! by data type (Mac / LP64)" << endl;
    cout << "===========================================" << endl;

    // 8-bit a byte s/u char
    cout << "signed char / byte     : 5"  << endl;   // 5!=120, 6!=720
    cout << "unsigned char          : 5"  << endl;   // still < 255 for 5!, not 6!

    // 16-bit
    cout << "signed short           : 7"  << endl;   // 7!=5040, 8!=40320
    cout << "unsigned short         : 8"  << endl;   // 8!=40320 < 65535

    // 32-bit
    cout << "signed int             : 12" << endl;   // 12! fits, 13! does not
    cout << "unsigned int           : 12" << endl;

    // 64-bit  (long == long long)
    cout << "signed long            : 20" << endl;
    cout << "unsigned long          : 20" << endl;
    cout << "signed long long       : 20" << endl;
    cout << "unsigned long long     : 20" << endl;

    // floating point — overflow to neg
    cout << "float                  : 34" << endl;   // 35! -> neg or overflow
    cout << "double                 : 170"<< endl;   // 171! -> neg or overflow

    return 0;
}

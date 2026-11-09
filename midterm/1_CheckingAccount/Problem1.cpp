/* 
 * File:   Problem1.cpp
 * Author: Abraham Murillo
 * Purpose: Midterm problem 1 — checking account with dynamic structure
 */

//System Libraries
#include <iostream>
#include <string>
using namespace std;

//User Libraries
#include "prob1.h"

//Global Constants

//Function Prototypes
Customer *fillCust();
void prnCust(const Customer *);
void destroy(Customer *);

//Execution Begins Here
int main(int argc, char** argv){
    Customer *cust = fillCust();   // pointer -> use -> not .
    prnCust(cust);
    destroy(cust);
    return 0;
}

void destroy(Customer *c){
    delete [] c->checks;     // free arrays first
    delete [] c->deposits;
    delete c;                // then free the structure
}

Customer *fillCust(){       //Array of structures so a table
    Customer *c = new Customer;   // structure on the heap

    // a. five-digit account number
    do{
        cout << "Enter 5 digit account number: ";
        cin  >> c->acctNum;
        if(c->acctNum < 10000 || c->acctNum > 99999){
            cout << "Must be exactly 5 digits. Try again.\n";
        }
    }while(c->acctNum < 10000 || c->acctNum > 99999);

    // b–c. ignore leftover '\n' from cin >> before getline
    cin.ignore();
    cout << "Enter name: ";
    getline(cin, c->name);
    cout << "Enter address: ";
    getline(cin, c->address);

    // d. beginning balance
    cout << "Enter beginning balance: ";
    cin  >> c->begBal;

    // e. checks — negative amount means done; grow array each time
    c->nChecks = 0;
    c->checks  = new float[0];

    cout << "Enter check amounts (negative number to stop):\n";
    float amount;
    cout << "Check amount: ";
    cin  >> amount;

    while(amount >= 0){
        float *temp = new float[c->nChecks + 1];   // one bigger
        for(int i = 0; i < c->nChecks; i++){
            temp[i] = c->checks[i];                // copy old
        }
        temp[c->nChecks] = amount;
        delete [] c->checks;                       // drop old block
        c->checks = temp;                          // keep new block
        c->nChecks++;

        cout << "Check amount: ";
        cin  >> amount;
    }

    // f. deposits — same grow pattern
    c->nDeposits = 0;
    c->deposits  = new float[0];

    cout << "Enter deposit amounts (negative number to stop):\n";
    cout << "Deposit amount: ";
    cin  >> amount;

    while(amount >= 0){
        float *temp = new float[c->nDeposits + 1];
        for(int i = 0; i < c->nDeposits; i++){
            temp[i] = c->deposits[i];
        }
        temp[c->nDeposits] = amount;
        delete [] c->deposits;
        c->deposits = temp;
        c->nDeposits++;

        cout << "Deposit amount: ";
        cin  >> amount;
    }

    // balance = begin + deposits - checks
    c->endBal = c->begBal;
    for(int i = 0; i < c->nDeposits; i++){
        c->endBal += c->deposits[i];
    }
    for(int i = 0; i < c->nChecks; i++){
        c->endBal -= c->checks[i];
    }

    cout << "New balance: $" << c->endBal << endl;

    if(c->endBal < 0){
        cout << "Account overdrawn. Additional $20 fee has been assessed.\n";
        c->endBal -= 20;   // debit the fee
        cout << "Balance with fee: $" << c->endBal << endl;
    }

    return c;
}

void prnCust(const Customer *c){
    cout << "\n----Customer Record----\n";
    cout << "Account:             " << c->acctNum  << endl;
    cout << "Name:                " << c->name     << endl;
    cout << "Address:             " << c->address  << endl;
    cout << "Beginning Balance:  $" << c->begBal   << endl;

    cout << "Checks (" << c->nChecks << "):\n";
    for(int i = 0; i < c->nChecks; i++){
        cout << "  #" << (i + 1) << ": $" << c->checks[i] << endl;
    }

    cout << "Deposits (" << c->nDeposits << "):\n";
    for(int i = 0; i < c->nDeposits; i++){
        cout << "  #" << (i + 1) << ": $" << c->deposits[i] << endl;
    }

    cout << "Ending Balance:    $" << c->endBal << endl;
}

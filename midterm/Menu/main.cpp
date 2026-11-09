/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Purpose: Midterm combined menu (MenuWfunctions style)
 *          Problems 1-7; problem 3 is separate project only
 */

//System Libraries
#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

//User Libraries
#include "prob1.h"
#include "prob2.h"
#include "prob7.h"

//Global Constants - Math/Physics Constants, Conversions,
//                   2-D Array Dimensions

//Function Prototypes — menu
void menu();
void prblm1();
void prblm2();
void prblm3();
void prblm4();
void prblm5();
void prblm6();
void prblm7();

//Problem 1 helpers
Customer *fillCust();
void prnCust(const Customer *);
void destroyCust(Customer *);

//Problem 2 helpers
Employee *fillEmp(int &);
float     calcPay(float, float);
void      prnCheck(const Employee *);
string    engAmt(float);
void      destroyEmp(Employee *);

//Problem 4 helpers
void splitDig(int n, int &d0, int &d1, int &d2, int &d3);
bool badDgts(int d0, int d1, int d2, int d3);
int  buildNum(int d0, int d1, int d2, int d3);
int  encrypt(int n);
int  decrypt(int n);

//Problem 7 helpers
bool     isPrime(int);
int      power(int, int &);
int      nPrimes(int);
Primes  *factor(int);
void     prtPrms(const Primes *);
void     cleanUp(Primes *);

//Execution Begins Here
int main(int argc, char** argv) {
    //Declare Variables
    char choice;
    
    //Loop and Display menu
    do{
        menu();
        cin>>choice;

        //Process/Map inputs to outputs
        switch(choice){
            case '1':{prblm1();break;}
            case '2':{prblm2();break;}
            case '3':{prblm3();break;}
            case '4':{prblm4();break;}
            case '5':{prblm5();break;}
            case '6':{prblm6();break;}
            case '7':{prblm7();break;}
            default: cout<<"Exiting Menu"<<endl;
        }
    }while(choice>='1'&&choice<='7');
    
    //Exit stage right!
    return 0;
}



void menu(){
    //Display menu
    cout<<endl<<"Choose from the following Menu"<<endl;
    cout<<"Type 1 for Problem 1 — Checking Account"<<endl;
    cout<<"Type 2 for Problem 2 — Paycheck"<<endl;
    cout<<"Type 3 for Problem 3 — Stats (separate project)"<<endl;
    cout<<"Type 4 for Problem 4 — Encrypt / Decrypt"<<endl;
    cout<<"Type 5 for Problem 5 — Factorial Data Types"<<endl;
    cout<<"Type 6 for Problem 6 — NASA Float"<<endl;
    cout<<"Type 7 for Problem 7 — Prime Factors"<<endl<<endl;
}


void prblm1(){
    cout<<"Problem 1 — Checking Account"<<endl;
    Customer *cust = fillCust();
    prnCust(cust);
    destroyCust(cust);
}

void destroyCust(Customer *c){
    delete [] c->checks;
    delete [] c->deposits;
    delete c;
}

Customer *fillCust(){
    Customer *c = new Customer;

    do{
        cout << "Enter 5 digit account number: ";
        cin  >> c->acctNum;
        if(c->acctNum < 10000 || c->acctNum > 99999){
            cout << "Must be exactly 5 digits. Try again.\n";
        }
    }while(c->acctNum < 10000 || c->acctNum > 99999);

    cin.ignore();
    cout << "Enter name: ";
    getline(cin, c->name);
    cout << "Enter address: ";
    getline(cin, c->address);

    cout << "Enter beginning balance: ";
    cin  >> c->begBal;

    c->nChecks = 0;
    c->checks  = new float[0];

    cout << "Enter check amounts (negative number to stop):\n";
    float amount;
    cout << "Check amount: ";
    cin  >> amount;

    while(amount >= 0){
        float *temp = new float[c->nChecks + 1];
        for(int i = 0; i < c->nChecks; i++){
            temp[i] = c->checks[i];
        }
        temp[c->nChecks] = amount;
        delete [] c->checks;
        c->checks = temp;
        c->nChecks++;

        cout << "Check amount: ";
        cin  >> amount;
    }

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
        c->endBal -= 20;
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


void prblm2(){
    cout<<"Problem 2 — Paycheck"<<endl;
    int nEmp = 0;
    Employee *emp = fillEmp(nEmp);
    for(int i = 0; i < nEmp; i++){
        prnCheck(&emp[i]);
    }
    destroyEmp(emp);
}

void destroyEmp(Employee *e){
    delete [] e;
}

Employee *fillEmp(int &n){
    cout << "How many employees? ";
    cin  >> n;

    Employee *e = new Employee[n];
    int count = 0;

    for(int i = 0; i < n; i++){
        cout << "\nEmployee #" << (i + 1) << endl;

        cin.ignore();
        cout << "Name: ";
        getline(cin, e[i].name);
        cout << "Hours worked: ";
        cin  >> e[i].hours;
        cout << "Rate of pay: ";
        cin  >> e[i].rate;

        if(e[i].hours < 0 || e[i].rate < 0){
            cout << "Invalid hours or rate. Stopping input.\n";
            break;
        }

        e[i].gross = calcPay(e[i].hours, e[i].rate);
        count++;
    }

    n = count;
    return e;
}

float calcPay(float hours, float rate){
    float pay = 0;
    if(hours <= 40){
        pay = hours * rate;
    }else if(hours < 50){
        pay  = 40 * rate;
        pay += (hours - 40) * rate * 2;
    }else{
        pay  = 40 * rate;
        pay += 9 * rate * 2;
        pay += (hours - 49) * rate * 3;
    }
    return pay;
}

void prnCheck(const Employee *e){
    cout << fixed << setprecision(2);
    cout << "\n========================================\n";
    cout << "           AB Agency\n";
    cout << "        123 Campus Drive, Moreno Valley\n";
    cout << "========================================\n";
    cout << "Name: " << e->name
         << "          Amount: $" << e->gross << endl;
    cout << "Amount: " << engAmt(e->gross) << endl;
    cout << "\nSignature: ____________________\n";
    cout << "========================================\n";
}

string engAmt(float amount){
    int totalCents = static_cast<int>(amount * 100.0f + 0.5f);
    int dollars    = totalCents / 100;
    int cents      = totalCents % 100;

    int n1000s =  dollars / 1000;
    int n100s  = (dollars % 1000) / 100;
    int n10s   = (dollars / 10) % 10;
    int n1s    =  dollars % 10;

    string ones[10]  = {"", "One", "Two", "Three", "Four",
                        "Five", "Six", "Seven", "Eight", "Nine"};
    string teens[10] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen",
                        "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    string tens[10]  = {"", "", "Twenty", "Thirty", "Forty",
                        "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

    string words = "";
    if(n1000s > 0) words += ones[n1000s] + " Thousand ";
    if(n100s  > 0) words += ones[n100s]  + " Hundred ";

    if(n10s == 1){
        words += teens[n1s] + " ";
    }else{
        if(n10s > 1) words += tens[n10s] + " ";
        if(n1s  > 0) words += ones[n1s]  + " ";
    }

    if(dollars == 0) words = "Zero ";

    words += "and ";
    string dig = "0123456789";
    words += dig[cents / 10];
    words += dig[cents % 10];
    words += "/100 Dollars";
    return words;
}


void prblm3(){
    cout<<"Problem 3 is included in a separate project: 3_Stats/"<<endl;
    cout<<"See StatStructureStubProb3.cpp, Array.h, Stats.h, StatsResults.txt"<<endl;
}


void prblm4(){
    cout<<"Problem 4 — Encrypt / Decrypt"<<endl;
    cout<<"Type E to encrypt, D to decrypt: ";
    char which;
    cin>>which;

    int n;
    cout<<"Enter a valid 4 digit number (0-7): ";
    cin>>n;

    if(which=='E' || which=='e'){
        int enc = encrypt(n);
        if(enc<0){
            cout<<"Error: Invalid digits ONLY 0-7.\n";
        }else{
            cout<<"Encrypted: "<<enc<<endl;
        }
    }else if(which=='D' || which=='d'){
        int orig = decrypt(n);
        if(orig<0){
            cout<<"Error: Invalid digits ONLY 0-7.\n";
        }else{
            cout<<"Decrypted: "<<orig<<endl;
        }
    }else{
        cout<<"Unknown choice. Use E or D."<<endl;
    }
}

void splitDig(int n, int &d0, int &d1, int &d2, int &d3){
    d0=(n/1000)%10;
    d1=(n/100)%10;
    d2=(n/10)%10;
    d3=n%10;
}

bool badDgts(int d0, int d1, int d2, int d3){
    int d[4] = {d0, d1, d2, d3};
    for(int i = 0; i < 4; i++){
        if(d[i] < 0 || d[i] > 7) return true;
    }
    return false;
}

int buildNum(int d0, int d1, int d2, int d3){
    return d0*1000 + d1*100 + d2*10 + d3;
}

int encrypt(int n){
    int d0, d1, d2, d3;
    splitDig(n, d0, d1, d2, d3);
    if (badDgts(d0,d1,d2,d3)) return -1;

    d0=(d0+3)%8;
    d1=(d1+3)%8;
    d2=(d2+3)%8;
    d3=(d3+3)%8;

    int t;
    t=d0; d0=d2; d2=t;
    t=d1; d1=d3; d3=t;

    return buildNum(d0,d1,d2,d3);
}

int decrypt(int n){
    int d0, d1, d2, d3;
    splitDig(n, d0, d1, d2, d3);
    if(badDgts(d0, d1,d2,d3)) return -1;

    int t;
    t = d0; d0 = d2; d2 = t;
    t = d1; d1 = d3; d3 = t;

    d0 = (d0 + 5) % 8;
    d1 = (d1 + 5) % 8;
    d2 = (d2 + 5) % 8;
    d3 = (d3 + 5) % 8;
    return buildNum(d0, d1, d2, d3);
}


void prblm5(){
    cout << "Problem 5 — Largest n for n! by data type (Mac / LP64)" << endl;
    cout << "===========================================" << endl;
    cout << "signed char / byte     : 5"  << endl;
    cout << "unsigned char          : 5"  << endl;
    cout << "signed short           : 7"  << endl;
    cout << "unsigned short         : 8"  << endl;
    cout << "signed int             : 12" << endl;
    cout << "unsigned int           : 12" << endl;
    cout << "signed long            : 20" << endl;
    cout << "unsigned long          : 20" << endl;
    cout << "signed long long       : 20" << endl;
    cout << "unsigned long long     : 20" << endl;
    cout << "float                  : 34" << endl;
    cout << "double                 : 170"<< endl;
}


void prblm6(){
    cout << "Problem 6 — Conversion to NASA 4 byte float format" << endl;
    cout << "====================================================" << endl;

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

    cout << "\n--- b) Negative values ---" << endl;
    cout << "  -49.1875     -> 9951.98125 (base 10 / NASA result)" << endl;
    cout << "  -3.07421875  -> binary 1101.11101101" << endl;
    cout << "  -0.2         -> .998" << endl;

    cout << "\n--- c) Float representations to decimal ---" << endl;
    cout << "  69999902 -> 0.412499" << endl;
    cout << "  69999903 -> 0.412499" << endl;
    cout << "  966667FF -> 0.58750009" << endl;

    cout << "\n(Hand scans are in folder 6_NASAFloat/.)" << endl;
}


void prblm7(){
    cout<<"Problem 7 — Prime Factors"<<endl;
    int n;
    cout << "Enter an integer [2, 65000]: ";
    cin  >> n;

    if(n < 2 || n > 65000){
        cout << "Invalid input.\n";
        return;
    }

    Primes *p = factor(n);
    prtPrms(p);
    cleanUp(p);
}

bool isPrime(int n){
    if(n <= 1) return false;
    int nsqrt = static_cast<int>(sqrt(n));
    for(int i = 2; i <= nsqrt; i++){
        if(n % i == 0) return false;
    }
    return true;
}

int power(int prime, int &n){
    int pwr = 0;
    while(n % prime == 0){
        n /= prime;
        pwr++;
    }
    return pwr;
}

int nPrimes(int n){
    int temp  = n;
    int count = 0;
    for(int i = 2; temp > 1; i++){
        if(temp % i == 0 && isPrime(i)){
            count++;
            while(temp % i == 0) temp /= i;
        }
    }
    return count;
}

Primes *factor(int n){
    Primes *p = new Primes;
    p->n       = static_cast<unsigned short>(n);
    p->nPrimes = static_cast<unsigned char>(nPrimes(n));
    p->pStrAry = new Prime[p->nPrimes];

    int temp = n;
    int idx  = 0;
    for(int i = 2; temp > 1 && idx < p->nPrimes; i++){
        if(temp % i == 0 && isPrime(i)){
            p->pStrAry[idx].prime = static_cast<unsigned short>(i);
            p->pStrAry[idx].power = static_cast<unsigned char>(power(i, temp));
            idx++;
        }
    }
    return p;
}

void prtPrms(const Primes *p){
    cout << p->n << " = ";
    for(int i = 0; i < p->nPrimes; i++){
        cout << p->pStrAry[i].prime << "^" << static_cast<int>(p->pStrAry[i].power);
        if(i < p->nPrimes - 1) cout << " * ";
    }
    cout << endl;
}

void cleanUp(Primes *p){
    delete [] p->pStrAry;
    delete p;
}

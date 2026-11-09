/* 
 * File:   Problem2.cpp
 * Author: Abraham Murillo
 * Purpose: Midterm problem 2 — paycheck with English amount
 */

//System Libraries
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//User Libraries
#include "prob2.h"

//Global Constants

//Function Prototypes
Employee *fillEmp(int &);
float     calcPay(float, float);
void      prnCheck(const Employee *);
string    engAmt(float);
void      destroy(Employee *);

int main(int argc, char** argv){
    // nEmp: requested size in, accepted count out
    int nEmp = 0;
    Employee *emp = fillEmp(nEmp);

    for(int i = 0; i < nEmp; i++){
        prnCheck(&emp[i]);   // address of one row
    }

    destroy(emp);
    return 0;
}

void destroy(Employee *e){
    delete [] e;   // one new[] for the whole table
}

Employee *fillEmp(int &n){
    cout << "How many employees? ";
    cin  >> n;

    // dynamic array of structures = table of employees
    Employee *e = new Employee[n];
    int count = 0;

    for(int i = 0; i < n; i++){
        cout << "\nEmployee #" << (i + 1) << endl;

        cin.ignore();   // clear '\n' left by cin >> before getline
        cout << "Name: ";
        getline(cin, e[i].name);
        cout << "Hours worked: ";
        cin  >> e[i].hours;
        cout << "Rate of pay: ";
        cin  >> e[i].rate;

        // negative hours or rate ends input early
        if(e[i].hours < 0 || e[i].rate < 0){
            cout << "Invalid hours or rate. Stopping input.\n";
            break;
        }

        e[i].gross = calcPay(e[i].hours, e[i].rate);
        count++;
    }

    n = count;   // only print accepted employees
    return e;
}

float calcPay(float hours, float rate){
    float pay = 0;
    // <=40 straight; 41-49 double; 50+ triple (9 hrs double then rest triple)
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
    // work in cents so float noise does not break the dollars
    int totalCents = static_cast<int>(amount * 100.0f + 0.5f);
    int dollars    = totalCents / 100;
    int cents      = totalCents % 100;

    // same place split as Arabic/Roman conversion
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

    // teens are special: 11 is not "Ten One"
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

/* 
 * File:   prob1.h
 * Author: Abraham Murillo
 * Purpose: Customer structure for midterm problem 1
 */

#ifndef PROB1_H
#define PROB1_H

#include <string>
using namespace std;

// One customer row — declaration only; no memory yet
struct Customer{
    int    acctNum;     // 5 digits only
    string name;
    string address;
    float  begBal;      // start of month
    float *checks;      // dynamic list of check amounts
    int    nChecks;
    float *deposits;    // dynamic list of deposit amounts
    int    nDeposits;
    float  endBal;      // after checks, deposits, possible $20 fee
};

#endif /* PROB1_H */

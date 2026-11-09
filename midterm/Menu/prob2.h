/* 
 * File:   prob2.h
 * Author: Abraham Murillo
 * Purpose: Employee structure for midterm problem 2
 */

#ifndef PROB2_H
#define PROB2_H

#include <string>
using namespace std;

// One employee row — gross is filled by calcPay, not typed in
struct Employee{
    string name;
    float  hours;   // hours worked
    float  rate;    // pay rate
    float  gross;   // calculated pay
};

#endif /* PROB2_H */

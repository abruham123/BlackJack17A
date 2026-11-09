/* 
 * File:   Problem7.cpp
 * Author: Abraham Murillo
 * Purpose: Midterm problem 7 — factor n into primes with powers
 *          Example: 300 = 2^2 * 3^1 * 5^2
 */

//System Libraries
#include <iostream>
#include <cmath>
using namespace std;

//User Libraries
#include "prob7.h"

//Function Prototypes
bool     isPrime(int);              // from ClassReview prime test
int      power(int, int &);         // count & remove this prime from n
int      nPrimes(int);              // how many distinct prime factors
Primes  *factor(int);               // required: build the structure
void     prtPrms(const Primes *);   // required: print factors
void     cleanUp(Primes *);         // free heap memory

int main(){
    int n;
    cout << "Enter an integer [2, 65000]: ";
    cin  >> n;

    if(n < 2 || n > 65000){
        cout << "Invalid input.\n";
        return 0;
    }

    Primes *p = factor(n);  //Factor builds a table of primes, we only print
    prtPrms(p);     //Prints primes
    cleanUp(p);     //Must free what factor allocated
    return 0;
}

// Same idea as ClassReview/Prime#Test.cpp
bool isPrime(int n){        //Only need to test divsors up to sqrt n
    if(n <= 1) return false;
    int nsqrt = static_cast<int>(sqrt(n));
    for(int i = 2; i <= nsqrt; i++){
        if(n % i == 0) return false;        //Nothing divides then n is prime
    }
    return true;
}

// How many times prime divides n; divide those factors out of n
int power(int prime, int &n){       // &n so dividing here changes the caller's copy of the number
     int pwr = 0;                   // example: n=300, prime=2 -> loop twice -> n becomes 75, return 
    while(n % prime == 0){
        n /= prime;
        pwr++;
    }
    return pwr;
}

// Count distinct prime factors (for sizing the array)
int nPrimes(int n){                     //First pass ONLY count how many primes so we know new[] size
    int temp  = n;
    int count = 0;
    for(int i = 2; temp > 1; i++){
        if(temp % i == 0 && isPrime(i)){
            count++;
            while(temp % i == 0) temp /= i;  // strip this prime
        }
    }
    return count;
}

Primes *factor(int n){
    Primes *p = new Primes;
    p->n       = static_cast<unsigned short>(n);
    p->nPrimes = static_cast<unsigned char>(nPrimes(n));
    p->pStrAry = new Prime[p->nPrimes];                 //pStrAry is a table, each row = its prime + power

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
        cout << p->pStrAry[i].prime << "^" << static_cast<int>(p->pStrAry[i].power);        //Cast power to int so it doesnt print as character
        if(i < p->nPrimes - 1) cout << " * ";
    }
    cout << endl;
}

void cleanUp(Primes *p){
    delete [] p->pStrAry;  // array of Prime first
    delete p;              // then the Primes structure
}

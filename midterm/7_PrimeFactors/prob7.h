/* 
 * File:   prob7.h
 * Author: Abraham Murillo
 * Purpose: Prime / Primes structures for midterm problem 7
 *          Class sizing: max power 16, max distinct primes 6 (n <= 65000)
 */

#ifndef PROB7_H
#define PROB7_H

struct Prime{
    unsigned char  power;  // exponent (fits; max needed is 16)
    unsigned short prime;  // prime factor
};

struct Primes{
    unsigned short n;        // number being factored
    unsigned char  nPrimes;  // how many distinct primes (max 6)
    Prime         *pStrAry;  // dynamic array of Prime rows
};

#endif /* PROB7_H */

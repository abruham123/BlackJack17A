/* 
 * File:   Dealer.h
 * Author: Abraham Murillo
 * Purpose: Dealer — one struct, one nested Hand
 */

#ifndef DEALER_H
#define DEALER_H

#include "Hand.h"

struct Dealer{
    Hand hand;   // nested Hand (owns its own Card array)
};

//Function Prototypes — bodies in main.cpp
Dealer *newDealer(int maxCards);
void    destroyDealer(Dealer *dlr);
bool    dealerShouldHit(const Hand &dealer);  // HIT_SOFT_17 constant

#endif /* DEALER_H */

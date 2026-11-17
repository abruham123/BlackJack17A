/* 
 * File:   Dealer.h
 * Author: Abraham Murillo
 * Purpose: Dealer — one struct, one hand
 */

#ifndef DEALER_H
#define DEALER_H

#include "Hand.h"

struct Dealer{
    // TODO: Hand hand;   // or Hand *hand; — pick one style
};

//Function Prototypes — optional heap helpers; Abraham fills bodies
Dealer *newDealer(int maxCards);
void    destroyDealer(Dealer *dlr);
bool    dealerShouldHit(const Hand &dealer);  // uses HIT_SOFT_17 later

#endif /* DEALER_H */

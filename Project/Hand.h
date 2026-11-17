/* 
 * File:   Hand.h
 * Author: Abraham Murillo
 * Purpose: One hand with its own dynamic Card array
 */

#ifndef HAND_H
#define HAND_H

#include "Card.h"

struct Hand{
    // TODO: Card *cards;
    // TODO: int nCards;
    // TODO: int maxCards;
    // TODO: int wager;   // optional until betting
};

//Function Prototypes — Abraham writes the bodies
Hand makeHand(int maxCards);
void addCard(Hand &h, Card c);
int  handValue(Hand h);
bool isSoft(const Hand &h);
bool isPair(const Hand *h);
void destroyHand(Hand &h);

#endif /* HAND_H */

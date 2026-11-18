/* 
 * File:   Hand.h
 * Author: Abraham Murillo
 * Purpose: One hand with its own dynamic Card array
 */

#ifndef HAND_H
#define HAND_H

#include "Card.h"

// Basic strategy / player choices (Ch11 enumeration) — rules of thumb later
enum Action { HIT, STAND, DOUBLE, SPLIT };

struct Hand{
    Card *cards;   // new Card[maxCards]
    int   nCards;  // how many dealt so far
    int   maxCards;
    int   wager;   // optional until full betting
};

//Function Prototypes — bodies in main.cpp
Hand makeHand(int maxCards);
void addCard(Hand &h, Card c);
void prntHand(const Hand &h);
int  handValue(Hand h);              // by value; Ace 1 or 11
bool isSoft(const Hand &h);          // const reference
bool isPair(const Hand *h);          // pointer
void destroyHand(Hand &h);

#endif /* HAND_H */

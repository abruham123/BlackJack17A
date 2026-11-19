/* 
 * File:   Deck.h
 * Author: Abraham Murillo
 * Purpose: Deck on the heap — Card array size 52 * nDecks
 */

#ifndef DECK_H
#define DECK_H

#include "Card.h"

// One shoe — main owns Deck* and calls destroyDeck
struct Deck{
    int   nCards; // 52 * nDecks
    int   top;    // next deal index
    Card *cards;  // new Card[nCards]
};

//Function Prototypes
Deck *newDeck(int nDecks);
void  fillDeck(Deck *d);
void  shuffleDeck(Deck *d);
Card  dealCard(Deck *d);
void  prntDeck(const Deck *d, int perLine);
void  destroyDeck(Deck *d);

#endif /* DECK_H */

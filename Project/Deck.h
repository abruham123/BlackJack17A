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
    // TODO: int nCards;   // 52 * nDecks
    // TODO: int top;      // next deal index
    // TODO: Card *cards;  // new Card[nCards]
};

//Function Prototypes — Abraham writes the bodies in a .cpp
Deck *newDeck(int nDecks);
void  fillDeck(Deck *d);
void  shuffleDeck(Deck *d);
Card  dealCard(Deck *d);
void  destroyDeck(Deck *d);

#endif /* DECK_H */

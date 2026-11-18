/* 
 * File:   Card.h
 * Author: Abraham Murillo
 * Purpose: One card — face/suit/value match Project/main.cpp helpers
 */

#ifndef CARD_H
#define CARD_H

// One playing card (no dynamic members)
struct Card{
    int  index;   // 0..51 within a 52-card deck cycle
    char faceVal; // from face(index)
    char suitVal; // from suit(index)
    char hardVal; // from value(index); Ace stored as 1 here
};

#endif /* CARD_H */

/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 17th, 2026, 12:10 PM
 * Purpose: Blackjack Project — Card/Deck slice (Ch9/Ch11). Not finished game.
 *          Keeps face/suit/value bridge from v1; main owns every pointer.
 */

//System Libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//User Libraries
#include "Card.h"
#include "Deck.h"

//Global Constants Only, No Global Variables
//Like PI, e, Gravity, or conversions

//Function Prototypes — v1 card helpers (keep)
char suit(int);
char face(int);
char value(int);

//Function Prototypes — Deck (also declared in Deck.h)
Deck *newDeck(int nDecks);
void  fillDeck(Deck *d);
void  shuffleDeck(Deck *d);
Card  dealCard(Deck *d);
void  prntDeck(const Deck *d, int perLine);
void  destroyDeck(Deck *d);

//Execution Begins Here
int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //Declare Variables — main owns Deck*
    int nDecks = 1;                 // start with one deck (52 cards)
    int perLine = 13;               // print one suit-row width
    Deck *deck = newDeck(nDecks);
    
    //Fill and shuffle the shoe
    fillDeck(deck);
    cout << "Deck before shuffle (" << deck->nCards << " cards):" << endl;
    prntDeck(deck, perLine);
    
    shuffleDeck(deck);
    cout << "Deck after shuffle:" << endl;
    prntDeck(deck, perLine);
    
    //Deal a few cards to show dealCard / top cursor
    cout << "Deal 5 cards:" << endl;
    for(int i = 0; i < 5; i++){
        Card c = dealCard(deck);
        cout << c.faceVal << c.suitVal
             << " (val=" << static_cast<int>(c.hardVal) << ")" << endl;
    }
    cout << "Cards left to deal: " << (deck->nCards - deck->top) << endl;
    
    //Deallocate memory — main frees every pointer
    destroyDeck(deck);
    
    //Exit stage right!
    return 0;
}

//******************************************************************************
//*                            Card helpers (v1 bridge)                        *
//******************************************************************************
char value(int n){
    n >= 0 ? n %= 13 : 0;
    n++;
    if(n > 10) n = 10;
    return n;
}

char face(int n){
    n >= 0 ? n %= 52 : 0;
    char faces[] = {'A','2','3','4','5','6','7','8','9','T','J','Q','K'};
    return faces[n % 13];
}

char suit(int n){
    n >= 0 ? n %= 52 : 0;
    char suits[] = {'S','D','C','H'};
    return suits[n / 13];
}

//******************************************************************************
//*                                  Deck                                      *
//******************************************************************************
Deck *newDeck(int nDecks){
    if(nDecks < 1) nDecks = 1;
    Deck *d = new Deck;
    d->nCards = 52 * nDecks;
    d->top = 0;
    d->cards = new Card[d->nCards];
    return d;
}

void fillDeck(Deck *d){
    // Each physical deck repeats indexes 0..51; helpers use %52 / %13
    for(int i = 0; i < d->nCards; i++){
        int idx = i % 52;
        d->cards[i].index   = idx;
        d->cards[i].faceVal = face(idx);
        d->cards[i].suitVal = suit(idx);
        d->cards[i].hardVal = value(idx);
    }
    d->top = 0;
}

void shuffleDeck(Deck *d){
    // Fisher-Yates using pointer/array access (Ch9)
    for(int i = d->nCards - 1; i > 0; i--){
        int j = rand() % (i + 1);
        Card temp = d->cards[i];
        d->cards[i] = d->cards[j];
        d->cards[j] = temp;
    }
    d->top = 0;
}

Card dealCard(Deck *d){
    // If shoe emptied, reshuffle (simple stub for later game loop)
    if(d->top >= d->nCards){
        shuffleDeck(d);
    }
    return d->cards[d->top++];
}

void prntDeck(const Deck *d, int perLine){
    cout << endl;
    for(int i = 0; i < d->nCards; i++){
        cout << d->cards[i].faceVal << d->cards[i].suitVal << " ";
        if(i % perLine == (perLine - 1)) cout << endl;
    }
    cout << endl;
}

void destroyDeck(Deck *d){
    delete [] d->cards;
    delete d;
}

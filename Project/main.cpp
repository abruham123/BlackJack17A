/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 17th, 2026, 1:20 PM
 * Purpose: Blackjack Project v4 — Card/Deck + Hand/Dealer/Player slice.
 *          Not a finished game. Keeps face/suit/value bridge.
 *          main owns every pointer and frees them before exit.
 */

//System Libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
using namespace std;

//User Libraries
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Dealer.h"
#include "Player.h"

//Global Constants Only, No Global Variables
const bool HIT_SOFT_17 = false;   // stand on all 17s when false

//Function Prototypes — card helpers (v1 bridge)
char suit(int);
char face(int);
char value(int);

//Function Prototypes — Deck
Deck *newDeck(int nDecks);
void  fillDeck(Deck *d);
void  shuffleDeck(Deck *d);
Card  dealCard(Deck *d);
void  prntDeck(const Deck *d, int perLine);
void  destroyDeck(Deck *d);

//Function Prototypes — Hand
Hand makeHand(int maxCards);
void addCard(Hand &h, Card c);
void prntHand(const Hand &h);
int  handValue(Hand h);
bool isSoft(const Hand &h);
bool isPair(const Hand *h);
void destroyHand(Hand &h);

//Function Prototypes — Dealer / Player
Dealer *newDealer(int maxCards);
void    destroyDealer(Dealer *dlr);
bool    dealerShouldHit(const Hand &dealer);
Player *newPlayer(const char name[], int maxHands, int maxCards, float bank);
void    prntPlayer(const Player *p);
void    destroyPlayer(Player *p);

//Execution Begins Here
int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //Declare — main owns these heap pointers
    int nDecks = 1;
    int maxCards = 12;   // enough for hit testing
    int maxHands = 4;    // room for splits later
    Deck   *deck   = newDeck(nDecks);
    Dealer *dealer = newDealer(maxCards);
    Player *player = newPlayer("Abraham", maxHands, maxCards, 100.0f);
    
    //Fill and shuffle shoe
    fillDeck(deck);
    shuffleDeck(deck);
    
    //Deal two each: player hand row 0, then dealer (nested Hand)
    addCard(player->hands[0], dealCard(deck));
    addCard(dealer->hand,     dealCard(deck));
    addCard(player->hands[0], dealCard(deck));
    addCard(dealer->hand,     dealCard(deck));
    
    //Display (C-string name + string displayName + nested hands)
    cout << "=== Blackjack Project v4 (Hand / Dealer / Player slice) ===" << endl;
    prntPlayer(player);
    cout << "Player hand: ";
    prntHand(player->hands[0]);
    cout << "  value=" << handValue(player->hands[0])
         << "  soft=" << (isSoft(player->hands[0]) ? "yes" : "no")
         << "  pair=" << (isPair(&player->hands[0]) ? "yes" : "no") << endl;
    
    cout << "Dealer hand: ";
    prntHand(dealer->hand);
    cout << "  value=" << handValue(dealer->hand)
         << "  shouldHit=" << (dealerShouldHit(dealer->hand) ? "yes" : "no") << endl;
    
    cout << "Cards left in shoe: " << (deck->nCards - deck->top) << endl;
    
    //Example enum use (Ch11) — not full strategy yet
    Action tip = isPair(&player->hands[0]) ? SPLIT : HIT;
    cout << "Rule-of-thumb Action enum sample: "
         << (tip == SPLIT ? "SPLIT" : tip == HIT ? "HIT" :
             tip == STAND ? "STAND" : "DOUBLE") << endl;
    
    //Deallocate — main frees every pointer
    destroyPlayer(player);
    destroyDealer(dealer);
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
    for(int i = d->nCards - 1; i > 0; i--){
        int j = rand() % (i + 1);
        Card temp = d->cards[i];
        d->cards[i] = d->cards[j];
        d->cards[j] = temp;
    }
    d->top = 0;
}

Card dealCard(Deck *d){
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

//******************************************************************************
//*                                  Hand                                      *
//******************************************************************************
Hand makeHand(int maxCards){
    if(maxCards < 2) maxCards = 2;
    Hand h;
    h.maxCards = maxCards;
    h.nCards = 0;
    h.wager = 0;
    h.cards = new Card[h.maxCards];
    return h;
}

void addCard(Hand &h, Card c){
    if(h.nCards < h.maxCards){
        h.cards[h.nCards++] = c;
    }
}

void prntHand(const Hand &h){
    for(int i = 0; i < h.nCards; i++){
        cout << h.cards[i].faceVal << h.cards[i].suitVal << " ";
    }
}

int handValue(Hand h){
    // By-value copy of Hand shell; Ace counted as 11 when it does not bust
    int total = 0;
    int aces = 0;
    for(int i = 0; i < h.nCards; i++){
        total += static_cast<int>(h.cards[i].hardVal);
        if(h.cards[i].faceVal == 'A') aces++;
    }
    // hardVal stores Ace as 1; try +10 once if soft is legal
    if(aces > 0 && total + 10 <= 21){
        total += 10;
    }
    return total;
}

bool isSoft(const Hand &h){
    int total = 0;
    int aces = 0;
    for(int i = 0; i < h.nCards; i++){
        total += static_cast<int>(h.cards[i].hardVal);
        if(h.cards[i].faceVal == 'A') aces++;
    }
    return (aces > 0 && total + 10 <= 21);
}

bool isPair(const Hand *h){
    if(h == nullptr || h->nCards != 2) return false;
    // Pair by blackjack rank value (10/J/Q/K all 10)
    return h->cards[0].hardVal == h->cards[1].hardVal;
}

void destroyHand(Hand &h){
    delete [] h.cards;
    h.cards = nullptr;
    h.nCards = 0;
    h.maxCards = 0;
}

//******************************************************************************
//*                              Dealer / Player                               *
//******************************************************************************
Dealer *newDealer(int maxCards){
    Dealer *d = new Dealer;
    d->hand = makeHand(maxCards);
    return d;
}

void destroyDealer(Dealer *dlr){
    if(dlr == nullptr) return;
    destroyHand(dlr->hand);
    delete dlr;
}

bool dealerShouldHit(const Hand &dealer){
    int v = handValue(dealer);
    if(v < 17) return true;
    if(v > 17) return false;
    // Exactly 17: hit only if soft and HIT_SOFT_17 is true
    if(HIT_SOFT_17 && isSoft(dealer)) return true;
    return false;
}

Player *newPlayer(const char name[], int maxHands, int maxCards, float bank){
    if(maxHands < 1) maxHands = 1;
    Player *p = new Player;
    // C-string copy (leave room for '\0')
    strncpy(p->name, name, 31);
    p->name[31] = '\0';
    p->displayName = string(p->name);   // std::string mirror
    p->bankroll = bank;
    p->maxHands = maxHands;
    p->nHands = 1;                      // start with one hand (row 0)
    p->hands = new Hand[p->maxHands];
    for(int i = 0; i < p->maxHands; i++){
        p->hands[i] = makeHand(maxCards);
    }
    return p;
}

void prntPlayer(const Player *p){
    cout << "Player C-string name: " << p->name << endl;
    cout << "Player string displayName: " << p->displayName << endl;
    cout << "Bankroll: $" << p->bankroll
         << "  hands in use: " << p->nHands << "/" << p->maxHands << endl;
}

void destroyPlayer(Player *p){
    if(p == nullptr) return;
    for(int i = 0; i < p->maxHands; i++){
        destroyHand(p->hands[i]);
    }
    delete [] p->hands;
    delete p;
}

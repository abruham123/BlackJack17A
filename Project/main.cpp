/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 17th, 2026, 7:00 PM
 * Purpose: Blackjack Project v5 — adds Ch12 binary GameConfig + PlayerRecord.
 *          Not a finished playable game loop yet. Keeps face/suit/value bridge.
 *          main owns every pointer and frees them before exit.
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

//User Libraries
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Dealer.h"
#include "Player.h"
#include "GameConfig.h"
#include "PlayerRecord.h"

//Global Constants Only, No Global Variables
const bool  HIT_SOFT_17 = false;   // stand on all 17s when false
const float BJ_PAY      = 1.5f;    // 3:2

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

//Function Prototypes — GameConfig / PlayerRecord (Ch12)
void setConfig(GameConfig &cfg, int nDecks, bool hitSoft17, float bjPay);
bool saveConfig(fstream &out, const GameConfig &cfg);
bool loadConfig(fstream &in, GameConfig &cfg);
void prntConfig(const GameConfig &cfg);
PlayerRecord *newRecords(int n);
void destroyRecords(PlayerRecord *r);
bool saveRecords(fstream &out, const PlayerRecord *r, int n);
bool loadRecords(fstream &in, PlayerRecord *r, int n);
bool readRecordAt(fstream &in, PlayerRecord &rec, int index);
void prntRecord(const PlayerRecord &rec);

//Execution Begins Here
int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //----- Ch12: plain config + binary files (in|out|binary = simultaneous) -----
    GameConfig cfg;
    setConfig(cfg, 1, HIT_SOFT_17, BJ_PAY);
    
    fstream cfgFile;
    fstream recFile;
    cfgFile.open("config.bin", ios::in|ios::out|ios::binary|ios::trunc);
    recFile.open("players.bin", ios::in|ios::out|ios::binary|ios::trunc);
    if(!cfgFile){
        cout << "Error: could not open config.bin" << endl;
        return 1;
    }
    if(!recFile){
        cout << "Error: could not open players.bin" << endl;
        cfgFile.close();
        return 1;
    }
    
    //Write config, rewind, read it back
    saveConfig(cfgFile, cfg);
    cfgFile.clear();
    cfgFile.seekg(0, ios::beg);
    GameConfig cfg2;
    loadConfig(cfgFile, cfg2);
    
    //PlayerRecord array on the heap — main owns
    int nRec = 3;
    PlayerRecord *recs = newRecords(nRec);
    strncpy(recs[0].name, "Abraham", 31); recs[0].name[31]='\0';
    recs[0].bankroll = 100.00f; recs[0].games = 0; recs[0].wins = 0; recs[0].losses = 0;
    strncpy(recs[1].name, "Lehr", 31);    recs[1].name[31]='\0';
    recs[1].bankroll = 250.50f; recs[1].games = 10; recs[1].wins = 6; recs[1].losses = 4;
    strncpy(recs[2].name, "Guest", 31);   recs[2].name[31]='\0';
    recs[2].bankroll = 50.00f;  recs[2].games = 2;  recs[2].wins = 1; recs[2].losses = 1;
    
    saveRecords(recFile, recs, nRec);
    
    //Random-access read of record index 1 (Ch12.9)
    PlayerRecord one;
    readRecordAt(recFile, one, 1);
    
    //----- Existing Card/Deck/Hand deal demo (still v4 features) -----
    int maxCards = 12;
    int maxHands = 4;
    Deck   *deck   = newDeck(cfg2.nDecks);
    Dealer *dealer = newDealer(maxCards);
    Player *player = newPlayer("Abraham", maxHands, maxCards, recs[0].bankroll);
    
    fillDeck(deck);
    shuffleDeck(deck);
    
    addCard(player->hands[0], dealCard(deck));
    addCard(dealer->hand,     dealCard(deck));
    addCard(player->hands[0], dealCard(deck));
    addCard(dealer->hand,     dealCard(deck));
    
    cout << fixed << setprecision(2);
    cout << "=== Blackjack Project v5 (binary config + records) ===" << endl;
    prntConfig(cfg2);
    cout << "Random-access record[1]: ";
    prntRecord(one);
    
    prntPlayer(player);
    cout << "Player hand: ";
    prntHand(player->hands[0]);
    cout << "  value=" << handValue(player->hands[0]) << endl;
    cout << "Dealer hand: ";
    prntHand(dealer->hand);
    cout << "  value=" << handValue(dealer->hand) << endl;
    
    //Deallocate — main frees every pointer
    destroyPlayer(player);
    destroyDealer(dealer);
    destroyDeck(deck);
    destroyRecords(recs);
    cfgFile.close();
    recFile.close();
    
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

//******************************************************************************
//*                     GameConfig / PlayerRecord (Ch12 binary)                *
//******************************************************************************
void setConfig(GameConfig &cfg, int nDecks, bool hitSoft17, float bjPay){
    cfg.nDecks = (nDecks < 1) ? 1 : nDecks;
    cfg.hitSoft17 = hitSoft17;
    cfg.bjPay = bjPay;
}

bool saveConfig(fstream &out, const GameConfig &cfg){
    // Pass stream by reference; write whole POD struct
    out.write(reinterpret_cast<const char*>(&cfg), sizeof(GameConfig));
    return static_cast<bool>(out);
}

bool loadConfig(fstream &in, GameConfig &cfg){
    in.read(reinterpret_cast<char*>(&cfg), sizeof(GameConfig));
    return static_cast<bool>(in);
}

void prntConfig(const GameConfig &cfg){
    cout << "GameConfig: nDecks=" << cfg.nDecks
         << "  hitSoft17=" << (cfg.hitSoft17 ? "true" : "false")
         << "  bjPay=" << cfg.bjPay << endl;
}

PlayerRecord *newRecords(int n){
    if(n < 1) n = 1;
    PlayerRecord *r = new PlayerRecord[n];
    for(int i = 0; i < n; i++){
        r[i].name[0] = '\0';
        r[i].bankroll = 0.0f;
        r[i].games = 0;
        r[i].wins = 0;
        r[i].losses = 0;
    }
    return r;
}

void destroyRecords(PlayerRecord *r){
    delete [] r;
}

bool saveRecords(fstream &out, const PlayerRecord *r, int n){
    // Write count then the array (structure records)
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    out.write(reinterpret_cast<const char*>(r), n * sizeof(PlayerRecord));
    return static_cast<bool>(out);
}

bool loadRecords(fstream &in, PlayerRecord *r, int n){
    int count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(int));
    if(count > n) count = n;   // do not overrun caller buffer
    in.read(reinterpret_cast<char*>(r), count * sizeof(PlayerRecord));
    return static_cast<bool>(in);
}

bool readRecordAt(fstream &in, PlayerRecord &rec, int index){
    // Layout: [int n][PlayerRecord * n] — skip count, then seek to index
    long cursor = static_cast<long>(sizeof(int) + index * sizeof(PlayerRecord));
    in.clear();                         // clear eof if set
    in.seekg(cursor, ios::beg);
    in.read(reinterpret_cast<char*>(&rec), sizeof(PlayerRecord));
    return static_cast<bool>(in);
}

void prntRecord(const PlayerRecord &rec){
    cout << rec.name << "  bankroll=$" << rec.bankroll
         << "  games=" << rec.games
         << "  W/L=" << rec.wins << "/" << rec.losses << endl;
}

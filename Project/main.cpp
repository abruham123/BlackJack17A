/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 17th, 2026, 7:15 PM
 * Purpose: Blackjack Project v6 — interactive hit/stand/double/split + settle.
 *          Keeps face/suit/value bridge. main owns every pointer / frees them.
 *          Comments below explain syntax on prototypes and in main.
 */

//System Libraries
#include <iostream>   // cout, cin, endl
#include <iomanip>    // fixed, setprecision — format money
#include <cstdlib>    // srand, rand
#include <ctime>      // time
#include <cstring>    // strncpy — C-string copy
#include <string>     // std::string
#include <fstream>    // fstream — binary files
using namespace std;

//User Libraries — bring in struct types + some prototypes from headers
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Dealer.h"
#include "Player.h"
#include "GameConfig.h"
#include "PlayerRecord.h"

//Global Constants Only, No Global Variables
const bool  HIT_SOFT_17 = false;   // false = stand on all 17s
const float BJ_PAY      = 1.5f;    // natural blackjack pays 3:2 → 1.5 * bet

//==============================================================================
// Function Prototypes
// Syntax tips:
//   TYPE name(args);     → "this function exists; body is later"
//   TYPE*                → returns/points to heap memory (caller often delete)
//   TYPE &               → reference: same object, can change it (no copy)
//   const TYPE &         → reference, but read-only (faster, safer)
//   TYPE name[]          → array decays to pointer (C-string = char[])
//   void                 → returns nothing
//   bool                 → returns true/false
//==============================================================================

//----- Card helpers (v1 bridge): take int index, return one char -----
char suit(int);     // suit letter S/D/C/H from card index
char face(int);     // face letter A..K from card index
char value(int);    // hard pip value 1..10 (Ace stored as 1 here)

//----- Deck: Deck* means "address of a Deck on the heap" -----
Deck *newDeck(int nDecks);                 // returns pointer → new Deck + Card[]
void  fillDeck(Deck *d);                   // Deck* = pass by pointer (can change deck)
void  shuffleDeck(Deck *d);
Card  dealCard(Deck *d);                   // returns a Card by value (copy of one card)
void  prntDeck(const Deck *d, int perLine);// const Deck* = may read, not modify
void  destroyDeck(Deck *d);                // delete [] cards; delete deck

//----- Hand: & = reference; const & = read-only reference; * = pointer -----
Hand makeHand(int maxCards);               // returns Hand by value (shell + new cards)
void addCard(Hand &h, Card c);             // Hand& = change caller's hand in place
void clearHand(Hand &h);                   // reset nCards to 0 (keep allocated array)
void prntHand(const Hand &h);              // const Hand& = print without copying/changing
int  handValue(Hand h);                    // Hand by value = works on a copy of the shell
bool isSoft(const Hand &h);                // true if Ace counting as 11
bool isPair(const Hand *h);                // Hand* = optional / address of a hand
void destroyHand(Hand &h);                 // delete [] the hand's Card array

//----- Dealer / Player -----
Dealer *newDealer(int maxCards);           // Dealer* on heap; nested Hand inside
void    destroyDealer(Dealer *dlr);
bool    dealerShouldHit(const Hand &dealer);
void    playDealer(Dealer *dlr, Deck *deck); // hits until stand rule
Player *newPlayer(const char name[], int maxHands, int maxCards, float bank);
void    prntPlayer(const Player *p);       // const Player* = read-only pointer
void    destroyPlayer(Player *p);
void    settleHand(Player *p, const Hand &playerH, const Hand &dealerH,
                   bool playerBJ, float bjPay);  // update bankroll

//----- Ch12 binary I/O: fstream& must be by reference (streams are not copyable) -----
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
    // argc / argv = command-line args (unused here; professor style keeps them)
    
    // srand needs an unsigned seed; time(0) is "now" so shuffle changes each run
    srand(static_cast<unsigned int>(time(0)));
    
    cout << fixed << setprecision(2);  // money prints like 100.00
    
    //----- Setup config (plain struct on the STACK — no new needed) -----
    GameConfig cfg;   // local variable; lives until main ends
    setConfig(cfg, 1, HIT_SOFT_17, BJ_PAY);
    
    // fstream objects also on the stack; open for read+write binary
    // | combines flags: in AND out AND binary AND trunc (start empty)
    fstream cfgFile;
    fstream recFile;
    cfgFile.open("config.bin", ios::in|ios::out|ios::binary|ios::trunc);
    recFile.open("players.bin", ios::in|ios::out|ios::binary|ios::trunc);
    if(!cfgFile || !recFile){          // stream converts to false if open failed
        cout << "Error opening binary files." << endl;
        return 1;                      // non-zero = error exit
    }
    saveConfig(cfgFile, cfg);
    
    //----- Heap objects: * means pointer; new returns an address -----
    // main OWNS these → must destroy* / delete before exit
    const int maxCards = 12;
    const int maxHands = 4;
    Deck   *deck   = newDeck(cfg.nDecks);              // pointer variable
    Dealer *dealer = newDealer(maxCards);
    Player *player = newPlayer("Abraham", maxHands, maxCards, 100.0f);
    
    fillDeck(deck);
    shuffleDeck(deck);
    
    cout << "=== Blackjack Project v6 (play loop) ===" << endl;
    prntConfig(cfg);
    prntPlayer(player);
    cout << "Commands: h=hit  s=stand  d=double  p=split  q=quit round early" << endl;
    
    //----- Play rounds until player quits or bankroll is empty -----
    char again = 'y';
    while(again == 'y' || again == 'Y'){
        if(player->bankroll <= 0){
            cout << "Bankroll empty. Game over." << endl;
            break;   // leave the while loop
        }
        
        // Bet: cin >> reads a float into bet (stack variable)
        float bet = 0;
        cout << "\nBankroll $" << player->bankroll << "  Enter bet: ";
        cin >> bet;
        if(bet <= 0 || bet > player->bankroll){
            cout << "Invalid bet." << endl;
            cout << "Play again (y/n)? ";
            cin >> again;
            continue;  // next while iteration
        }
        
        // Clear hands for a new round (arrays stay allocated)
        clearHand(dealer->hand);
        for(int i = 0; i < player->maxHands; i++){
            clearHand(player->hands[i]);
            player->hands[i].wager = 0;
        }
        player->nHands = 1;
        player->hands[0].wager = bet;
        player->bankroll -= bet;   // take bet up front
        
        // Deal two each: player->hands[0] is first hand (row 0)
        // -> means "follow the pointer, then use member"
        addCard(player->hands[0], dealCard(deck));
        addCard(dealer->hand,     dealCard(deck));
        addCard(player->hands[0], dealCard(deck));
        addCard(dealer->hand,     dealCard(deck));
        
        cout << "Dealer shows: " << dealer->hand.cards[0].faceVal
             << dealer->hand.cards[0].suitVal << " ??" << endl;
        cout << "Your hand: ";
        prntHand(player->hands[0]);
        cout << " value=" << handValue(player->hands[0]) << endl;
        
        bool playerBJ = (player->hands[0].nCards == 2 &&
                         handValue(player->hands[0]) == 21);
        bool dealerBJ = (dealer->hand.nCards == 2 &&
                         handValue(dealer->hand) == 21);
        
        // Natural blackjack check before hitting
        if(playerBJ || dealerBJ){
            cout << "Dealer hand: ";
            prntHand(dealer->hand);
            cout << endl;
            settleHand(player, player->hands[0], dealer->hand, playerBJ, cfg.bjPay);
        }else{
            // Optional split on first action if pair
            char choice;
            bool firstAction = true;
            bool handDone = false;
            
            while(!handDone){
                cout << "Action (h/s/d";
                if(firstAction && isPair(&player->hands[0]) &&
                   player->nHands < player->maxHands &&
                   player->bankroll >= player->hands[0].wager){
                    cout << "/p";
                }
                cout << "): ";
                cin >> choice;   // reads one char
                
                if(choice == 'h' || choice == 'H'){
                    addCard(player->hands[0], dealCard(deck));
                    cout << "Your hand: ";
                    prntHand(player->hands[0]);
                    cout << " value=" << handValue(player->hands[0]) << endl;
                    if(handValue(player->hands[0]) > 21){
                        cout << "Bust!" << endl;
                        handDone = true;
                    }
                }else if(choice == 's' || choice == 'S'){
                    handDone = true;
                }else if(choice == 'd' || choice == 'D'){
                    // Double: one more card, double wager if funds allow
                    if(player->bankroll >= player->hands[0].wager){
                        player->bankroll -= player->hands[0].wager;
                        player->hands[0].wager *= 2;
                        addCard(player->hands[0], dealCard(deck));
                        cout << "Doubled. Hand: ";
                        prntHand(player->hands[0]);
                        cout << " value=" << handValue(player->hands[0]) << endl;
                        handDone = true;
                    }else{
                        cout << "Not enough bankroll to double." << endl;
                    }
                }else if((choice == 'p' || choice == 'P') && firstAction &&
                         isPair(&player->hands[0]) &&
                         player->nHands < player->maxHands &&
                         player->bankroll >= player->hands[0].wager){
                    // Split: move 2nd card to hands[1], deal one to each
                    player->bankroll -= player->hands[0].wager;
                    player->hands[1].wager = player->hands[0].wager;
                    addCard(player->hands[1], player->hands[0].cards[1]);
                    player->hands[0].nCards = 1;
                    player->nHands = 2;
                    addCard(player->hands[0], dealCard(deck));
                    addCard(player->hands[1], dealCard(deck));
                    cout << "Split hand0: "; prntHand(player->hands[0]);
                    cout << "  hand1: "; prntHand(player->hands[1]); cout << endl;
                    // Simple: after split, auto-finish each with stand after optional hits
                    for(int h = 0; h < player->nHands; h++){
                        char ch2 = 'h';
                        while(handValue(player->hands[h]) < 21 &&
                              (ch2 == 'h' || ch2 == 'H')){
                            cout << "Hand " << h << ": ";
                            prntHand(player->hands[h]);
                            cout << " value=" << handValue(player->hands[h])
                                 << "  (h/s): ";
                            cin >> ch2;
                            if(ch2 == 'h' || ch2 == 'H'){
                                addCard(player->hands[h], dealCard(deck));
                            }
                        }
                    }
                    handDone = true;
                }else if(choice == 'q' || choice == 'Q'){
                    handDone = true;
                }else{
                    cout << "Unknown command." << endl;
                }
                firstAction = false;
            }
            
            // Play dealer if any player hand still alive
            bool anyAlive = false;
            for(int h = 0; h < player->nHands; h++){
                if(handValue(player->hands[h]) <= 21) anyAlive = true;
            }
            if(anyAlive){
                playDealer(dealer, deck);
            }
            cout << "Dealer final: ";
            prntHand(dealer->hand);
            cout << " value=" << handValue(dealer->hand) << endl;
            
            for(int h = 0; h < player->nHands; h++){
                cout << "Settle hand " << h << ": ";
                prntHand(player->hands[h]);
                cout << endl;
                settleHand(player, player->hands[h], dealer->hand, false, cfg.bjPay);
            }
        }
        
        cout << "Bankroll now $" << player->bankroll << endl;
        cout << "Play again (y/n)? ";
        cin >> again;
    }
    
    //----- Save a simple PlayerRecord snapshot (Ch12) -----
    PlayerRecord *recs = newRecords(1);
    strncpy(recs[0].name, player->name, 31);
    recs[0].name[31] = '\0';
    recs[0].bankroll = player->bankroll;
    recs[0].games = 1;
    recs[0].wins = 0;
    recs[0].losses = 0;
    saveRecords(recFile, recs, 1);
    cout << "Saved record: ";
    prntRecord(recs[0]);
    
    //----- Teardown: free EVERYTHING main allocated -----
    destroyRecords(recs);
    destroyPlayer(player);   // frees each Hand's cards, then hands[], then Player
    destroyDealer(dealer);
    destroyDeck(deck);
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

void clearHand(Hand &h){
    // Keep the Card array; just mark empty for the next round
    h.nCards = 0;
    h.wager = 0;
}

void prntHand(const Hand &h){
    for(int i = 0; i < h.nCards; i++){
        cout << h.cards[i].faceVal << h.cards[i].suitVal << " ";
    }
}

int handValue(Hand h){
    int total = 0;
    int aces = 0;
    for(int i = 0; i < h.nCards; i++){
        total += static_cast<int>(h.cards[i].hardVal);
        if(h.cards[i].faceVal == 'A') aces++;
    }
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
    if(HIT_SOFT_17 && isSoft(dealer)) return true;
    return false;
}

void playDealer(Dealer *dlr, Deck *deck){
    while(dealerShouldHit(dlr->hand)){
        addCard(dlr->hand, dealCard(deck));
    }
}

Player *newPlayer(const char name[], int maxHands, int maxCards, float bank){
    if(maxHands < 1) maxHands = 1;
    Player *p = new Player;
    strncpy(p->name, name, 31);
    p->name[31] = '\0';
    p->displayName = string(p->name);
    p->bankroll = bank;
    p->maxHands = maxHands;
    p->nHands = 1;
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

void settleHand(Player *p, const Hand &playerH, const Hand &dealerH,
                bool playerBJ, float bjPay){
    int pv = handValue(playerH);
    int dv = handValue(dealerH);
    float bet = static_cast<float>(playerH.wager);
    
    if(pv > 21){
        cout << "  Result: player bust — lose $" << bet << endl;
        // bet already removed
    }else if(dv > 21){
        cout << "  Result: dealer bust — win $" << bet << endl;
        p->bankroll += bet * 2.0f;          // stake back + win
    }else if(playerBJ && dv != 21){
        cout << "  Result: blackjack — win $" << (bet * bjPay) << endl;
        p->bankroll += bet + bet * bjPay;   // stake + 3:2
    }else if(pv > dv){
        cout << "  Result: player wins — win $" << bet << endl;
        p->bankroll += bet * 2.0f;
    }else if(pv < dv){
        cout << "  Result: dealer wins — lose $" << bet << endl;
    }else{
        cout << "  Result: push — bet returned" << endl;
        p->bankroll += bet;
    }
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
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    out.write(reinterpret_cast<const char*>(r), n * sizeof(PlayerRecord));
    return static_cast<bool>(out);
}

bool loadRecords(fstream &in, PlayerRecord *r, int n){
    int count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(int));
    if(count > n) count = n;
    in.read(reinterpret_cast<char*>(r), count * sizeof(PlayerRecord));
    return static_cast<bool>(in);
}

bool readRecordAt(fstream &in, PlayerRecord &rec, int index){
    long cursor = static_cast<long>(sizeof(int) + index * sizeof(PlayerRecord));
    in.clear();
    in.seekg(cursor, ios::beg);
    in.read(reinterpret_cast<char*>(&rec), sizeof(PlayerRecord));
    return static_cast<bool>(in);
}

void prntRecord(const PlayerRecord &rec){
    cout << rec.name << "  bankroll=$" << rec.bankroll
         << "  games=" << rec.games
         << "  W/L=" << rec.wins << "/" << rec.losses << endl;
}

/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 18th, 2026, 8:10 PM
 * Purpose: Blackjack Project v7 — finished playable game (no insurance, no W/L).
 *          Light UI cleanup. Comments explain prototypes, main, and -> below.
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cctype>     // toupper, isalpha — Ch10 character testing
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
const bool  HIT_SOFT_17 = false;
const float BJ_PAY      = 1.5f;    // 3:2

//==============================================================================
// Function Prototypes  (* = pointer, & = reference, const = read-only)
//==============================================================================
char suit(int);
char face(int);
char value(int);
void copyName(char dest[], const char src[]);  // tiny own C-string helper

Deck *newDeck(int nDecks);          // returns Deck* (heap address)
void  fillDeck(Deck *d);            // Deck* — change the deck through pointer
void  shuffleDeck(Deck *d);
Card  dealCard(Deck *d);
void  prntDeck(const Deck *d, int perLine);
void  destroyDeck(Deck *d);

Hand makeHand(int maxCards);
void addCard(Hand &h, Card c);      // Hand& — same hand, no copy
void clearHand(Hand &h);
void prntHand(const Hand &h);
int  handValue(Hand h);             // by value — copy of Hand shell
bool isSoft(const Hand &h);
bool isPair(const Hand *h);         // Hand* — address of a hand
void destroyHand(Hand &h);

Dealer *newDealer(int maxCards);
void    destroyDealer(Dealer *dlr);
bool    dealerShouldHit(const Hand &dealer);
void    playDealer(Dealer *dlr, Deck *deck);
Player *newPlayer(const char name[], int maxHands, int maxCards, float bank);
void    prntPlayer(const Player *p);
void    destroyPlayer(Player *p);
void    settleHand(Player *p, const Hand &playerH, const Hand &dealerH,
                   bool playerBJ, float bjPay);

void setConfig(GameConfig &cfg, int nDecks, bool hitSoft17, float bjPay);
bool saveConfig(fstream &out, const GameConfig &cfg);   // fstream& required
bool loadConfig(fstream &in, GameConfig &cfg);
void prntConfig(const GameConfig &cfg);
PlayerRecord *newRecords(int n);
void destroyRecords(PlayerRecord *r);
bool saveRecords(fstream &out, const PlayerRecord *r, int n);
bool loadRecords(fstream &in, PlayerRecord *r, int n);
bool readRecordAt(fstream &in, PlayerRecord &rec, int index);
void prntRecord(const PlayerRecord &rec);

//Execution Begins Here
int main(int, char**) {
    srand(static_cast<unsigned int>(time(0)));
    cout << fixed << setprecision(2);
    
    // Stack config (no new)
    GameConfig cfg;
    setConfig(cfg, 1, HIT_SOFT_17, BJ_PAY);
    
    // Two binary files open for input+output at once (Ch12.10)
    fstream cfgFile;
    fstream recFile;
    cfgFile.open("config.bin", ios::in|ios::out|ios::binary|ios::trunc);
    recFile.open("players.bin", ios::in|ios::out|ios::binary|ios::trunc);
    if(!cfgFile || !recFile){
        cout << "Could not open save files.\n";
        return 1;
    }
    saveConfig(cfgFile, cfg);
    
    // Heap: * holds an address. -> later means "go to that address, use member"
    const int maxCards = 12;
    const int maxHands = 4;
    Deck   *deck   = newDeck(cfg.nDecks);
    Dealer *dealer = newDealer(maxCards);
    Player *player = newPlayer("Abraham", maxHands, maxCards, 100.0f);
    
    fillDeck(deck);
    shuffleDeck(deck);
    
    cout << "\n===== BLACKJACK v7 =====\n";
    cout << "Bankroll starts at $100.  No insurance.\n";
    cout << "[h] hit  [s] stand  [d] double  [p] split\n";
    
    char again = 'y';
    while(again == 'y' || again == 'Y'){
        // player->bankroll : pointer player, member bankroll
        if(player->bankroll <= 0){
            cout << "\nOut of money. Thanks for playing.\n";
            break;
        }
        
        float bet = 0;
        cout << "\n----------------------------\n";
        cout << "Bankroll: $" << player->bankroll << "\nBet: ";
        cin >> bet;
        if(bet <= 0 || bet > player->bankroll){
            cout << "Invalid bet.\nAgain (y/n)? ";
            cin >> again;
            continue;
        }
        
        clearHand(dealer->hand);
        for(int i = 0; i < player->maxHands; i++){
            clearHand(player->hands[i]);
        }
        player->nHands = 1;
        player->hands[0].wager = bet;
        player->bankroll -= bet;
        
        addCard(player->hands[0], dealCard(deck));
        addCard(dealer->hand,     dealCard(deck));
        addCard(player->hands[0], dealCard(deck));
        addCard(dealer->hand,     dealCard(deck));
        
        cout << "Dealer: " << dealer->hand.cards[0].faceVal
             << dealer->hand.cards[0].suitVal << " ??\n";
        cout << "You:    ";
        prntHand(player->hands[0]);
        cout << "(" << handValue(player->hands[0]) << ")\n";
        
        bool playerBJ = (player->hands[0].nCards == 2 &&
                         handValue(player->hands[0]) == 21);
        bool dealerBJ = (dealer->hand.nCards == 2 &&
                         handValue(dealer->hand) == 21);
        
        if(playerBJ || dealerBJ){
            cout << "Dealer: ";
            prntHand(dealer->hand);
            cout << "(" << handValue(dealer->hand) << ")\n";
            settleHand(player, player->hands[0], dealer->hand, playerBJ, cfg.bjPay);
        }else{
            char choice;
            bool firstAction = true;
            bool handDone = false;
            
            while(!handDone){
                cout << "Action [h/s/d";
                if(firstAction && isPair(&player->hands[0]) &&
                   player->nHands < player->maxHands &&
                   player->bankroll >= player->hands[0].wager) cout << "/p";
                cout << "]: ";
                cin >> choice;
                choice = static_cast<char>(toupper(static_cast<unsigned char>(choice)));
                
                if(choice == 'H'){
                    addCard(player->hands[0], dealCard(deck));
                    cout << "You: ";
                    prntHand(player->hands[0]);
                    cout << "(" << handValue(player->hands[0]) << ")\n";
                    if(handValue(player->hands[0]) > 21){
                        cout << "Bust.\n";
                        handDone = true;
                    }
                }else if(choice == 'S'){
                    handDone = true;
                }else if(choice == 'D'){
                    if(player->bankroll >= player->hands[0].wager){
                        player->bankroll -= player->hands[0].wager;
                        player->hands[0].wager *= 2;
                        addCard(player->hands[0], dealCard(deck));
                        cout << "Double. You: ";
                        prntHand(player->hands[0]);
                        cout << "(" << handValue(player->hands[0]) << ")\n";
                        handDone = true;
                    }else cout << "Need more cash to double.\n";
                }else if(choice == 'P' && firstAction && isPair(&player->hands[0]) &&
                         player->nHands < player->maxHands &&
                         player->bankroll >= player->hands[0].wager){
                    player->bankroll -= player->hands[0].wager;
                    player->hands[1].wager = player->hands[0].wager;
                    addCard(player->hands[1], player->hands[0].cards[1]);
                    player->hands[0].nCards = 1;
                    player->nHands = 2;
                    addCard(player->hands[0], dealCard(deck));
                    addCard(player->hands[1], dealCard(deck));
                    cout << "Split.\n";
                    for(int h = 0; h < player->nHands; h++){
                        char ch2 = 'H';
                        while(handValue(player->hands[h]) < 21 && ch2 == 'H'){
                            cout << "Hand " << (h+1) << ": ";
                            prntHand(player->hands[h]);
                            cout << "(" << handValue(player->hands[h]) << ") [h/s]: ";
                            cin >> ch2;
                            ch2 = static_cast<char>(toupper(static_cast<unsigned char>(ch2)));
                            if(ch2 == 'H') addCard(player->hands[h], dealCard(deck));
                        }
                    }
                    handDone = true;
                }else if(isalpha(static_cast<unsigned char>(choice))){
                    cout << "Use h, s, d, or p.\n";
                }
                firstAction = false;
            }
            
            bool anyAlive = false;
            for(int h = 0; h < player->nHands; h++){
                if(handValue(player->hands[h]) <= 21) anyAlive = true;
            }
            if(anyAlive) playDealer(dealer, deck);
            
            cout << "Dealer: ";
            prntHand(dealer->hand);
            cout << "(" << handValue(dealer->hand) << ")\n";
            
            for(int h = 0; h < player->nHands; h++){
                settleHand(player, player->hands[h], dealer->hand, false, cfg.bjPay);
            }
        }
        
        cout << "Bankroll: $" << player->bankroll << "\nAgain (y/n)? ";
        cin >> again;
    }
    
    // Save name + bankroll only (no W/L tracking)
    PlayerRecord *recs = newRecords(1);
    copyName(recs[0].name, player->name);
    recs[0].bankroll = player->bankroll;
    saveRecords(recFile, recs, 1);
    cout << "\nSaved: ";
    prntRecord(recs[0]);
    cout << "Thanks for playing.\n";
    
    destroyRecords(recs);
    destroyPlayer(player);
    destroyDealer(dealer);
    destroyDeck(deck);
    cfgFile.close();
    recFile.close();
    
    //Exit stage right!
    return 0;
}

//******************************************************************************
//* Card helpers — plain ints/chars, no pointers here                          *
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

// Own C-string copy (Ch10) — dest/src are char arrays → decay to char*
void copyName(char dest[], const char src[]){
    strncpy(dest, src, 31);
    dest[31] = '\0';   // always end C-string with null
}

//******************************************************************************
//* Deck — every d->... means: follow pointer d, then use that member          *
//******************************************************************************
Deck *newDeck(int nDecks){
    if(nDecks < 1) nDecks = 1;
    Deck *d = new Deck;          // d holds the ADDRESS of a Deck
    d->nCards = 52 * nDecks;     // same as (*d).nCards
    d->top = 0;
    d->cards = new Card[d->nCards];  // d->cards is a Card* (array on heap)
    return d;                    // return the address to main
}

void fillDeck(Deck *d){
    // Pointer compare: skip null (Ch9 comparing pointers)
    if(d == nullptr) return;
    for(int i = 0; i < d->nCards; i++){
        int idx = i % 52;
        // Pointer arithmetic: *(d->cards + i) is the same as d->cards[i]
        Card *pCard = d->cards + i;   // address of element i
        pCard->index   = idx;         // -> on a Card*
        pCard->faceVal = face(idx);
        pCard->suitVal = suit(idx);
        pCard->hardVal = value(idx);
    }
    d->top = 0;
}

void shuffleDeck(Deck *d){
    if(d == nullptr) return;
    for(int i = d->nCards - 1; i > 0; i--){
        int j = rand() % (i + 1);
        // Swap through the pointer's array
        Card temp = d->cards[i];
        d->cards[i] = d->cards[j];
        d->cards[j] = temp;
    }
    d->top = 0;
}

Card dealCard(Deck *d){
    if(d->top >= d->nCards) shuffleDeck(d);
    // d->top++ uses the int at that address, then advances it
    return d->cards[d->top++];
}

void prntDeck(const Deck *d, int perLine){
    cout << endl;
    for(int i = 0; i < d->nCards; i++){
        // const Deck* still allows -> for reading members
        cout << d->cards[i].faceVal << d->cards[i].suitVal << " ";
        if(i % perLine == (perLine - 1)) cout << endl;
    }
    cout << endl;
}

void destroyDeck(Deck *d){
    if(d == nullptr) return;     // comparing pointer to null
    delete [] d->cards;          // free the Card array first
    d->cards = nullptr;          // pointer init / clear after delete
    delete d;                    // then free the Deck shell
}

//******************************************************************************
//* Hand — & means "the real Hand"; * means "address of a Hand"                *
//******************************************************************************
Hand makeHand(int maxCards){
    if(maxCards < 2) maxCards = 2;
    Hand h;                      // local Hand shell
    h.maxCards = maxCards;
    h.nCards = 0;
    h.wager = 0;
    h.cards = new Card[h.maxCards];  // . not -> because h is not a pointer
    return h;
}

void addCard(Hand &h, Card c){
    // h is an alias — h.cards changes the caller's hand
    if(h.nCards < h.maxCards) h.cards[h.nCards++] = c;
}

void clearHand(Hand &h){
    h.nCards = 0;
    h.wager = 0;
}

void prntHand(const Hand &h){
    for(int i = 0; i < h.nCards; i++){
        cout << h.cards[i].faceVal << h.cards[i].suitVal << " ";
    }
}

int handValue(Hand h){
    // by-value: h is a copy of the shell (still shares the same cards pointer)
    int total = 0, aces = 0;
    for(int i = 0; i < h.nCards; i++){
        total += static_cast<int>(h.cards[i].hardVal);
        if(h.cards[i].faceVal == 'A') aces++;
    }
    if(aces > 0 && total + 10 <= 21) total += 10;
    return total;
}

bool isSoft(const Hand &h){
    int total = 0, aces = 0;
    for(int i = 0; i < h.nCards; i++){
        total += static_cast<int>(h.cards[i].hardVal);
        if(h.cards[i].faceVal == 'A') aces++;
    }
    return (aces > 0 && total + 10 <= 21);
}

bool isPair(const Hand *h){
    // h is a pointer → must use ->  (h->nCards means (*h).nCards)
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
//* Dealer / Player — nested Hand; Player holds Hand* array for splits         *
//******************************************************************************
Dealer *newDealer(int maxCards){
    Dealer *d = new Dealer;      // d → Dealer on heap
    d->hand = makeHand(maxCards);// nested Hand lives inside *d
    return d;
}

void destroyDealer(Dealer *dlr){
    if(dlr == nullptr) return;
    destroyHand(dlr->hand);      // free cards inside nested hand
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
    // Both args are pointers: use -> to reach their members
    while(dealerShouldHit(dlr->hand)){
        addCard(dlr->hand, dealCard(deck));
    }
}

Player *newPlayer(const char name[], int maxHands, int maxCards, float bank){
    if(maxHands < 1) maxHands = 1;
    Player *p = new Player;
    copyName(p->name, name);             // C-string member via ->
    p->displayName = string(p->name);    // std::string member via ->
    p->bankroll = bank;
    p->maxHands = maxHands;
    p->nHands = 1;
    p->hands = new Hand[p->maxHands];    // array of Hand structs
    for(int i = 0; i < p->maxHands; i++){
        p->hands[i] = makeHand(maxCards);
    }
    return p;
}

void prntPlayer(const Player *p){
    // const Player* — may read with ->, may not assign into *p
    cout << p->displayName << "  $" << p->bankroll << endl;
}

void destroyPlayer(Player *p){
    if(p == nullptr) return;
    for(int i = 0; i < p->maxHands; i++){
        destroyHand(p->hands[i]);
    }
    delete [] p->hands;   // free Hand array
    delete p;             // free Player shell
}

void settleHand(Player *p, const Hand &playerH, const Hand &dealerH,
                bool playerBJ, float bjPay){
    int pv = handValue(playerH);
    int dv = handValue(dealerH);
    float bet = static_cast<float>(playerH.wager);
    
    if(pv > 21){
        cout << "  Bust — lose $" << bet << endl;
    }else if(dv > 21){
        cout << "  Dealer bust — win $" << bet << endl;
        p->bankroll += bet * 2.0f;       // -> changes player's money
    }else if(playerBJ && dv != 21){
        cout << "  Blackjack — win $" << (bet * bjPay) << endl;
        p->bankroll += bet + bet * bjPay;
    }else if(pv > dv){
        cout << "  You win $" << bet << endl;
        p->bankroll += bet * 2.0f;
    }else if(pv < dv){
        cout << "  Dealer wins — lose $" << bet << endl;
    }else{
        cout << "  Push — bet returned\n";
        p->bankroll += bet;
    }
}

//******************************************************************************
//* Ch12 binary — pass fstream by reference; write/read POD structs            *
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
    cout << "Decks=" << cfg.nDecks
         << "  soft17=" << (cfg.hitSoft17 ? "hit" : "stand")
         << "  BJ pays " << cfg.bjPay << endl;
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
    // Random access: seek to byte offset of record index
    long cursor = static_cast<long>(sizeof(int) + index * sizeof(PlayerRecord));
    in.clear();
    in.seekg(cursor, ios::beg);
    in.read(reinterpret_cast<char*>(&rec), sizeof(PlayerRecord));
    return static_cast<bool>(in);
}

void prntRecord(const PlayerRecord &rec){
    // Name + bankroll only (no W/L tracking)
    cout << rec.name << "  $" << rec.bankroll << endl;
}

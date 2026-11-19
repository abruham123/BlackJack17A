/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 18th, 2026, 8:10 PM
 * Purpose: Blackjack Project v7 — finished playable game (no insurance, no W/L).
 *          Light UI cleanup. Comments explain prototypes, main, and -> below.
 *
 * How the pieces fit (game logic + chapters):
 *   Card  = one playing card (Ch11 struct)
 *   Deck  = shoe of Cards on the heap (Ch9 new/delete + Ch11 array of structs)
 *   Hand  = cards + wager for one seat/split (Ch11; & vs * in helpers)
 *   Dealer = nested Hand (Ch11.6) — one hole/up card pair, then draws
 *   Player = name (C-string + string), bankroll, Hand[] for splits (Ch10+11)
 *   GameConfig / PlayerRecord = settings + save blob (Ch12 binary)
 *
 * Round flow in main:
 *   bet -> clear hands -> deal 2 each -> (BJ?) or player acts -> dealer draws
 *   -> settle vs dealer -> ask again -> save bankroll -> destroy heap
 *
 * Checkoff tags below look like  [Ch9.8]  [Ch10.3]  [Ch11.9]  [Ch12.7]
 * so you can find scored concepts while reading the game code.
 */

//System Libraries
#include <iostream>   // cin/cout — game UI
#include <iomanip>    // fixed/setprecision for money  [Ch12.2 formatting]
#include <cstdlib>    // srand/rand for shuffle
#include <ctime>      // time(0) seed
#include <cstring>    // strncpy — C-string library  [Ch10.4]
#include <cctype>     // toupper, isalpha — Ch10 character testing
#include <string>     // std::string displayName     [Ch10.7]
#include <fstream>    // fstream binary save/load    [Ch12]
using namespace std;

//User Libraries — ADTs: each .h is a struct type the game owns
#include "Card.h"          // [Ch11.1/.2] face/suit/hard value
#include "Deck.h"          // [Ch11] shoe: Card* + top index
#include "Hand.h"          // [Ch11] + enum Action  [Ch11.11]
#include "Dealer.h"        // [Ch11.6] nested Hand
#include "Player.h"        // [Ch10 C-string/string] + Hand[]
#include "GameConfig.h"    // [Ch12.8] POD settings record
#include "PlayerRecord.h"  // [Ch12.8/.9] save name/bankroll

//Global Constants Only, No Global Variables
// House rules shared by dealer logic + config seed
const bool  HIT_SOFT_17 = false;   // false = stand on all 17s (soft or hard)
const float BJ_PAY      = 1.5f;    // 3:2 blackjack payout multiplier

//==============================================================================
// Function Prototypes  (* = pointer, & = reference, const = read-only)
// Syntax reminder used all game long:
//   Deck *d     -> pass address; body uses d->member
//   Hand &h     -> alias to caller's hand; body uses h.member
//   const Hand& -> read-only alias (can't change cards/wager)
//   Hand h      -> copy of Hand shell (handValue doesn't need to mutate)
//==============================================================================
char suit(int);                    // map index -> 'S','D','C','H'
char face(int);                    // map index -> 'A'..'K'
char value(int);                   // hard pip value (A=1, face=10)
void copyName(char dest[], const char src[]);  // tiny own C-string helper [Ch10.6]

Deck *newDeck(int nDecks);          // returns Deck* (heap address) [Ch9.9]
void  fillDeck(Deck *d);            // Deck* — change the deck through pointer
void  shuffleDeck(Deck *d);         // randomize order before dealing
Card  dealCard(Deck *d);            // next card from top; reshuffle if empty
void  prntDeck(const Deck *d, int perLine);
void  destroyDeck(Deck *d);         // delete[] cards, then delete Deck [Ch9.8]

Hand makeHand(int maxCards);        // build empty Hand (returns struct) [Ch11.8]
void addCard(Hand &h, Card c);      // Hand& — same hand, no copy [Ch11.7]
void clearHand(Hand &h);            // reset nCards/wager between rounds
void prntHand(const Hand &h);       // show faces for UI
int  handValue(Hand h);             // by value — copy of Hand shell (Ace soft/hard)
bool isSoft(const Hand &h);         // Ace counted as 11 without bust
bool isPair(const Hand *h);         // Hand* — address of a hand [Ch11.9]
void destroyHand(Hand &h);          // free that Hand's Card array

Dealer *newDealer(int maxCards);    // heap Dealer with nested Hand
void    destroyDealer(Dealer *dlr);
bool    dealerShouldHit(const Hand &dealer);  // uses HIT_SOFT_17 rule
void    playDealer(Dealer *dlr, Deck *deck);  // draw until stand
Player *newPlayer(const char name[], int maxHands, int maxCards, float bank);
void    prntPlayer(const Player *p);
void    destroyPlayer(Player *p);
void    settleHand(Player *p, const Hand &playerH, const Hand &dealerH,
                   bool playerBJ, float bjPay);  // pay / take bet into bankroll

void setConfig(GameConfig &cfg, int nDecks, bool hitSoft17, float bjPay);
bool saveConfig(fstream &out, const GameConfig &cfg);   // fstream& required [Ch12.3]
bool loadConfig(fstream &in, GameConfig &cfg);
void prntConfig(const GameConfig &cfg);
PlayerRecord *newRecords(int n);    // heap array of save records
void destroyRecords(PlayerRecord *r);
bool saveRecords(fstream &out, const PlayerRecord *r, int n);  // [Ch12.7/.8]
bool loadRecords(fstream &in, PlayerRecord *r, int n);
bool readRecordAt(fstream &in, PlayerRecord &rec, int index);  // [Ch12.9 seek]
void prntRecord(const PlayerRecord &rec);

//Execution Begins Here — owns the heap tree: Deck, Dealer, Player, Records
int main(int, char**) {
    // Seed RNG once so shuffle differs each run
    srand(static_cast<unsigned int>(time(0)));
    cout << fixed << setprecision(2);   // money always two decimals [Ch12.2]
    
    // Stack config (no new) — table rules live here, then get binary-saved
    GameConfig cfg;                     // [Ch11.4] initialize via setter
    setConfig(cfg, 1, HIT_SOFT_17, BJ_PAY);
    
    // Two binary files open for input+output at once (Ch12.10)
    // Same fstream can write config and later player records [Ch12.6 multiple files]
    fstream cfgFile;                    // [Ch12.1] file operations
    fstream recFile;
    cfgFile.open("config.bin", ios::in|ios::out|ios::binary|ios::trunc);   // [Ch12.7]
    recFile.open("players.bin", ios::in|ios::out|ios::binary|ios::trunc);
    if(!cfgFile || !recFile){           // [Ch12.4] basic stream error test
        cout << "Could not open save files.\n";
        return 1;
    }
    saveConfig(cfgFile, cfg);           // write POD GameConfig bytes [Ch12.5/.8]
    
    // Heap: * holds an address. -> later means "go to that address, use member"
    // Ownership: main creates, main destroys (bottom of main).
    const int maxCards = 12;            // room for hit sequences
    const int maxHands = 4;             // room for splits (we use up to 2)
    Deck   *deck   = newDeck(cfg.nDecks);                 // [Ch9.2/.8] pointer var
    Dealer *dealer = newDealer(maxCards);                 // nested Hand inside
    Player *player = newPlayer("Abraham", maxHands, maxCards, 100.0f);
    
    fillDeck(deck);                     // build 52*n Cards through Deck*
    shuffleDeck(deck);                  // randomize before first deal
    
    cout << "\n===== BLACKJACK v7 =====\n";
    cout << "Bankroll starts at $100.  No insurance.\n";
    cout << "[h] hit  [s] stand  [d] double  [p] split\n";
    
    // -------- GAME LOOP: one bet = one round --------
    char again = 'y';
    while(again == 'y' || again == 'Y'){
        // player->bankroll : pointer player, member bankroll
        // Game rule: can't play if broke
        if(player->bankroll <= 0){
            cout << "\nOut of money. Thanks for playing.\n";
            break;
        }
        
        // --- Betting ---
        // Take chips from bankroll now; settleHand may put them back (+ win)
        float bet = 0;
        cout << "\n----------------------------\n";
        cout << "Bankroll: $" << player->bankroll << "\nBet: ";
        cin >> bet;
        if(bet <= 0 || bet > player->bankroll){
            cout << "Invalid bet.\nAgain (y/n)? ";
            cin >> again;
            continue;
        }
        
        // --- Reset table for a new round ---
        // dealer->hand is a nested Hand (use . after -> to reach Hand members)
        clearHand(dealer->hand);
        for(int i = 0; i < player->maxHands; i++){
            clearHand(player->hands[i]);   // [Ch11.5] array of Hand structs
        }
        player->nHands = 1;                // start with one seat; split can make 2
        player->hands[0].wager = bet;      // this hand's stake
        player->bankroll -= bet;           // chips leave the bank until settle
        
        // --- Initial deal: player, dealer, player, dealer ---
        // dealCard uses Deck*; addCard uses Hand& so the same Hand grows
        addCard(player->hands[0], dealCard(deck));
        addCard(dealer->hand,     dealCard(deck));
        addCard(player->hands[0], dealCard(deck));
        addCard(dealer->hand,     dealCard(deck));
        
        // Show one dealer upcard; hole card hidden until settle
        cout << "Dealer: " << dealer->hand.cards[0].faceVal
             << dealer->hand.cards[0].suitVal << " ??\n";
        cout << "You:    ";
        prntHand(player->hands[0]);
        cout << "(" << handValue(player->hands[0]) << ")\n";
        
        // Natural blackjack? two cards totaling 21
        bool playerBJ = (player->hands[0].nCards == 2 &&
                         handValue(player->hands[0]) == 21);
        bool dealerBJ = (dealer->hand.nCards == 2 &&
                         handValue(dealer->hand) == 21);
        
        if(playerBJ || dealerBJ){
            // Instant settle — no hit/stand needed
            cout << "Dealer: ";
            prntHand(dealer->hand);
            cout << "(" << handValue(dealer->hand) << ")\n";
            settleHand(player, player->hands[0], dealer->hand, playerBJ, cfg.bjPay);
        }else{
            // --- Player decisions (maps to enum Action ideas in Hand.h) ---
            char choice;
            bool firstAction = true;   // double/split only legal on first decision
            bool handDone = false;
            
            while(!handDone){
                // Offer split only when: first action + pair + seat free + cash left
                cout << "Action [h/s/d";
                if(firstAction && isPair(&player->hands[0]) &&   // [Ch11.9] Hand*
                   player->nHands < player->maxHands &&
                   player->bankroll >= player->hands[0].wager) cout << "/p";
                cout << "]: ";
                cin >> choice;
                // [Ch10.2] case conversion so 'h' and 'H' match
                choice = static_cast<char>(toupper(static_cast<unsigned char>(choice)));
                
                if(choice == 'H'){
                    // Hit: take one card from the same Deck* into this Hand&
                    addCard(player->hands[0], dealCard(deck));
                    cout << "You: ";
                    prntHand(player->hands[0]);
                    cout << "(" << handValue(player->hands[0]) << ")\n";
                    if(handValue(player->hands[0]) > 21){
                        cout << "Bust.\n";
                        handDone = true;   // bust ends player turn for this path
                    }
                }else if(choice == 'S'){
                    // Stand: keep current total; dealer plays next
                    handDone = true;
                }else if(choice == 'D'){
                    // Double: one more equal bet, exactly one card, then done
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
                    // Split: second Hand gets the pair mate; each Hand gets a card
                    // Syntax: player->hands[i] is Hand (use .); &hands[0] is Hand*
                    player->bankroll -= player->hands[0].wager;          // second bet
                    player->hands[1].wager = player->hands[0].wager;
                    addCard(player->hands[1], player->hands[0].cards[1]);
                    player->hands[0].nCards = 1;   // first hand keeps only card 0
                    player->nHands = 2;            // now two live seats
                    addCard(player->hands[0], dealCard(deck));
                    addCard(player->hands[1], dealCard(deck));
                    cout << "Split.\n";
                    // Play each split hand with hit/stand only
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
                }else if(isalpha(static_cast<unsigned char>(choice))){  // [Ch10.1]
                    cout << "Use h, s, d, or p.\n";
                }
                firstAction = false;   // after first decision, no double/split
            }
            
            // --- Dealer turn ---
            // Only draw if at least one player hand is still alive (<=21)
            bool anyAlive = false;
            for(int h = 0; h < player->nHands; h++){
                if(handValue(player->hands[h]) <= 21) anyAlive = true;
            }
            if(anyAlive) playDealer(dealer, deck);  // both args are pointers
            
            cout << "Dealer: ";
            prntHand(dealer->hand);
            cout << "(" << handValue(dealer->hand) << ")\n";
            
            // Settle every live seat against the same dealer Hand
            // settleHand takes Player* so it can do p->bankroll += ...
            for(int h = 0; h < player->nHands; h++){
                settleHand(player, player->hands[h], dealer->hand, false, cfg.bjPay);
            }
        }
        
        cout << "Bankroll: $" << player->bankroll << "\nAgain (y/n)? ";
        cin >> again;
    }
    
    // -------- Save + teardown --------
    // Save name + bankroll only (no W/L tracking)
    PlayerRecord *recs = newRecords(1);           // [Ch9] heap array of records
    copyName(recs[0].name, player->name);         // [Ch10.6] own C-string fn
    recs[0].bankroll = player->bankroll;          // final chips after all rounds
    saveRecords(recFile, recs, 1);                // count + POD array [Ch12.7/.8]
    cout << "\nSaved: ";
    prntRecord(recs[0]);
    cout << "Thanks for playing.\n";
    
    // Destroy in reverse-ish dependency order: records, player hands, dealer, deck
    destroyRecords(recs);
    destroyPlayer(player);   // frees each Hand's cards, then hands[], then Player
    destroyDealer(dealer);
    destroyDeck(deck);
    cfgFile.close();         // [Ch12.1] close both streams
    recFile.close();
    
    //Exit stage right!
    return 0;
}

//******************************************************************************
//* Card helpers — plain ints/chars, no pointers here                          *
//* Game link: Deck fill calls these to stamp face/suit/hardVal on each Card   *
//******************************************************************************
char value(int n){
    // Hard value used by handValue(); Ace starts as 1, soft may add +10
    n >= 0 ? n %= 13 : 0;
    n++;
    if(n > 10) n = 10;       // J/Q/K -> 10
    return n;
}

char face(int n){
    // Display character shown in prntHand / dealer upcard
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
// Used when saving Player.name into PlayerRecord.name
void copyName(char dest[], const char src[]){
    strncpy(dest, src, 31);   // [Ch10.4] library C-string function
    dest[31] = '\0';          // always end C-string with null  [Ch10.3]
}

//******************************************************************************
//* Deck — every d->... means: follow pointer d, then use that member          *
//* Game link: main's Deck* is the shoe; dealCard feeds every Hand on the table*
//******************************************************************************
Deck *newDeck(int nDecks){
    if(nDecks < 1) nDecks = 1;
    Deck *d = new Deck;          // d holds the ADDRESS of a Deck  [Ch9.8 alloc]
    d->nCards = 52 * nDecks;     // same as (*d).nCards            [Ch9.1/.2]
    d->top = 0;                  // next deal index
    d->cards = new Card[d->nCards];  // d->cards is a Card* (array on heap) [Ch9.3]
    return d;                    // return the address to main     [Ch9.9]
}

void fillDeck(Deck *d){
    // Pointer compare: skip null (Ch9 comparing pointers)
    if(d == nullptr) return;     // [Ch9.5/.6] init/compare
    for(int i = 0; i < d->nCards; i++){
        int idx = i % 52;
        // Pointer arithmetic: *(d->cards + i) is the same as d->cards[i]
        Card *pCard = d->cards + i;   // address of element i  [Ch9.4]
        pCard->index   = idx;         // -> on a Card*
        pCard->faceVal = face(idx);   // display + Ace test later
        pCard->suitVal = suit(idx);
        pCard->hardVal = value(idx);  // scoring pip
    }
    d->top = 0;                  // start dealing from the front
}

void shuffleDeck(Deck *d){
    // Fisher–Yates using rand(); called at start and when shoe empties
    if(d == nullptr) return;
    for(int i = d->nCards - 1; i > 0; i--){
        int j = rand() % (i + 1);
        // Swap through the pointer's array  [Ch9.3 arrays/pointers]
        Card temp = d->cards[i];
        d->cards[i] = d->cards[j];
        d->cards[j] = temp;
    }
    d->top = 0;
}

Card dealCard(Deck *d){
    // Game: every hit/deal path comes through here
    if(d->top >= d->nCards) shuffleDeck(d);  // shoe empty -> reshuffle
    // d->top++ uses the int at that address, then advances it
    return d->cards[d->top++];
}

void prntDeck(const Deck *d, int perLine){
    // Debug helper — not required during play UI
    cout << endl;
    for(int i = 0; i < d->nCards; i++){
        // const Deck* still allows -> for reading members  [Ch9.7 const pointer]
        cout << d->cards[i].faceVal << d->cards[i].suitVal << " ";
        if(i % perLine == (perLine - 1)) cout << endl;
    }
    cout << endl;
}

void destroyDeck(Deck *d){
    if(d == nullptr) return;     // comparing pointer to null
    delete [] d->cards;          // free the Card array first  [Ch9.8]
    d->cards = nullptr;          // pointer init / clear after delete
    delete d;                    // then free the Deck shell
}

//******************************************************************************
//* Hand — & means "the real Hand"; * means "address of a Hand"                *
//* Game link: dealer has one Hand; player has Hand[] so split can use hands[1]*
//******************************************************************************
Hand makeHand(int maxCards){
    // Called by newDealer / newPlayer to equip empty seats
    if(maxCards < 2) maxCards = 2;
    Hand h;                      // local Hand shell  [Ch11.4 initialize]
    h.maxCards = maxCards;
    h.nCards = 0;
    h.wager = 0;
    h.cards = new Card[h.maxCards];  // . not -> because h is not a pointer
    return h;                    // [Ch11.8] return structure by value
}

void addCard(Hand &h, Card c){
    // h is an alias — h.cards changes the caller's hand  [Ch11.7 args]
    // Game: hit, deal, double, and split all grow a Hand this way
    if(h.nCards < h.maxCards) h.cards[h.nCards++] = c;
}

void clearHand(Hand &h){
    // Between rounds: keep the Card array, just mark empty + wipe wager
    h.nCards = 0;
    h.wager = 0;
}

void prntHand(const Hand &h){
    // UI only — walks cards with .
    for(int i = 0; i < h.nCards; i++){
        cout << h.cards[i].faceVal << h.cards[i].suitVal << " ";
    }
}

int handValue(Hand h){
    // by-value: h is a copy of the shell (still shares the same cards pointer)
    // Soft Ace: if +10 still <= 21, count one Ace as 11
    int total = 0, aces = 0;
    for(int i = 0; i < h.nCards; i++){
        total += static_cast<int>(h.cards[i].hardVal);  // [Ch10.5]-ish cast
        if(h.cards[i].faceVal == 'A') aces++;
    }
    if(aces > 0 && total + 10 <= 21) total += 10;
    return total;
}

bool isSoft(const Hand &h){
    // Used by dealerShouldHit when HIT_SOFT_17 is true
    int total = 0, aces = 0;
    for(int i = 0; i < h.nCards; i++){
        total += static_cast<int>(h.cards[i].hardVal);
        if(h.cards[i].faceVal == 'A') aces++;
    }
    return (aces > 0 && total + 10 <= 21);
}

bool isPair(const Hand *h){
    // h is a pointer → must use ->  (h->nCards means (*h).nCards)
    // Game: gates the 'p' split option in main
    if(h == nullptr || h->nCards != 2) return false;
    return h->cards[0].hardVal == h->cards[1].hardVal;
}

void destroyHand(Hand &h){
    // Part of Player/Dealer teardown — free this seat's Card array
    delete [] h.cards;
    h.cards = nullptr;
    h.nCards = 0;
    h.maxCards = 0;
}

//******************************************************************************
//* Dealer / Player — nested Hand; Player holds Hand* array for splits         *
//* Game link: playDealer mutates dealer->hand; settleHand mutates p->bankroll *
//******************************************************************************
Dealer *newDealer(int maxCards){
    Dealer *d = new Dealer;      // d → Dealer on heap
    d->hand = makeHand(maxCards);// nested Hand lives inside *d  [Ch11.6]
    return d;
}

void destroyDealer(Dealer *dlr){
    if(dlr == nullptr) return;
    destroyHand(dlr->hand);      // free cards inside nested hand
    delete dlr;
}

bool dealerShouldHit(const Hand &dealer){
    // House rule: hit <17; stand >17; on 17 optionally hit soft
    int v = handValue(dealer);
    if(v < 17) return true;
    if(v > 17) return false;
    if(HIT_SOFT_17 && isSoft(dealer)) return true;
    return false;
}

void playDealer(Dealer *dlr, Deck *deck){
    // Both args are pointers: use -> to reach their members
    // Game: only called when a player hand is still alive
    while(dealerShouldHit(dlr->hand)){
        addCard(dlr->hand, dealCard(deck));
    }
}

Player *newPlayer(const char name[], int maxHands, int maxCards, float bank){
    // Builds the human seat: identity + money + Hand array for splits
    if(maxHands < 1) maxHands = 1;
    Player *p = new Player;
    copyName(p->name, name);             // C-string member via ->  [Ch10.3]
    p->displayName = string(p->name);    // std::string member via -> [Ch10.7]
    p->bankroll = bank;
    p->maxHands = maxHands;
    p->nHands = 1;
    p->hands = new Hand[p->maxHands];    // array of Hand structs  [Ch11.5]
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
    // Free every Hand's cards, then the Hand array, then the Player shell
    if(p == nullptr) return;
    for(int i = 0; i < p->maxHands; i++){
        destroyHand(p->hands[i]);
    }
    delete [] p->hands;   // free Hand array
    delete p;             // free Player shell
}

void settleHand(Player *p, const Hand &playerH, const Hand &dealerH,
                bool playerBJ, float bjPay){
    // Compare one player Hand to dealer; update money through Player*
    // Bet was already removed from bankroll when the round started
    int pv = handValue(playerH);
    int dv = handValue(dealerH);
    float bet = static_cast<float>(playerH.wager);
    
    if(pv > 21){
        cout << "  Bust — lose $" << bet << endl;
        // chips already gone — nothing to add back
    }else if(dv > 21){
        cout << "  Dealer bust — win $" << bet << endl;
        p->bankroll += bet * 2.0f;       // -> changes player's money (stake+win)
    }else if(playerBJ && dv != 21){
        cout << "  Blackjack — win $" << (bet * bjPay) << endl;
        p->bankroll += bet + bet * bjPay;  // stake back + 3:2
    }else if(pv > dv){
        cout << "  You win $" << bet << endl;
        p->bankroll += bet * 2.0f;
    }else if(pv < dv){
        cout << "  Dealer wins — lose $" << bet << endl;
    }else{
        cout << "  Push — bet returned\n";
        p->bankroll += bet;              // stake only
    }
}

//******************************************************************************
//* Ch12 binary — pass fstream by reference; write/read POD structs            *
//* Game link: save table rules + end-of-session PlayerRecord (name/bankroll)  *
//******************************************************************************
void setConfig(GameConfig &cfg, int nDecks, bool hitSoft17, float bjPay){
    // Fill GameConfig on the stack before binary write
    cfg.nDecks = (nDecks < 1) ? 1 : nDecks;
    cfg.hitSoft17 = hitSoft17;
    cfg.bjPay = bjPay;
}

bool saveConfig(fstream &out, const GameConfig &cfg){
    // [Ch12.3] fstream& + [Ch12.5] write + [Ch12.7] binary bytes of struct
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
    // Heap array so we can grow to multiple saved players later if needed
    if(n < 1) n = 1;
    PlayerRecord *r = new PlayerRecord[n];
    for(int i = 0; i < n; i++){
        r[i].name[0] = '\0';       // empty C-string
        r[i].bankroll = 0.0f;
        r[i].games = 0;            // fields exist but this project doesn't track W/L
        r[i].wins = 0;
        r[i].losses = 0;
    }
    return r;
}

void destroyRecords(PlayerRecord *r){
    delete [] r;
}

bool saveRecords(fstream &out, const PlayerRecord *r, int n){
    // File layout: int count, then n * sizeof(PlayerRecord)  [Ch12.8]
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
    // Random access: seek to byte offset of record index  [Ch12.9]
    // Layout matches saveRecords: skip leading int, then index * record size
    long cursor = static_cast<long>(sizeof(int) + index * sizeof(PlayerRecord));
    in.clear();
    in.seekg(cursor, ios::beg);   // [Ch12.5] member function seekg
    in.read(reinterpret_cast<char*>(&rec), sizeof(PlayerRecord));
    return static_cast<bool>(in);
}

void prntRecord(const PlayerRecord &rec){
    // Name + bankroll only (no W/L tracking)
    cout << rec.name << "  $" << rec.bankroll << endl;
}

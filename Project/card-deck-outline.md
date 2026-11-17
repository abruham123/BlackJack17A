/*
 * File:   card-deck-outline.md
 * Author: Abraham Murillo
 * Purpose: Outline only — Card/Deck growth from Project/main.cpp (not a finished game)
 *
 * Bridge from v1 helpers in Project/main.cpp:
 *   char suit(int);  char face(int);  char value(int);
 * Keep those mappings when filling Card / Deck.
 *
 * Ownership (see docs/architecture.md):
 *   main owns every pointer and frees them before exit
 *   GameConfig | Deck(+Card[]) | Dealer(+Hand) | Player(+Hand[]) | PlayerRecord[]
 *
 * Course map: Ch9 heap · Ch10 strings · Ch11 structs · Ch12 binary
 */

# Card / Deck outline (BJ-4) — prototypes & stubs only

Abraham fills **structure member lists** and **function bodies**. Agents: do not write a full Blackjack game here.

Suggested header files under `Project/` (stubs optional):

| Header | Role |
|---|---|
| `Card.h` | One card (wraps face/suit/value from index) |
| `Deck.h` | Heap shoe: `Card` array, `52 * nDecks` |
| `Hand.h` | One hand + its own dynamic `Card` array |
| `Dealer.h` | One dealer shell → one `Hand` |
| `Player.h` | One player → `Hand` array on heap (splits) |
| `GameConfig.h` | Plain config struct (also binary) |
| `PlayerRecord.h` | One save-record row (also binary array) |

---

## 1. Headers Abraham fills (struct fields — outline)

### `Card` (Ch11)
- Fields Abraham chooses, e.g. index and/or face/suit/value chars — **must stay consistent** with `Project/main.cpp` `face` / `suit` / `value`.
- No dynamic members on `Card` itself.

### `Deck` (Ch9 + Ch11) — one struct on the heap
- `int nCards;` / `int top;` (deal cursor)
- `Card *cards;` → `new Card[52 * nDecks]`

### `Hand` (Ch9 + Ch11)
- `Card *cards;` → that hand’s own array
- `int nCards;` / `int maxCards;`
- `int wager;` (optional until betting)

### `Dealer` — one struct, one hand
- `Hand hand;` **or** `Hand *hand;` (pick one style and stay consistent)

### `Player` — one struct
- `char name[32];` and/or `string displayName;` (Ch10)
- `float bankroll;`
- `Hand *hands;` — **heap array**; row 0 = original, later rows = splits
- `int nHands;` / `int maxHands;`

### `GameConfig` — plain struct (also binary file)
- e.g. `int nDecks;`, `bool hitSoft17;`, `float bjPay;` (3:2 → 1.5f)
- No pointers inside if you want a simple `.write` of the whole struct

### `PlayerRecord` — one row; array on heap + binary file
- e.g. name C-string, bankroll, games played, wins/losses — **prefer POD / fixed arrays** for easy binary I/O

---

## 2. Prototypes Abraham must write (grouped by ownership tree)

Keep Lehr order in `.cpp`: System Libraries → User Libraries → Global Constants → **these prototypes** → `main` → definitions.

### From v1 `Project/main.cpp` (keep)
```cpp
char suit(int);
char face(int);
char value(int);
```

### GameConfig (plain + binary)
```cpp
void  setConfig(GameConfig &cfg, int nDecks);          // by reference
bool  saveConfig(fstream &out, const GameConfig &cfg); // ios::binary
bool  loadConfig(fstream &in, GameConfig &cfg);
```

### Deck (heap — main owns pointer)
```cpp
Deck *newDeck(int nDecks);           // new Deck + new Card[]
void  fillDeck(Deck *d);             // use face/suit/value(i)
void  shuffleDeck(Deck *d);          // pointer swaps / indexes
Card  dealCard(Deck *d);             // advance top
void  destroyDeck(Deck *d);          // delete [] cards; delete d;
```

### Hand / Dealer
```cpp
Hand  makeHand(int maxCards);        // by value return; allocates cards
void  addCard(Hand &h, Card c);      // by reference
int   handValue(Hand h);             // by value; Ace 1 or 11 — TODO
bool  isSoft(const Hand &h);         // const reference
bool  isPair(const Hand *h);         // pointer
void  destroyHand(Hand &h);          // delete [] h.cards
Dealer *newDealer(int maxCards);     // optional heap dealer
void   destroyDealer(Dealer *dlr);
```

### Player (Hand array = splits)
```cpp
Player *newPlayer(int maxHands, int maxCards);
void    destroyPlayer(Player *p);    // free each hand’s cards, then hands[], then p
// later (not this slice): splitHand, hitHand, etc. — stubs only if needed
```

### PlayerRecord[] (heap + binary)
```cpp
PlayerRecord *newRecords(int n);
void  destroyRecords(PlayerRecord *r);
bool  saveRecords(fstream &out, const PlayerRecord *r, int n);
bool  loadRecords(fstream &in, PlayerRecord *r, int n);
```

### main teardown checklist (Ch9)
```cpp
// destroyPlayer(...);
// destroyDealer(...);   // if heap
// destroyDeck(...);
// destroyRecords(...);
// Exit stage right
```

---

## 3. Rough draft — binary save/load skeleton (Ch12)

Beginner-friendly, professor style, **TODOs only** — not production I/O.

```cpp
/*
 * File:   BinaryIODraft.cpp
 * Author: Abraham Murillo
 * Purpose: Rough draft — ios::binary save/load for GameConfig + PlayerRecord
 *          Outline / TODOs only; not a finished game
 */

//System Libraries
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

//User Libraries
// #include "GameConfig.h"
// #include "PlayerRecord.h"

//Global Constants Only
//const float BJ_PAY = 1.5f;

//Function Prototypes
bool saveConfig(fstream &out, const GameConfig &cfg);
bool loadConfig(fstream &in, GameConfig &cfg);
bool saveRecords(fstream &out, const PlayerRecord *r, int n);
bool loadRecords(fstream &in, PlayerRecord *r, int n);

//Execution begins here
int main(int argc, char** argv) {
    //Declare
    GameConfig cfg;                 // plain struct on stack
    // TODO: setConfig(cfg, /*nDecks*/);

    int nRec = 2;
    PlayerRecord *recs = new PlayerRecord[nRec];  // main owns
    // TODO: fill recs[i] name / bankroll (C-string + floats)

    //Open binary files (Ch12)
    fstream cfgFile;
    fstream recFile;
    cfgFile.open("config.bin", ios::in|ios::out|ios::binary);
    recFile.open("players.bin", ios::in|ios::out|ios::binary);
    // TODO: test open failed → print and exit

    //Write / read round-trip (stubs)
    // TODO: saveConfig(cfgFile, cfg);
    // TODO: loadConfig(cfgFile, cfg);
    // TODO: saveRecords(recFile, recs, nRec);
    // TODO: loadRecords(recFile, recs, nRec);

    //Recover memory — main owns every pointer
    delete [] recs;
    cfgFile.close();
    recFile.close();

    //Exit stage right
    return 0;
}

bool saveConfig(fstream &out, const GameConfig &cfg) {
    // TODO: out.write(reinterpret_cast<const char*>(&cfg), sizeof(GameConfig));
    // Works best when GameConfig has NO pointers inside
    return true;
}

bool loadConfig(fstream &in, GameConfig &cfg) {
    // TODO: in.read(reinterpret_cast<char*>(&cfg), sizeof(GameConfig));
    return true;
}

bool saveRecords(fstream &out, const PlayerRecord *r, int n) {
    // TODO: optionally write n first, then the array
    // TODO: out.write(reinterpret_cast<const char*>(r), n * sizeof(PlayerRecord));
    return true;
}

bool loadRecords(fstream &in, PlayerRecord *r, int n) {
    // TODO: in.read(reinterpret_cast<char*>(r), n * sizeof(PlayerRecord));
    // Caller must already new[] enough rows (main owns)
    return true;
}
```

**Notes for Abraham**
- Pass `fstream` **by reference** (Ch12 slides).
- Prefer fixed-size `char name[32]` inside `PlayerRecord` so the whole struct is binary-safe.
- If a struct holds pointers (`Deck`, `Hand`), write fields one-by-one (size, then data) like `Dynamic_Fixed_Size.cpp` — do **not** `.write` the pointer address.

---

## 4. Suggested `main` growth (next coding step — still outline)

```cpp
int main(int argc, char** argv) {
    // TODO: keep a short demo of face/suit/value OR call fillDeck that uses them
    GameConfig cfg;
    Deck *deck = nullptr;
    // deck = newDeck(cfg.nDecks);
    // fillDeck(deck); shuffleDeck(deck);
    // ... later: Player / Dealer / records ...
    // destroyDeck(deck);
    return 0; //Exit stage right
}
```

## 5. Test checklist (when bodies exist)

- [ ] `face`/`suit`/`value` still match v1 for indexes 0..51
- [ ] `destroyDeck` frees `cards` then `Deck`
- [ ] Player split rows each free their own `Card` array
- [ ] `config.bin` / `players.bin` round-trip without leaking

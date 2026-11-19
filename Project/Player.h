/* 
 * File:   Player.h
 * Author: Abraham Murillo
 * Purpose: Player — Hand array on heap (row 0 original, later = splits)
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

#include "Hand.h"

struct Player{
    char   name[32];      // C-string (Ch10)
    string displayName;   // std::string (Ch10)
    float  bankroll;
    Hand  *hands;         // new Hand[maxHands]; row 0 = first hand
    int    nHands;
    int    maxHands;
    int    wins;          // settled-hand wins this session
    int    losses;        // settled-hand losses this session
    int    games;         // settled hands (includes pushes)
};

//Function Prototypes — bodies in main.cpp
Player *newPlayer(const char name[], int maxHands, int maxCards, float bank);
void    prntPlayer(const Player *p);
void    destroyPlayer(Player *p);

#endif /* PLAYER_H */

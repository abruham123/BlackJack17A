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
    // TODO: char name[32];       // C-string practice
    // TODO: string displayName;  // std::string practice
    // TODO: float bankroll;
    // TODO: Hand *hands;         // new Hand[maxHands]
    // TODO: int nHands;
    // TODO: int maxHands;
};

//Function Prototypes — Abraham writes the bodies
Player *newPlayer(int maxHands, int maxCards);
void    destroyPlayer(Player *p);

#endif /* PLAYER_H */

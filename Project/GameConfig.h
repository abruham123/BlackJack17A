/* 
 * File:   GameConfig.h
 * Author: Abraham Murillo
 * Purpose: Plain game settings — also saved with ios::binary
 */

#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <fstream>
using namespace std;

// Prefer NO pointers inside so sizeof write/read is easy
struct GameConfig{
    // TODO: int nDecks;
    // TODO: bool hitSoft17;
    // TODO: float bjPay;   // 3:2 → 1.5f
};

//Function Prototypes — Abraham writes the bodies
void setConfig(GameConfig &cfg, int nDecks);
bool saveConfig(fstream &out, const GameConfig &cfg);
bool loadConfig(fstream &in, GameConfig &cfg);

#endif /* GAMECONFIG_H */

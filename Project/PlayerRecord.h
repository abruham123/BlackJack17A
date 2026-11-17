/* 
 * File:   PlayerRecord.h
 * Author: Abraham Murillo
 * Purpose: One binary save row — array on heap + players.bin
 */

#ifndef PLAYERRECORD_H
#define PLAYERRECORD_H

#include <fstream>
using namespace std;

// Prefer fixed char[] (no string/pointers) for simple binary I/O
struct PlayerRecord{
    // TODO: char name[32];
    // TODO: float bankroll;
    // TODO: int games;
    // TODO: int wins;
    // TODO: int losses;
};

//Function Prototypes — Abraham writes the bodies
PlayerRecord *newRecords(int n);
void destroyRecords(PlayerRecord *r);
bool saveRecords(fstream &out, const PlayerRecord *r, int n);
bool loadRecords(fstream &in, PlayerRecord *r, int n);

#endif /* PLAYERRECORD_H */

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
    char  name[32];
    float bankroll;
    int   games;
    int   wins;
    int   losses;
};

//Function Prototypes — bodies in main.cpp
PlayerRecord *newRecords(int n);
void destroyRecords(PlayerRecord *r);
bool saveRecords(fstream &out, const PlayerRecord *r, int n);   //3 arguments and one of them is a file
bool loadRecords(fstream &in, PlayerRecord *r, int n);          //3 Arguments, one of them is a file
bool readRecordAt(fstream &in, PlayerRecord &rec, int index);  // random access
void prntRecord(const PlayerRecord &rec);                      //Passing in one const

#endif /* PLAYERRECORD_H */

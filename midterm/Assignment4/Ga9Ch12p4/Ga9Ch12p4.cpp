/*
 * File:   Ga9Ch12p4.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 12 - Programming Challenge 4
 * Tail Program
 *
 * Asks for a file name and displays the last 10 lines (the "tail").
 * If the file has fewer than 10 lines, displays the entire file and
 * prints a message saying so.
 *
 * Uses seekg() to rewind to the start of the file for a second pass
 * (same chapter idea as random-access file positioning).
 */

//System Libraries Here
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
const int TAIL_LINES = 10;   // How many lines to show from the end

//Function Prototypes Here
int countLines(fstream &);              // Count lines in the file
void showTail(fstream &, int);          // Print the last TAIL_LINES (or all)

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare Variables
    string fileName;    // Name of the file to display
    fstream inFile;     // Input file stream
    int totalLines;     // Total number of lines in the file

    //Ask for the file name and open it
    cout << "Enter the file name: ";
    getline(cin, fileName);
    inFile.open(fileName.c_str(), ios::in);

    if (inFile.fail()) {
        cout << "Error: could not open " << fileName << endl;
        return 1;
    }

    //First pass: count how many lines are in the file
    totalLines = countLines(inFile);

    //seekg back to the beginning so we can read again
    inFile.clear();                      // Clear eofbit from the first pass
    inFile.seekg(0L, ios::beg);          // Move get pointer to start of file

    //Second pass: display the tail (or the whole file)
    showTail(inFile, totalLines);

    //Close the file
    inFile.close();

    //Exit
    return 0;
}

//Read every line once and return how many lines were found
int countLines(fstream &inFile) {
    string line;
    int count = 0;

    while (getline(inFile, line)) {
        count++;
    }
    return count;
}

//If totalLines < 10, print the whole file and a message.
//Otherwise skip the early lines and print only the last 10.
void showTail(fstream &inFile, int totalLines) {
    string line;
    int skip;   // How many lines to skip before printing

    if (totalLines < TAIL_LINES) {
        //Fewer than 10 lines: show everything
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        cout << endl;
        cout << "The entire file has been displayed." << endl;
    } else {
        //Skip all but the last 10 lines
        skip = totalLines - TAIL_LINES;
        for (int i = 0; i < skip; i++) {
            getline(inFile, line);
        }

        //Print the remaining (last) 10 lines
        while (getline(inFile, line)) {
            cout << line << endl;
        }
    }
}

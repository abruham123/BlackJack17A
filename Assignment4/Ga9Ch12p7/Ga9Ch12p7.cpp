/*
 * File:   Ga9Ch12p7.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 12 - Programming Challenge 7
 * Sentence Filter
 *
 * Asks for two file names. Reads sentences ending with a period from the
 * input file, capitalizes the first letter of each sentence, lowercases all
 * other letters, and writes the result to the output file.
 */

//System Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Physics/Math/Conversions found here

//Function Prototypes
void filterSentences(ifstream &, ofstream &);

//Execution Begins Here
int main() {
    //Declare Variables
    string inName;      // Input file name
    string outName;     // Output file name
    ifstream inFile;    // Input file stream
    ofstream outFile;   // Output file stream

    //Get the input file name and open it
    cout << "Enter the name of the input file: ";
    getline(cin, inName);
    inFile.open(inName.c_str());
    if (inFile.fail()) {
        cout << "Error: could not open " << inName << endl;
        return 1;
    }

    //Get the output file name and open it
    cout << "Enter the name of the output file: ";
    getline(cin, outName);
    outFile.open(outName.c_str());
    if (outFile.fail()) {
        cout << "Error: could not open " << outName << endl;
        inFile.close();
        return 1;
    }

    //Process the file: uppercase sentence starts, lowercase elsewhere
    filterSentences(inFile, outFile);

    //Close files
    inFile.close();
    outFile.close();

    cout << "Filtered text written to " << outName << endl;

    //Exit stage right!
    return 0;
}

//Copy inFile to outFile, uppercasing the first letter of each sentence
//and lowercasing all other letters. Sentences are assumed to end with '.'.
void filterSentences(ifstream &inFile, ofstream &outFile) {
    char ch;                    // Current character
    bool newSentence = true;    // Next letter starts a new sentence

    while (inFile.get(ch)) {
        if (isalpha(static_cast<unsigned char>(ch))) {
            if (newSentence) {
                outFile.put(static_cast<char>(
                    toupper(static_cast<unsigned char>(ch))));
                newSentence = false;
            } else {
                outFile.put(static_cast<char>(
                    tolower(static_cast<unsigned char>(ch))));
            }
        } else {
            outFile.put(ch);
            if (ch == '.') {
                newSentence = true;
            }
        }
    }
}

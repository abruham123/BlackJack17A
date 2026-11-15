/*
 * File:   Ga9Ch12p6.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 12 - Programming Challenge 6
 * String Search
 *
 * Asks for a file name and a search string. Reads the file line by line,
 * prints every line that contains the string, and reports how many times
 * the string appears in the file.
 *
 * Outline follows ClassReview/ReadWriteStruct2BinFiles.cpp style
 * (prototypes + helper functions). File reading uses text mode / getline,
 * similar to Ga9Ch12p7.cpp, because this is a text-search problem.
 */

//System Libraries Here
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
//Like PI, e, Gravity, or conversions

//Function Prototypes Here
int searchFile(fstream &, const string &);   // Search; print matching lines; return count

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare Variables
    string fileName;    // Name of the file 
    string searchStr;   // String the user 
    fstream inFile;     // Input file stream
    int count;          // How many times searchStr was found

    cout << "Enter the file name: ";
    getline(cin, fileName);
    inFile.open(fileName.c_str(), ios::in);     //Data will be read from the file 

    if (inFile.fail()) {
        cout << "Error: could not open " << fileName << endl;
        return 1;
    }

    //Ask for the string to search for
    cout << "Enter the string to search for: ";
    getline(cin, searchStr);

    //Search the file: print matching lines and count occurrences
    count = searchFile(inFile, searchStr);

    //Report the total number of times the string appeared
    cout << endl;
    cout << "The string \"" << searchStr << "\" was found "
         << count << " time(s) in the file." << endl;

    //Close the file
    inFile.close();

    //Exit
    return 0;
}

//Read the file one line at a time with getline.
//For each line, use string::find to locate every occurrence of searchStr.
//Whenever it is found, print that entire line and add 1 to the count.
//Returns the total number of occurrences in the file.
int searchFile(fstream &inFile, const string &searchStr) {
    string line;                        // Current line from the file
    string::size_type pos;              // Position where searchStr was found
    int count = 0;                      // Running total of matches

    while (getline(inFile, line)) {
        
        //Start looking from the beginning of this line
        pos = line.find(searchStr);

        //Keep searching the same line in case the string appears more than once
        while (pos != string::npos) {
            
            //Display the whole line that contains the match
            cout << line << endl;
            count++;

            //Look for the next occurrence after this one
            pos = line.find(searchStr, pos + 1);
        }
    }

    return count;
}

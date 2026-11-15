/*
 * File:   Ga9Ch12p8.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 12 - Programming Challenge 8
 * Array/File Functions
 *
 * Demonstrates writing an integer array to a binary file and reading it
 * back into another array using arrayToFile and fileToArray.
 */

//System Libraries
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Physics/Math/Conversions found here

//Function Prototypes
void arrayToFile(string, int *, int);   // Write array to a binary file
void fileToArray(string, int *, int);   // Read binary file into an array
void displayArray(const int *, int);    // Print array contents to the screen

//Execution Begins Here
int main() {
   
    //Declare Variables

    // SIZE is how many integers we store; the binary file holds SIZE * sizeof(int) bytes
    const int SIZE = 10;
    int numbers[SIZE] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; // Source array
    int loaded[SIZE] = {0};                                        // Destination array (starts empty / zeroed)
    string fileName = "numbers.bin";                               // Binary file used 

    //Original array before writing
    cout << "Original array:" << endl;
    displayArray(numbers, SIZE);

    //Write the entire integer array to the binary file
    //ios::binary tells the stream not to translate newlines or other text formatting
    arrayToFile(fileName, numbers, SIZE);   //The 3 arguments
    cout << "Array written to " << fileName << endl;

    //Read that same binary file back into a different array
    fileToArray(fileName, loaded, SIZE);    //The 3 arguments
    cout << "Array read back from " << fileName << endl;

    //Display what was loaded to verify the write/read worked
    cout << "Array after reading from file:" << endl;
    displayArray(loaded, SIZE);

    return 0;
}


//Writes size integers from arrayArr,
//then closes the file.
//write() expects a char* and a byte count. reinterpret_cast converts the
//int* to char* so the raw bytes of the ints are stored (not text like "10 20").
void arrayToFile(string fileName, int *arrayArr, int size) {
    fstream outFile;    //Output mode; Data will be written to a file (opens file in binary)

    //ios::out  = write mode (create)
    //ios::binary = store exact memory bytes
    outFile.open(fileName.c_str(), ios::out | ios::binary);

    if (outFile.fail()) {
        cout << "Error: could not open " << fileName << " for writing." << endl;
        return;
    }

    //Write size integers: total bytes = size * sizeof(int)
    outFile.write(reinterpret_cast<char *>(arrayArr),
                  size * static_cast<streamsize>(sizeof(int)));

    outFile.close();
}


//Fills arrayArr with size integers
//from the file, then closes the file.
//
//read() mirrors write(): same cast and same byte count so the ints
//are reconstructed exactly as they were stored.
void fileToArray(string fileName, int *arrayArr, int size) {
    fstream inFile;         //Input mode; Data will be read from a file (open file in binary)

    //ios::in   = read mode
    //ios::binary = read raw bytes the same way they were written
    inFile.open(fileName.c_str(), ios::in | ios::binary);

    if (inFile.fail()) {
        cout << "Error: could not open " << fileName << " for reading." << endl;
        return;
    }

    //Read size integers into the array pointed to by arrayArr
    inFile.read(reinterpret_cast<char *>(arrayArr),
                size * static_cast<streamsize>(sizeof(int)));

    inFile.close();
}

//Prints each element of the array on one line
void displayArray(const int *arrayArr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arrayArr[i] << " ";     //Seperated by spaces
    }
    cout << endl;
}

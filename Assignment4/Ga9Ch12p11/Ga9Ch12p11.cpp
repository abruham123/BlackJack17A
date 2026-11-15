/*
 * File:   Ga9Ch12p11.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 12 - Programming Challenge 11
 * Corporate Sales Data Output
 *
 * Based on Class/Structures_To_Files/0_ReadWriteStructureToBinaryFile
 * (fixed-size structure read/write). Replaces the random Test data with
 * East/West/North/South quarterly sales from the user.
 *
 * Input Validation: Do not accept negative numbers for any sales figures.
 */

//System Libraries Here
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
using namespace std;

//User Libraries Here
//Fixed-size record: name is a char array so sizeof(DivSales) is constant
struct DivSales {
    char name[10];   // Division Name (East, West, North, South)
    int quarter;     // Quarter (1, 2, 3, or 4)
    float sales;     // Quarterly Sales
};

//Global Constants Only, No Global Variables
//Like PI, e, Gravity, or conversions

//Function Prototypes Here
//Whole structure can be written/read with write() and read()
void wrtTxt(fstream &, DivSales *, int);
void wrtBin(fstream &, DivSales *);
DivSales *readBin(fstream &, int);
void prntTxt(DivSales *);
float getSales();   // Reads sales; rejects negatives

//Program Execution Begins Here
int main(int argc, char** argv) {

    
    //  ios::in|ios::out so we can write then seek/read on the same stream
    //  ios::trunc creates/clears the file each run
    //  ios::binary for the .bin file
    fstream txtFile, binFile;
    txtFile.open("data.txt", ios::in | ios::out | ios::trunc);
    binFile.open("data.bin", ios::in | ios::out | ios::binary | ios::trunc);

    //Declare all Variables Here and Loop
    //4 divisions x 4 quarters = 16 fixed-size records
    const int NUM_DIVS = 4;
    const char *divNames[NUM_DIVS] = {"East", "West", "North", "South"};
    int nRecords = 0;   // How many records written so far

    //Fill a binary and text file with DivSales structures from the user
    for (int d = 0; d < NUM_DIVS; d++) {
        cout << "Enter four quarters' sales for the "
             << divNames[d] << " division" << endl;

        for (int q = 1; q <= 4; q++) {
            //Create the Structure and Fill it
            DivSales *sale = new DivSales;
            strncpy(sale->name, divNames[d], 9);
            sale->name[9] = '\0';
            sale->quarter = q;

            cout << "Quarter " << q << " sales: ";
            sale->sales = getSales();

            //Write to binary and text files
            wrtTxt(txtFile, sale, nRecords);
            wrtBin(binFile, sale);
            delete sale;

            nRecords++;
        }
    }

    //Read back in one record, then print out (same demo as the class example)
    int randRec = 0;   // First record: East, Quarter 1
    cout << "Random record to find = " << randRec << endl;
    DivSales *sale = readBin(binFile, randRec);
    prntTxt(sale);
    cout << "The size of the DivSales structure = "
         << sizeof(DivSales) << " bytes" << endl;

    //Delete the array
    delete sale;

    //Close the files
    txtFile.close();
    binFile.close();

    //Exit
    return 0;
}

//Seek to record * sizeof(DivSales), then read that many bytes into a new struct
DivSales *readBin(fstream &in, int record) {
    //Declare and initialize variables
    long cursor = 0L;           //Where to place the cursor
    DivSales *a = new DivSales; //Declare the structure to return

    //Find the record (fixed size => multiply index by sizeof)
    cursor = record * sizeof(DivSales);

    //Fill the structure
    in.seekg(cursor, ios::beg);
    in.read(reinterpret_cast<char *>(a), sizeof(DivSales));
    return a;
}

//Write the entire fixed-size structure as raw bytes
void wrtBin(fstream &out, DivSales *a) {
    out.write(reinterpret_cast<char *>(a), sizeof(DivSales));
}

//Write the same record in readable text form
void wrtTxt(fstream &out, DivSales *a, int record) {
    out << endl;
    out << "Structure Contents Record = " << record << endl;
    out << setw(10) << a->name << endl;
    out << setw(10) << a->quarter << endl;
    out << fixed << setprecision(2);
    out << setw(10) << a->sales << endl;
    out << endl;
}

//Print one structure to the screen
void prntTxt(DivSales *a) {
    cout << endl;
    cout << "Structure Contents " << endl;
    cout << setw(10) << a->name << endl;
    cout << setw(10) << a->quarter << endl;
    cout << fixed << setprecision(2);
    cout << setw(10) << a->sales << endl;
    cout << endl;
}

//Input Validation: do not accept negative sales
float getSales() {
    float amount;
    cin >> amount;
    while (amount < 0) {
        cout << "Sales cannot be negative. Re-enter: ";
        cin >> amount;
    }
    return amount;
}

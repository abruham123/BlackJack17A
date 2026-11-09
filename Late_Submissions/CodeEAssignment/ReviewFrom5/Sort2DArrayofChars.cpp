/* 
 * File:   main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose:  Sorting a 2-D array of characters if row and columns match
 */

//System Libraries Here
#include <iostream>//cin,cout
#include <cstring> //strlen(),strcmp(),strcpy()
using namespace std;

//User Libraries Here
//Global Constants Only, No Global Variables
//Allowed like PI, e, Gravity, conversions, array dimensions necessary
const int COLMAX=80;//Only 20 required, and 1 for null terminator

//Function Prototypes Here
int  read(char [][COLMAX],int &);           //Outputs row and columns detected from input
void sort(char [][COLMAX],int,int);         //Sort by row
void print(const char [][COLMAX],int,int);  //Print the sorted 2-D array

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare all Variables Here
    const int ROW=30;             //Only 20 required
    char array[ROW][COLMAX];      //Bigger than necessary (2D bc row and coloumn)
    int colIn,colDet,rowIn,rowDet;//Row, Col input and detected from the user
    
    //Input the size of the array you are sorting
    cout<<"Read in a 2 dimensional array of characters and sort by Row"<<endl;
    cout<<"Input the number of rows <= 20"<<endl;
    cin>>rowIn;
    cout<<"Input the maximum number of columns <=20"<<endl;
    cin>>colIn;
    
    //Now read in the array of characters and determine it's size
    rowDet=rowIn;
    cout<<"Now input the array."<<endl;
    colDet=read(array,rowDet);  //Read the array and fills into rowDet
    
    //Compare the size input vs. size detected and sort if same
    //Else output different size
    if(rowDet==rowIn&&colDet==colIn){
        sort(array,rowIn,colIn);
        cout<<"The Sorted Array"<<endl;
        print(array,rowIn,colIn);
    }else{
        if(rowDet!=rowIn)
        cout<<(rowDet<rowIn?"Row Input size less than specified.":
            "Row Input size greater than specified.")<<endl;
        if(colDet!=colIn)
        cout<<(colDet<colIn?"Column Input size less than specified.":
            "Column Input size greater than specified.")<<endl;
    }
    
    //Exit
    return 0;
}
//read: store one word in each row. Return the longest length.
//rowDet is a reference, so assigning it changes rowDet in main.
int read(char array[][COLMAX],int &rowDet){
    //Start at 0. rowDet currently holds the claimed rowIn (for example 3).
    //The first word has to go in row 0, not row 3.
    int rowsRead=0;
    int longest=0;

    //cin >> array[rowsRead] stores one word and writes the '\0' after it.
    //The loop condition is the read itself: true for each word, false when input ends.
    while (cin>>array[rowsRead]){
        int length=strlen(array[rowsRead]);        //counts letters before '\0', not the '\0'
        if (length>longest) longest=length;          //detected columns = longest word
        rowsRead++;                                  //next word goes in the next row
    }

    rowDet=rowsRead; //real number of words read, written back to main
    return longest;  //main stores this return value in colDet
}

//sort: bubble sort, same neighbor passes as the 1D program.
//array[j] is a whole row (the word). array[j][0] would be one character.
//colDet is required by the prototype. strcmp already stops at '\0'.
void sort (char array[][COLMAX], int rowDet, int colDet){
    for (int i=0; i<rowDet-1; i++)              //one pass per row
        for (int j=0; j<rowDet-1; j++)          //compare row j with the next row
            if (strcmp(array[j],array[j+1])>0) //>0 means row j belongs after row j+1
                swap(array[j],array[j+1]);      //exchange every character in the two rows
}

//print: const means this function reads the array and does not change it.
//cout << array[i] prints that row until its '\0', then endl starts the next row.
void print(const char array[][COLMAX], int rowDet, int colDet){
    for (int i=0; i<rowDet; i++){
        cout<<array[i]<<endl;
    }
}
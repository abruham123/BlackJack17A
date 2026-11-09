/* 
 * File:   main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose:  Sorting an array of characters if specified correctly
 */

//System Libraries Here
#include <iostream>//cout,cin
#include <cstring> //strlen()
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
//Like PI, e, Gravity, or conversions

//Function Prototypes Here
int  read(char []);         //Read Cstring and return how many chars were stored
void sort(char [],int);    //Sort the array in ascending order
void print(const char [],int); //Print that many chars on one line

//Program Execution Begins Here
int main(int argc, char** argv) 
{
    //Declare all Variables Here
    const int SIZE=80;      //Larger than needed
    char array[SIZE];       //Character array larger than needed
    int sizeIn,sizeDet;     //Number of characters to be read, check against length
    
    //Input the size of the array you are sorting
    cout<<"Read in a 1 dimensional array of characters and sort"<<endl;
    cout<<"Input the array size where size <= 20"<<endl;
    cin>>sizeIn;
    
    //Now read in the array of characters and determine it's size
    cout<<"Now read the Array"<<endl;
    
    sizeDet=read(array);//Determine it's size
    
    //Compare the size input vs. size detected and sort if same
   
    //Else output different size to sort
    if(sizeDet==sizeIn){
        sort(array,sizeIn);         //Sort the array
        print(array,sizeIn);        //Print the array
    }
    else
    {
        cout<<(sizeDet<sizeIn?"Input size less than specified.":
            "Input size greater than specified.")<<endl;
    }
    
    
    //Exit
    return 0;
}
//Read: Input the character array, then return detected length
int read(char array[]) {
    //Input the character array:Read characters as one cstring
    cin>>array;
    //Return the detected length
    return strlen(array);

}
//Sort: Bublle sort, each pass compares neighbors and swaps any pair that it out of order
void sort(char array[],int size){
    //Process
    for (int i=0; i<size-1; i++)//Outer:Pass one for each position
        for (int j=0; j<size-1; j++)//Inner: Compares adjacent elements
            if (array[j]>array[j+1])//If out of order, swap
    swap(array[j],array[j+1]);//Swap the pair
}
//Print: Print that many chars on one line
void print(const char array[],int size){
    for (int i=0; i<size; i++)
    cout<<array[i]; //Print each character of the array
    cout<<endl;

}
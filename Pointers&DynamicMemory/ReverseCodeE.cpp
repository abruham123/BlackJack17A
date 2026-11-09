/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 4th, 2026, 8:21 PM
 * Purpose:  Reverse a dynamic array
 */

//Libraries
#include <iostream>//I/O Library
using namespace std;

//No Global Constants

//Function Prototypes
int *getData(int &);        //Fill the array
int *sort(const int *,int); //Sort smallest to largest
int *reverse(const int *,int); //Sort in reverse order
void prntDat(const int *,int); //Print the array

//Execution begins here
int main(int argc, char*argv[]) {
    //Declare variables and fill
    int size;       //Size of the integer array
    int *array;     //Pointer to the integer array
    
    //Read the size and the integer data
    array=getData(size);
    
    //Sort a copy from smallest to largest, then print it
    int *sorted=sort(array,size);
    prntDat(sorted,size);
    
    //Reverse that sorted array and print the largest-to-smallest result
    int *rev=reverse(sorted,size);
    prntDat(rev,size);
    
    //Delete allocated memory
    delete []array;
    delete []sorted;
    delete []rev;
    
    //Exit stage right
    return 0;
}

int *getData(int &n){
    //First input is the size. That size decides the allocation.
    cin>>n;
    int *array=new int[n];
    for(int i=0;i<n;i++){
        cin>>array[i];
    }
    return array;
}

int *sort(const int *array,int n){
    //Copy so the array that was read stays in its original order
    int *copy=new int[n];
    for(int i=0;i<n;i++){
        copy[i]=array[i];
    }
    //Smallest to largest
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if(copy[j]<copy[i]){
                int temp=copy[i];
                copy[i]=copy[j];
                copy[j]=temp;
            }
        }
    }
    return copy;
}

int *reverse(const int *array,int n){
    //Copy, then swap the ends the way the digit reverse does
    int *rev=new int[n];
    for(int i=0;i<n;i++){
        rev[i]=array[i];
    }
    for(int i=0;i<n/2;i++){
        int temp=rev[i];
        rev[i]=rev[n-1-i];
        rev[n-1-i]=temp;
    }
    return rev;
}

void prntDat(const int *array,int n){
    for(int i=0;i<n;i++){
        cout<<array[i];
        if(i<n-1)cout<<" ";
    }
}

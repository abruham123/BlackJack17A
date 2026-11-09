/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 4th, 2026, 8:36 PM
 * Purpose:  Successive sum of a dynamic array
 */

//Libraries
#include <iostream>//I/O Library
using namespace std;

//No Global Constants

//Function Prototypes
int *getData(int &);        //Return the array size and the array from the inputs
int *sumAry(const int *,int);//Return the array with successive sums
void prntAry(const int *,int);//Print the array

//Execution begins here
int main(int argc, char*argv[]) {
    //Declare variables and fill
    int size;   //Size of the integer array
    int *array; //Pointer to the integer array
    
    //Read the size and the integer data
    array=getData(size);
    
    //Print the original array
    prntAry(array,size);
    cout<<endl;
    
    //Parallel array: each spot is the sum of every member up through that spot
    int *sums=sumAry(array,size);
    prntAry(sums,size);
    
    //Delete allocated memory
    delete []array;
    delete []sums;
    
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

int *sumAry(const int *array,int n){
    //New array, same size. Do not change the array that was read.
    int *sum=new int[n];
    if(n>0)sum[0]=array[0];
    for(int i=1;i<n;i++){
        sum[i]=sum[i-1]+array[i];
    }
    return sum;
}

void prntAry(const int *array,int n){
    for(int i=0;i<n;i++){
        cout<<array[i];
        if(i<n-1)cout<<" ";
    }
}

/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 4th, 2026, 8:47 PM
 * Purpose:  Augment a 1-D array by 1 element
 */

//Libraries
#include <iostream>//I/O Library
using namespace std;

//No Global Constants

//Function Prototypes
//getData: position 1 is main's size. The & lets this function write that size.
//The int * in front is the return type: the address of the new array.
int *getData(int &);            //Read the array
//augment: position 1 is the original array, position 2 is its size.
//const means this function may read those slots but may not change them.
int *augment(const int *,int);  //Augment and copy the original array
//prntAry: position 1 is whichever array to print, position 2 is how many slots.
void prntAry(const int *,int);  //Print the array

//Execution begins here
int main(int argc, char*argv[]) {
    
    int size;       //How many integers were read. getData writes this.
    int *array;     //Address of the original block. * means pointer.
    
    //Position 1 of getData is size. The returned address is stored in array.
    array=getData(size);
    
    //Print the original. Position 1 is array, position 2 is size.
    //endl is here, not inside prntAry, so only this line ends with a newline.
    prntAry(array,size);
    cout<<endl;
    
    //New block is one element longer. Original stays where it is.
    //Position 1 is the original, position 2 is the old size.
    int *aug=augment(array,size);
    
    //The augmented block has size+1 slots, so position 2 is size+1.
    prntAry(aug,size+1);
    
    //delete [] frees the whole block that new[] created. One delete per new.
    delete []array;
    delete []aug;
    
    //Exit stage right
    return 0;
}

//Position 1, n, is a reference to main's size. Writing n writes main's size.
int *getData(int &n){
    //First number typed is the size. cin stores it in n, which is main's size.
    cin>>n;
    //new int[n] asks for n integers in a row and returns the address of slot 0.
    int *array=new int[n];
    //i is the index. array[i] is slot i. The loop stops before i reaches n.
    for(int i=0;i<n;i++){
        cin>>array[i];
    }
    //Hand the address of slot 0 back to main. The block stays alive after return.
    return array;
}

//Position 1 is read-only. Position 2, n, is a copy of the original size.
int *augment(const int *array,int n){
    //One extra slot. Indexes of this new block run from 0 through n.
    int *aug=new int[n+1];
    //The new front slot is 0. Nothing from the original is stored at index 0.
    aug[0]=0;
    //Original slot i moves to augmented slot i+1. That is the shift by 1 index.
    //i=0 copies array[0] into aug[1]. The last copy is array[n-1] into aug[n].
    for(int i=0;i<n;i++){
        aug[i+1]=array[i];
    }
    //Return the address of the new block. Caller must delete [] it later.
    return aug;
}

//Position 1 is read-only. Position 2 is how many numbers to print, not the allocated size.
void prntAry(const int *array,int n){
    for(int i=0;i<n;i++){
        cout<<array[i];
        //A space follows every number except the last one in this line.
        if(i<n-1)cout<<" ";
    }
}

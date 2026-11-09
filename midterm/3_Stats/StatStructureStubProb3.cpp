/* 
 * File:   main.cpp
 * Author: Dr. Mark E. Lehr
 * Purpose:  Driver program to test out the statistics problem.
 */

//Libraries
#include <iostream>//I/O
using namespace std;

//User Libraries
#include "Array.h"
#include "Stats.h"

//No Global Constants

//Function Prototypes I supply
Array *fillAry(int,int);        //Fill an array and put into a structure
void prntAry(const Array *,int);//Print the array 
int *copy(const int *,int);     //Copy the array
void mrkSort(int *,int);        //Sort an array
void prtStat(const Stats *);    //Print the structure
void rcvrMem(Array *);          //Recover memory from the Array Structure
void rcvrMem(Stats *);          //Recover memory from Statistics Structure

//Functions you are to supply
Stats *stat(const Array *);     //Find & store mean, median, & modes in structure

//Execution begins here
int main(int argc, char*argv[]) {
    //Declare variables
    int arySize;//Array Size
    int modNum; //Number to control the modes (digits 0 to 9 allowed)
    Array *array;
    
    //Input the size and mod number
    cout<<"This program develops an array to be analyzed"<<endl;
    cout<<"Array size from mod number to 100"<<endl;
    cout<<"Mod number from 2 to 10"<<endl;
    cout<<"Input the Array Size and the mod number to be used."<<endl;
    cin>>arySize>>modNum;
    cout<<endl<<endl;
    
    //Fill the array
    array=fillAry(arySize,modNum);
    
    //Print the initial array or the unsorted data (10 perline)
    cout<<"Original Array before sorting"<<endl;
    prntAry(array,10);
    
    //Sort the int array in place
    mrkSort(array->data,array->size);       //equal values sit nect to each other
    cout<<"Sorted Array to be utilize for the stat function"<<endl;
    prntAry(array,10);      //Print again so you can check sort
    
    //My function that i write, modes into stat structure
    Stats *stats=stat(array);
    
    //Print the statistics, avg, mean, median...
    prtStat(stats);
    
    //Recover allocated memory
    //free heap memory
    rcvrMem(array);         //Delete [] data then Array
    rcvrMem(stats);         //free mode array, then delete stats
    
    //Exit stage right
    return 0;
}

int *copy(const int *a,int n){
    //Declare and allocate an array
    //that is a c
    int *b=new int[n];
    //Fill
    for(int i=0;i<n;i++){
        b[i]=a[i];
    }
    //Return the copy
    return b;
}

void prtStat(const Stats *ary){
    cout<<endl;
    cout<<"The average of the array = "<<ary->avg<<endl;
    cout<<"The median of the array  = "<<ary->median<<endl;
    cout<<"The number of modes      = "<<
            ary->mode->size<<endl;
    cout<<"The max Frequency        = "<<
            ary->modFreq<<endl;
    if(ary->mode->size==0){
        cout<<"The mode set             = {null}"<<endl;
        return;
    }
    cout<<"The mode set             = {";
    for(int i=0;i<ary->mode->size-1;i++){
        cout<<ary->mode->data[i]<<",";
    }
    cout<<ary->mode->data[ary->mode->size-1]<<"}"<<endl;
}

void mrkSort(int *array,int n){
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if(array[j]<array[i]){
                int temp=array[i];
                array[i]=array[j];
                array[j]=temp;
            }
        }
    }
}

void rcvrMem(Stats *stats){
    rcvrMem(stats->mode);
    delete stats;
}

void rcvrMem(Array *array){
    delete []array->data;
    delete array;
}

void prntAry(const Array *array,int perLine){
    //Output the array
    for(int i=0;i<array->size;i++){
        cout<<array->data[i]<<" ";
        if(i%perLine==(perLine-1))cout<<endl;
    }
    cout<<endl;
}

Array *fillAry(int n, int modNum){
    //Allocate memory
    Array *array=new Array;
    array->size=n;
    array->data=new int[array->size];
    
    //Fill the array with mod numbers
    for(int i=0;i<n;i++){
        array->data[i]=i%modNum;
    }
    
    //Exit function
    return array;
}

Stats *stat(const Array *array){        //Drive already sorted array->data
    Stats *stats = new Stats;   //One structure on the heap
    stats->mode = new Array;

    // --- 1) MEAN: sum / size -> stats->avg ---
    float sum=0;
    for (int i=0; i<array->size; i++){
        sum+= array->data[i];
    }
    stats->avg = sum/array->size;
    
    // 2. indexes: for size n, middles are n/2-1 and n/2 (0-based)
    int n = array->size;
    if (n%2==1){                            //odd: one middle element
        stats->median=array-> data [n/2];
    }else{                                  //even; average of 2 middle elemenets
        stats->median=(array->data[n/2-1]+array->data[n/2])/2.0f;
    }

    // --- 3) MODE FREQUENCY: longest run of equal values in sorted data ---
    // walk the array; count how many times each value repeats
    // stats->modFreq = highest count you find
    stats->modFreq=1;
    int count=1;
    for(int i=1; i<array->size; i++){
        if(array->data[i]==array->data[i-1]){
            count++;
            if(count>stats->modFreq){
                stats->modFreq=count;
            }
        }else{
            count=1;        //new value starts new run
        }
    }

    // 4) #ofMODES: count values whose run length == modFreq (if modFreq == 1 -> treat as NO mode nModes = 0, {null})
    int nModes = 0;
    if(stats->modFreq>1){
        count=1;
        for(int i=1; i<array->size; i++){
            if (array->data[i]== array->data[i-1]){
                count++;
            }else {
                if (count==stats->modFreq) nModes++;
                count =1;
            }
        }
        if(count==stats->modFreq) nModes++;         //last run needs to be checked
    }


    // --- 5) ALLOCATE mode array and FILL with those values ---
    stats->mode->size = nModes;
    if(nModes != 0){
        stats->mode->data = new int[nModes];
        int m=0;
        count=1;
        for(int i=1; i<array->size; i++){
            if(array->data[i]==array->data[i-1]){
                count++;
        }else{
            if(count==stats->modFreq){
                stats->mode->data[m]=array->data[i-1];
                m++;
            }
            count=1;
        }
        }
        if(count==stats->modFreq){
            stats->mode->data[m]=array->data[array->size-1];
        }
        
        // second pass: copy each value that appears modFreq times
    }else{
        stats->mode->data = nullptr;  // no modes; safe for delete []
    }

    return stats;
}
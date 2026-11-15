/* 
 * File:   main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose:  Sorting an array of characters if specified correctly
 */

//System Libraries Here
#include <iostream>//cout,cin
#include<cstdlib> //rand()
#include<ctime> //time()
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
//Like PI, e, Gravity, or conversions

//Function Prototypes Here
int *FillData(int);
int *FillIndx(int);
void printAry(int [], int, int);
void printAry(int [], int[], int, int);
void mrkSrt(int [], int[], int);
void cleanup(int *, int *);

//Program Execution Begins Here
int main(int argc, char** argv) 
{
    srand(static_cast<unsigned int>(time(0)));
    //Declare all Variables Here
     int size=80;      
    int *array, *indx;     //array is not an integer it is an address of an integer
    
    //Initialize Variables
    cout<<endl<<endl<<"Before Sorting"<<endl;
    array = FillData(size);
    indx = FillIndx(size);
    printAry(array,size,10);
    printAry(indx,size,10);
    printAry(array,indx,size,10);

    //Mapping Input to Output
    mrkSrt(array,indx,size);

    //Display Reults
    cout<<endl<<endl<<"After Sorting"<<endl;
    printAry(array,size,10);
    printAry(indx,size,10);
    printAry(array,indx,size,10);

    //Deallocate arrays
    cleanup(array, indx);

    //Exit Stage Right
    return 0;

}

void cleanup(int *array, int *indx){
    delete [] array;
    delete [] indx;
}

void mrkSrt(int a[], int indx[], int n){
    for (int i = 0; i < n; i++){
        for (int j=i+1; j<n; j++){
            if (a[indx[i]]>a[indx[j]]){
                int temp = indx[i];
                indx[i] = indx[j];
                indx[j] = temp;
            }
        }
    }
}


int *FillData(int n){
    int *array = new int[n];
    for (int i = 0; i < n; i++){
        array[i] = rand()%90+10;
    }
    return array;
}


//Comment for this function
int *FillIndx(int n){
    int *indx = new int[n];
    for (int i = 0; i < n; i++){
        indx[i] = i;
    }
    return indx;
}

void printAry(int a[], int n, int perLine){
    cout<<endl;
    for (int i=0; i<n; i++){
        if (a[i]<10) 
        cout<<" ";
        cout<<a[i]<<" ";
        if (i%perLine==(perLine-1)) 
        cout<<endl;
    }
    cout<<endl;
}
void printAry(int a[], int indx[], int n, int perLine){
   cout<<endl;
    for (int i=0; i<n; i++){
        cout<<a[indx[i]]<<" ";
        if (i%perLine==(perLine-1)) 
        cout<<endl;
    }
    cout<<endl;
}
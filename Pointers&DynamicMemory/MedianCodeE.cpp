/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 4th, 2026, 8:06 PM
 * Purpose:  Dynamic median
 */

//Libraries
#include <iostream>//I/O Library
#include <iomanip> //2 decimal places
using namespace std;

//No Global Constants

//Function Prototypes
int *getData(int &);        //Return the array size and the array
void prntDat(int *,int);    //Print the integer array
float *median(int *,int);   //Fill the median Array with the Float array size, the median, and the integer array data
void prntMed(float *);      //Print the median Array

//Execution begins here
int main(int argc, char*argv[]) {
    //Declare variables and fill
    int size;   //Size of the integer array
    int *array; //Pointer to the integer array
    
    //Read the size and the integer data
    array=getData(size);
    
    //Print the integer array
    prntDat(array,size);
    cout<<endl;
    
    //Build the median array: size, median, then the integer data
    float *medAry=median(array,size);
    
    //Print the median array with 2 decimal places
    prntMed(medAry);
    
    //Delete allocated memory
    delete []array;
    delete []medAry;
    
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

void prntDat(int *array,int n){
    for(int i=0;i<n;i++){
        cout<<array[i];
        if(i<n-1)cout<<" ";
    }
    cout<<endl;
}

float *median(int *array,int n){
    //Copy so the integer array keeps the order that was read
    int *copy=new int[n];
    for(int i=0;i<n;i++){
        copy[i]=array[i];
    }
    //Sort the copy. The middle of the sorted copy is the median.
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if(copy[j]<copy[i]){
                int temp=copy[i];
                copy[i]=copy[j];
                copy[j]=temp;
            }
        }
    }
    
    //Float array is 2 larger: [0] size, [1] median, [2...] integer data
    float *medAry=new float[n+2];
    medAry[0]=n+2;
    if(n%2==1){                         //odd: one middle element
        medAry[1]=copy[n/2];
    }else{                              //even: average of the 2 middle elements
        medAry[1]=(copy[n/2-1]+copy[n/2])/2.0f;
    }
    for(int i=0;i<n;i++){
        medAry[i+2]=array[i];
    }
    
    delete []copy;
    return medAry;
}

void prntMed(float *array){
    int n=array[0];     //The float array stores its own size in element 0
    cout<<n;            //Size prints as a whole number
    cout<<fixed<<setprecision(2);
    for(int i=1;i<n;i++){
        cout<<" "<<array[i];
    }
}

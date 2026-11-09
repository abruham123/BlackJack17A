/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on DATE AND TIME HERE
 * Purpose:  Summing, Finding the Max and Min of an integer array
 * Note:  Look at the output for the format for print
 */

//System Libraries Here
#include <iostream>//cin,cout
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables

//Function Prototypes Here= Each ohf them list the positions the call must fill from left to right
void read(int [],int);//Pos 1 array, pos2 how many integers to read
int  stat(const int [],int,int &,int &); //Pos 1 array, pos2 how many integers to read, pos3 max, pos4 min
void print(const int [],int,int,int,int); //Pos 1 array, pos2 how many integers to read, pos3 sum, pos4 max, pos5 min

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare all Variables Here
    const int SIZE=80;
    int array[SIZE];
    int sizeIn,sum,min,max;
    
//Input the size of the array you are sorting
cout<<"Read in a 1 dimensional array of integers find sum/min/max"<<endl;
cout<<"Input the array size where size <= 20"<<endl;

//The user types the size here. sizeIn belongs to main.
cin>>sizeIn;

//Pass main's array and main's sizeIn. Position 1, then position 2.
//The user types the integers inside read, into this same array.
read(array,sizeIn);

//Positions: array, sizeIn, max, min. The returned int is stored in sum.
sum=stat(array,sizeIn,max,min);

//Positions: array, sizeIn, sum, max, min.
print(array,sizeIn,sum,max,min);

    //Exit
    return 0;
}
//Position 1 shares main's array. Position 2 is a copy of the size, 5 in the sample.
void read(int array[], int sizeIn){
    for (int i=0; i<sizeIn; i++){
        cin>>array[i]; //this cin is where the user types 1, 10, 100, 1000, 10000
    }
}

//const array: read the slots, do not change them.
//&max and &min are main's variables. return sends the sum back to sum=stat(...)
int stat(const int array[], int sizeIn, int &max, int &min){
    int sum=0;
    max=array[0]; //writes main's max
    min=array[0]; //writes main's min
    for (int i=0; i<sizeIn; i++){
        sum+=array[i];
        if (array[i]>max) max=array[i];
        if (array[i]<min) min=array[i];
    }
    return sum;
}

//Copies of sum, max, and min arrive in this order because the call passes them in this order.
void print(const int array[], int sizeIn, int sum, int max, int min){
    for (int i=0; i<sizeIn; i++){
        cout<<"a["<<i<<"] = "<<array[i]<<endl;
    }
    cout<<"Min  = "<<min<<endl;
    cout<<"Max  = "<<max<<endl;
    cout<<"Sum  = "<<sum<<endl;
}
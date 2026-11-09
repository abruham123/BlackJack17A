/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 4th, 2026, 8:43 PM
 * Purpose:  Total the elements in a 2-D array
 */

//Libraries
#include <iostream>//I/O Library
using namespace std;

//No Global Constants

//Function Prototypes
int **getData(int &,int &);             //Return the 2-D array and its size.
void prntDat(const int* const *,int,int);//Print the 2-D Array
void destroy(int **,int,int);            //Deallocate memory
int sum(const int * const *, int,int);   //Return the Sum

//Execution begins here
int main(int argc, char*argv[]) {
    //Declare variables and fill
    int rows,cols;  //Size of the 2-D array
    int **array;    //Pointer to the rows
    
    //Read the size and the table
    array=getData(rows,cols);
    
    //Print the table, then the total on the next line
    prntDat(array,rows,cols);
    cout<<sum(array,rows,cols);
    
    //Delete allocated memory
    destroy(array,rows,cols);
    
    //Exit stage right
    return 0;
}

int **getData(int &rows,int &cols){
    //First two inputs are the row count and the column count
    cin>>rows>>cols;
    int **array=new int*[rows];
    for(int row=0;row<rows;row++){
        array[row]=new int[cols];
        for(int col=0;col<cols;col++){
            cin>>array[row][col];
        }
    }
    return array;
}

void prntDat(const int* const *array,int rows,int cols){
    for(int row=0;row<rows;row++){
        for(int col=0;col<cols;col++){
            cout<<array[row][col];
            if(col<cols-1)cout<<" ";
        }
        cout<<endl;
    }
}

void destroy(int **array,int rows,int cols){
    for(int row=0;row<rows;row++){
        delete []array[row];
    }
    delete []array;
}

int sum(const int * const *array,int rows,int cols){
    int total=0;
    for(int row=0;row<rows;row++){
        for(int col=0;col<cols;col++){
            total+=array[row][col];
        }
    }
    return total;
}

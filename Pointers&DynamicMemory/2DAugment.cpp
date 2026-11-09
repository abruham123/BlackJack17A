/* 
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on November 4th, 2026, 8:52 PM
 * Purpose:  Augment a 2-D array by 1 row and 1 column
 */

//Libraries
#include <iostream>//I/O Library
using namespace std;

//No Global Constants

//Function Prototypes
//Positions 1 and 2 are main's row and column counts. int ** returns the rows.
int **getData(int &,int &);                 //Get the Matrix Data
//Position 1 is the matrix. Positions 2 and 3 are how many rows and columns to print.
void printDat(const int * const *,int,int); //Print the Matrix
//Same three positions. Returns a new matrix one row and one column larger.
int **augment(const int * const *,int,int); //Augment the original array
//Position 1 is the matrix. Position 2 is the row count, which is all destroy needs.
void destroy(int **,int);                   //Destroy the Matrix, i.e., reallocate memory

//Execution begins here
int main(int argc, char*argv[]) {
    //Declare variables and fill
    int rows,cols;  //getData writes both sizes
    int **array;    //Address of the row pointers
    
    //Read the original matrix. The newline sits between the two matrices.
    array=getData(rows,cols);
    printDat(array,rows,cols);
    cout<<endl;
    
    //New matrix is (rows+1) by (cols+1). Original is left unchanged.
    int **aug=augment(array,rows,cols);
    printDat(aug,rows+1,cols+1);
    
    //Each matrix is destroyed with its own row count
    destroy(array,rows);
    destroy(aug,rows+1);
    
    //Exit stage right
    return 0;
}

int **getData(int &rows,int &cols){
    //First two numbers are the size. & writes them into main's variables.
    cin>>rows>>cols;
    //One pointer per row, then one integer block per row
    int **array=new int*[rows];
    for(int row=0;row<rows;row++){
        array[row]=new int[cols];
        for(int col=0;col<cols;col++){
            cin>>array[row][col];
        }
    }
    return array;
}

void printDat(const int * const *array,int rows,int cols){
    for(int row=0;row<rows;row++){
        for(int col=0;col<cols;col++){
            cout<<array[row][col];
            if(col<cols-1)cout<<" ";
        }
        if(row<rows-1)cout<<endl;
    }
}

int **augment(const int * const *array,int rows,int cols){
    //const: read the original, do not change it. New size is one larger each way.
    int **aug=new int*[rows+1];
    for(int row=0;row<rows+1;row++){
        aug[row]=new int[cols+1];
        for(int col=0;col<cols+1;col++){
            aug[row][col]=0;    //First row and first column stay 0
        }
    }
    //Original [row][col] lands at [row+1][col+1], shifted down and right
    for(int row=0;row<rows;row++){
        for(int col=0;col<cols;col++){
            aug[row+1][col+1]=array[row][col];
        }
    }
    return aug;
}

void destroy(int **array,int rows){
    //Delete each row block, then the array of row pointers
    for(int row=0;row<rows;row++){
        delete []array[row];
    }
    delete []array;
}

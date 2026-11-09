/* 
 * File:   main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose:  Sum Rows, Sum Columns, Grand Sum of an integer array
 */

//System Libraries Here
#include <iostream>//cin,cout
#include <iomanip> //setw(10)
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
//Allowed like PI, e, Gravity, conversions, array dimensions necessary
const int COLMAX=80;  //Max Columns much larger than needed.

//Function Prototypes Here
void read(int [][COLMAX],int &,int &);//Prompt for size then table
void sum(const int [][COLMAX],int,int,int[][COLMAX]);//Sum rows,col,grand total
void print(const int [][COLMAX],int,int,int);//Either table can be printed

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare all Variables Here
    const int ROW=80;           //Max Rows much larger than needed
    int array[ROW][COLMAX]={};  //Declare original array
    int augAry[ROW][COLMAX]={}; //Actual augmented table row+1, col+1
    int row,col;                
    
    //Input the original table
    read(array,row,col);
    
    //Augment the original table by the sums
    sum(array,row,col,augAry);
    
    //Output the original array
    cout<<endl<<"The Original Array"<<endl;
    print(array,row,col,10);//setw(10)
    
    //Output the augmented array
    cout<<endl<<"The Augmented Array"<<endl;
    print(augAry,row+1,col+1,10);//setw(10)
    
    //Exit
    return 0;
}
//Positions: table, row count, column count. Both ints are references to main.
void read(int array[][COLMAX], int &row, int &col){
    cout<<"nput a table and output the Augment row,col and total sums."<<endl;
    cin>>row>>col;                         //user types the two sizes
    cout<<"First input the number of rows and cols. <20 for each"<<endl;
    cout<<"Now input the table."<<endl;
    for (int i=0; i<row; i++)              //one pass per row
        for (int j=0; j<col; j++)          //one pass per column in that row
            cin>>array[i][j];              //user types array[row][column]
}

//Positions: original table, rows, columns, augmented table.
//Copy the original, then write sums into column col, row row, and the corner.
void sum(const int array[][COLMAX], int row, int col, int augAry[][COLMAX]){
    for (int i=0; i<row; i++)
        for (int j=0; j<col; j++)
            augAry[i][j]=array[i][j];      //same numbers, new table

    for (int i=0; i<row; i++){
        int rowSum=0;
        for (int j=0; j<col; j++)
            rowSum+=augAry[i][j];
        augAry[i][col]=rowSum;             //new column, at the end of this row
    }

    for (int j=0; j<col; j++){
        int colSum=0;
        for (int i=0; i<row; i++)
            colSum+=augAry[i][j];          //original column only, stop before the new column
        augAry[row][j]=colSum;             //new row, under this column
    }

    int grand=0;
    for (int i=0; i<row; i++)
        grand+=augAry[i][col];             //add the row sums already stored
    augAry[row][col]=grand;                //lower corner
}

//Positions: whichever table, how many rows to show, how many columns, setw width.
void print(const int array[][COLMAX], int row, int col, int n){
    for (int i=0; i<row; i++){
        for (int j=0; j<col; j++)
            cout<<setw(n)<<array[i][j];
        cout<<endl;
    }
}
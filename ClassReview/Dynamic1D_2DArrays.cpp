/* 
 * File:   main.cpp
 * Author: Dr. Mark E. Lehr
 * Created on September 22nd, 2021, 1:30 PM
 * Purpose:  Compare 1 and 2 Dimensional Dynamic Arrays
 */

//System Libraries Here
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
//Like PI, e, Gravity, or conversions and Array Dimension


//Function Prototypes Here
void prntAry(const vector<int> &, int);
void prntAry(const vector<vector<int>> &);
vector<int> fillAry(int,int,int);
vector<vector<int>> fillAry(int,int);
void fillAry(vector<vector<int>>&, const vector<int>&, int);
void swap(int &,int &);
void smlLst(vector<int>&, int);
void mrkSort(vector<int> &);

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //Declare all Variables Here
    int rowsize=4;//Row size for both 1 and 2 D arrays
    int colsize=3;//The column size for a 2 dimensional Array
    int lowRng=100,highRng=999;
    int perLine=4;
        
    //Fill each parallel array
    vector<int> array = fillAry(rowsize,highRng,lowRng);
    vector<int> brray = fillAry(rowsize,highRng/10,lowRng/10);
    vector<int> crray = fillAry(rowsize,highRng/100,lowRng/100);
    
    //Sort the array the for all positions
    mrkSort(array);
    mrkSort(brray);
    mrkSort(crray);
    
    //Fill the 2-D array
    vector<vector<int>> table = fillAry(rowsize,colsize);
    fillAry(table,array,0);
    fillAry(table,brray,1);
    fillAry(table,crray,2);
    
    //Print the values in the array
    prntAry(array,perLine);
    prntAry(brray,perLine);
    prntAry(crray,perLine);
    prntAry(table);
    


    //Exit
    return 0;
}

void fillAry(vector<vector<int>>& a, const vector<int>& c, int wchCol){
    for(int row=0;row<static_cast<int>(a.size());row++){
        a[row][wchCol]=c[row];
    }
}

vector<int> fillAry(int n,int hr,int lr){
    n=n<2?2:n;
    vector<int> a(n);
    for(int indx=0;indx<n;indx++){
        vector<int> a(n);
    }
    return a;
}

vector<vector<int>> fillAry(int rows,int cols){
    //Allocate Memory for 2-D Array
    rows=rows<2?2:rows;
    cols=cols<2?2:cols;
    return vector<vector<int>>(rows,vector<int>(cols,0));
}

void prntAry(const vector<vector<int>> &a){
     cout<<endl<<"The Array Values"<<endl;
    for(int row=0;row<static_cast<int>(a.size());row++){
        for(int col=0;col<static_cast<int>(a[row].size());col++){
            cout<<setw(4)<<a[row][col];
        }
        cout<<endl;
    }
}

void prntAry(const vector<int> &a, int perLine){
    //Print the values in the array
    cout<<endl<<"The Array Values"<<endl;
    for(int indx=0;indx<static_cast<int>(a.size());indx++){
        cout<<a[indx]<<" ";
        if(indx%perLine==(perLine-1))cout<<endl;
    }
    cout<<endl;
}

void mrkSort(vector<int> &a){
    for(int pos=0;pos<static_cast<int>(a.size())-1;pos++){
        smlLst(a,pos);
    }
}

void smlLst(vector<int>& a, int pos){
    for(int i=pos+1;i<static_cast<int>(a.size());i++){
        if(a[pos]>a[i]){
            swap(a[pos],a[i]);
        }
    }
}
void swap(int&a, int&b){
    int temp=a;
    a=b;
    b=temp;
}
/* 
 * File:   main.cpp
 * Author: YOUR NAME HERE
 * Created on DATE AND TIME HERE
 * Purpose:  Even, Odd Vectors and Array Columns Even, Odd
 * Note:  Check out content of Sample conditions in Hacker Rank
 * Input size of integer array, then array, output columns of Even, Odd
 * Vectors then Even, Odd 2-D Array
 */

//System Libraries Here
#include <iostream>//cin,cout
#include <vector>  //vectors<>
#include <iomanip> //Format setw(),right
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
//Allowed like PI, e, Gravity, conversions, array dimensions necessary
const int COLMAX=2;//Only 2 columns needed, even and odd

//Function Prototypes Here
void read(vector<int> &, vector<int> &);
void copy(vector<int>, vector<int>,int [][COLMAX]);
void prntVec(vector<int>, vector<int>,int);//int n is the format setw(n)
void prntAry(const int [][COLMAX],int,int,int);

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare all Variables Here
    const int ROW=80;           //No more than 80 rows
    int array[ROW][COLMAX];     //Really, just an 80x2 array, even vs. odd
    vector<int> even(0),odd(0); //Declare even,odd vectors
    
    //Input data and place even in one vector odd in the other
    read(even,odd);
    
    //Now output the content of the vectors
    //          setw(10)
    prntVec(even,odd,10);//Input even, odd vectors with setw(10);
    
    //Copy the vectors into the 2 dimensional array
    copy(even,odd,array);
    
    //Now output the content of the array
    //                              setw(10)
    prntAry(array,even.size(),odd.size(),10);//Same format as even/odd vectors
    
    //Exit
    return 0;
}
//Positions: even, odd. The & means push_back fills main's vectors.
void read(vector<int> &even, vector<int> &odd){
    int count, number;
    cout<<"Input the number of integers to input."<<endl;
    cin>>count;                              //how many integers follow
    cout<<"Input each number."<<endl;
    for (int i=0; i<count; i++){
        cin>>number;
        if (number%2==0) even.push_back(number); //remainder 0 means even
        else odd.push_back(number);
    }
}
//Positions: copy of even, copy of odd, main's array.
//Column 0 receives the even values. Column 1 receives the odd values.
void copy(vector<int> even, vector<int> odd, int array[][COLMAX]){
    for (int i=0; i<even.size(); i++)
        array[i][0]=even[i];
    for (int i=0; i<odd.size(); i++)
        array[i][1]=odd[i];
}

//Three fields of width n: a blank label column, then Even, then Odd
void prntVec(vector<int> even, vector<int> odd, int n){
    int rows=even.size();
    if (odd.size()>rows) rows=odd.size();
    cout<<setw(n)<<"Vector"<<setw(n)<<"Even"<<setw(n)<<"Odd"<<endl;
    for (int i=0; i<rows; i++){
        cout<<setw(n)<<"";                              //label column stays blank
        if (i<even.size()) cout<<setw(n)<<even[i];
        else cout<<setw(n)<<"";
        if (i<odd.size()) cout<<setw(n)<<odd[i];
        else cout<<setw(n)<<"";
        cout<<endl;
    }
}

//Same three fields. The title in the first field is Array.
void prntAry(const int array[][COLMAX], int evenSize, int oddSize, int n){
    int rows=evenSize;
    if (oddSize>rows) rows=oddSize;
    cout<<setw(n)<<"Array"<<setw(n)<<"Even"<<setw(n)<<"Odd"<<endl;
    for (int i=0; i<rows; i++){
        cout<<setw(n)<<"";
        if (i<evenSize) cout<<setw(n)<<array[i][0];    //column 0 is the even value
        else cout<<setw(n)<<"";
        if (i<oddSize) cout<<setw(n)<<array[i][1];     //column 1 is the odd value
        else cout<<setw(n)<<"";
        cout<<endl;
    }
}
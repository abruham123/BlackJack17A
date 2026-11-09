/* 
 * Note:  This time you create the strcmp() function that takes into
 * account the new sort order.
 */

//System Libraries Here
#include <iostream>//cin,cout
#include <cstring> //strlen(),strcmp(),strcpy()
using namespace std;

//Global Constants Only, No Global Variables
//Allowed like PI, e, Gravity, conversions, array dimensions necessary
const int COLMAX=80;//Only 20 required, and 1 for null terminator

//Function Prototypes Here
int  read(char [][COLMAX],int &);//Outputs row and columns detected from input
void sort(char [][COLMAX],int,int,const char[],const char[]);//Sort by row using strcmp();
void print(const char [][COLMAX],int,int);//Print the sorted 2-D array
//int strcmp(char a[],char b[],char replace[],char with[]){
int strcmp(char [],char [],const char [],const char []);//Replace sort order

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare all Variables Here
    const int ROW=30;             //Only 20 required
    char array[ROW][COLMAX];      //Bigger than necessary 
    int colIn,colDet,rowIn,rowDet;//Row, Col input and detected
    char replace[COLMAX],with[COLMAX];//New sort order
    
    //Input the new sort order then sort
    cout<<"Modify the sort order by swapping these characters."<<endl;
    cin>>replace;
    cout<<"With the following characters"<<endl;
    cin>>with;
    cout<<"Read in a 2 dimensional array of characters and sort by Row"<<endl;
    cout<<"Input the number of rows <= 20"<<endl;
    cin>>rowIn;
    cout<<"Input the maximum number of columns <=20"<<endl;
    cin>>colIn;
    
    //Now read in the array of characters and determine it's size
    rowDet=rowIn;
    cout<<"Now input the array."<<endl;
    colDet=read(array,rowDet);
    
    //Compare the size input vs. size detected and sort if same
    //Else output different size
    //if(rowDet==rowIn&&colDet==colIn){
        sort(array,rowIn,colIn,replace,with);
        cout<<"The Sorted Array"<<endl;
        print(array,rowIn,colIn);
    
    //Exit
    return 0;
}
//Same as problem 3. Count from 0 then write the real row count back through &
int read (char array[][COLMAX], int &rowDet){
    int rowsRead=0;
    int longest=0;

    while(cin>>array[rowsRead]){        //One word goes into this row
        int length=strlen (array[rowsRead]);    //letters before '\0'
        if(length>longest) longest=length;
        rowsRead++;
    }

    rowDet=rowsRead;
    return longest;     //main stores this in colDet
}
//Pass replace & with so this calls strcmp not 2argument library
//2 extra arguments are what makes you call strcmp
void sort(char array[][COLMAX], int rows, int cols, const char replace[], const char with[]){
    for (int i=0; i<rows-1; i++)        //One pass per row
        for (int j=0; j<rows-1; j++)    //Compare row j with next row
            if (strcmp(array[j], array[j+1], replace, with)>0)
                swap (array[j], array[j+1]);        //Same row exchange
}
//This is what gets outputed
void print (const char array[][COLMAX], int rows, int cols){
    for (int i=0; i<rows; i++)
    cout<<array[i]<<endl;
}

int strcmp(char a[], char b[], const char replace [], const char with []){
    int lenA=strlen(a);
    int lenB=strlen(b);
    int n=lenA;
    if (lenB<n) n=lenB;                      //only compare letters both rows have
    int pairs=strlen(replace);
    if (strlen(with)<pairs) pairs=strlen(with);
    for (int i=0; i<n; i++){
        char letterA=a[i];
        char letterB=b[i];
        for (int k=0; k<pairs; k++){         //linear search through the swap list
            if (a[i]==replace[k]) letterA=with[k];
            if (b[i]==replace[k]) letterB=with[k];
        }
        if (letterA>letterB) return 1;       //same test as array[j] > array[j+1]
        if (letterA<letterB) return -1;
    }
    if (lenA==lenB) return 0;                //rows matched through the end
    if (lenA>lenB) return 1;                 //the longer row belongs after
    return -1;
}
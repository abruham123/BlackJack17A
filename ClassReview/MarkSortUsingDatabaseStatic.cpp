/*






*/


#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;
//User Libraries
//Function protototypes
void FillArray(int [],int [],int);
void PrintArray(int [], int, int);
void mrkSrt1 (int [], int);








//Global Constants
int main (int argv, char **argc)
{

//Set random number seed
srand(static_cast<unsigned int>(time(0)));

//Declare Variables
const int SIZE=100;     //Constant size of the array (static) known at compile time
int array[SIZE];        //Array of 100 ints uninitialized (garabage until filled)
int indx [SIZE];        //100 ints that will hold the position 0-99

//Initialize Variables
cout<<endl<<endl<<"Before Sorting"<<endl;
FillArray(array,indx,SIZE);
PrintArray(array, SIZE, 10);
PrintArray(indx, SIZE, 10);

//Mapping Input to Output
mrkSrt1(array,SIZE);

//Display Results
cout<<endl<<endl<<"After Sorting"<<endl;
PrintArray(array, SIZE, 10);
PrintArray(indx, SIZE, 10);

//Exit Stage right
return 0;
}

//Prototype Definition and parameters must match eg int, int and the variables a and n
void mrkSrt1( int a[], int n){
for(int i=0; i<n-1; i++){
    for(int j=1+i; j<n; j++){
        if(a[i]>a[j]){              //If a[i] is greater than a[j], swap (smaller value is further right)
            int temp=a[i];          //swap a[1] and a[j] using a temp
            a[i]=a[j];
            a[j]=temp;
        }
    }
}
}
//Filla array a[] w/ random 2-digit #'s and indx[] with each elements position
void FillArray(int a[], int indx[],int n){
    for (int i=0; i<n; i++){
        a[i]=rand()%90+10;      //[10,99]
        indx[i]=i;
    }
}
//Print n values of a[], starting new line after ever perLine values
void PrintArray(int a[], int n, int perLine){
    cout<<endl;
    for(int i=0; i<n; i++){
        cout<<a[i]<<" ";
        if(i%perLine==(perLine-1)) cout<<endl; //End of each row true @ i=9,19,29...
    }
    cout<<endl;

}







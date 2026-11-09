#include<iostream>
using namespace std;
//Function prototypes
void split(int n, int &d0, int &d1, int &d2, int &d3);      //Pull 1 into into 4 digigts like place value
bool badDgts(int d0, int d1, int d2, int d3);               //true if any digit is 8/9 or <0
int build(int d0, int d1, int d2, int d3);                           //build one int from 4 digits
int encrypt(int n);       

//Function begins
int main(){
//Initialization
int n;
cout<<"Enter a valid 4 digit number (0-7): ";
cin>>n;     //One cin for whole number

int enc= encrypt(n);
if(enc<0){
    cout<<"Error: Invalid digits ONLY 0-7.\n";
    
}else{
        cout<<"Encrypted: "<<enc<<endl;
    }
    return 0;
}
void split(int n, int &d0, int &d1, int &d2, int &d3){
    d0=(n/1000)%10;         //First digit
    d1=(n/100)%10;
    d2=(n/10)%10;
    d3=n%10;                //Last digit
}
bool badDgts(int d0, int d1, int d2, int d3){
    // put digits in an array so a for-loop can check all 4
    int d[4] = {d0, d1, d2, d3};
    for(int i = 0; i < 4; i++){
        if(d[i] < 0 || d[i] > 7) return true;  // 8 or 9 (or junk)
    }
    return false;   // all digits valid 0-7
}
int build(int d0, int d1, int d2, int d3){
    return d0*1000 + d1*100 + d2*10 + d3;
}
int encrypt(int n){
    int d0, d1, d2, d3;
    split(n, d0, d1, d2, d3);

    if (badDgts(d0,d1,d2,d3)) return-1; //Signal error to main

    //Math for each digit
    d0=(d0+3)%8;
    d1=(d1+3)%8;
    d2=(d2+3)%8;
    d3=(d3+3)%8;

    //Swap
    int t;
    t=d0; d0=d2;  d2=t;
    t=d1; d1=d3;  d3=t;

    return build(d0,d1,d2,d3);
}







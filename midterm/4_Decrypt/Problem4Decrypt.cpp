#include<iostream>
using namespace std;
//Function prototypes   (Same as encrypt)
void split(int n, int &d0, int &d1, int &d2, int &d3);
bool badDgts(int d0, int d1, int d2, int d3);
int build(int d0, int d1, int d2, int d3);
int decrypt(int n);

//Where program begins
int main(){
    int n;
    cout<<"Enter a valid 4 digit number(0-7): ";
    cin>>n;

    int orig = decrypt(n);
    if(orig<0){
        cout<<"Error: Invalid digits ONLY 0-7.\n";
    }else{
        cout<<"Decrypted: "<<orig<<endl;
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
    int decrypt(int n){
        int d0, d1, d2, d3;
        split(n, d0, d1, d2, d3);

        if(badDgts(d0, d1,d2,d3)) return -1;

        //Undo swap FIRST
        int t;
        t = d0; d0 = d2; d2 = t;
        t = d1; d1 = d3; d3 = t;

        //Undo the math
        d0 = (d0 + 5) % 8;
         d1 = (d1 + 5) % 8;
        d2 = (d2 + 5) % 8;
        d3 = (d3 + 5) % 8;
        return build(d0, d1, d2, d3);
    }
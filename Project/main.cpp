#include<iostream>
using namespace std;
char suit(int);
char face(int);
char value(int);
int main(int argc, char **argv){
    for (int i=0; i<52; i++){
        cout<<i<<" "<<face(i)<<suit(i)<<" "<<static_cast<int>(value(i))<<endl;
    }
return 0;
}
char value(int n){
    n>=0?n%=13:0;
    n++;
    if (n>10)n=10;
    return n;
}
char face(int n){
    n>=0?n%=52:0;
    char faces[]={'A','2','3','4','5','6','7','8','9','T','J','Q','K'};
    return faces[n%13];
}

char suit(int n){
    n>=0?n%=52:0;
    char suits[]={'S','D','C','H'};
    return suits[n/13];
}
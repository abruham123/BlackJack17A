/* V2
 * File:   main.cpp
 * Author: Abraham Murillo
 * Created on DATE AND TIME HERE
 * Purpose:  Input something, output it reversed with some modifications
 * Note:Range should be 5 digits, 321 = 00321 reverse = 12300 before subtraction
 *      12300 - 999 = 11301 after subtraction
 *      12300 = 00321 = 321 after reversal and no subtraction
 */

//System Libraries Here
#include <iostream>//cin,cout,endl
#include <cstring> //strlen()
using namespace std;
//User Libraries Here

//Global Constants Only, No Global Variables
//Like PI, e, Gravity, or conversions

//Function Prototypes Here
bool  inRange(const char [],unsigned short &);//Output true,unsigned or false
bool  reverse(unsigned short,signed short &);//Output true,short or false
short subtrct(signed short,int);

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare all Variables Here
    const int SIZE=80;     //More than enough supposed to be at least 5 
    char  digits[SIZE];    //Whats the user types still chars and not numbers
    unsigned short unShort;//In range, stores the number here 0-65535
    short snShort;         //Reverse stores the flipped number here
    
    //Input or initialize values Here
    cout<<"Reverse a number and subtract if possible."<<endl;
    cout<<"Input a number in the range of an unsigned short"<<endl;
    cin>>digits;        //One word of character such as 321
    
    //Test if it is in the Range of an unsigned short
    //inRange function is called first and done first
    if(!inRange(digits,unShort)){           //returns false for letters or numbers above 65535
        cout<<"No Conversion Possible"<<endl;
        return 0;
    }
    
    //Reverse and see if it falls in the range of an signed short
    //ONLY runs when inRange is returned true
    if(!reverse(unShort,snShort)){
        cout<<"No Conversion Possible"<<endl;
        return 0;       //Stops here 999 if not subtracted
    }
    
    //Now subtract if the result is not negative else don't subtract
    snShort=subtrct(snShort,999);
    
    //Output the result. 321 prints 11301. 12300 prints 321
    cout<<snShort<<endl;
    
    //Exit
    return 0;
}
//True when every character is a digit and the value is 0 through 65535
bool inRange(const char digits[], unsigned short &unShort){
    int len=strlen(digits);
        if (len==0 || len>5) return false;  
                //unsigned short is at most 5 digits
    int value=0;
        for (int i=0; i<len; i++){
            if (digits[i]<'0' || digits[i]>'9') return false; //character is not a digit
        value=value*10+(digits[i]-'0');     //'5'-'0' gives the number 5 ('0' is a char)
    }
        if (value>65535) return false;      //Checks for bounds too big
        unShort=value;                              //writes main's unShort
        return true;
}
//Pad to 5 digits, reverse them, and keep the result only if it fits in a signed short
bool reverse(unsigned short unShort, signed short &snShort){
    char digits[6]="00000";     //321 will be stores as 00321
    for (int i=4; i>=0; i--){   //Does the padding because of the amout of digits
        digits[i]=(unShort%10)+'0';             //Remainder 1 because the character '1'
        unShort/=10;                            //Drops the last digit and moves left
    }
    for (int i=0; i<2; i++){                    //Swap ends; 00321 becomes 12300
        char temp=digits[i];
        digits[i]=digits[4-i];
        digits[4-i]=temp;
    }
    int value=0;
    for (int i=0; i<5; i++)
        value=value*10+(digits[i]-'0');
    if (value>32767) return false;              //signed short maximum
    snShort=value;
    return true;
}
//Subtract only when the result stays zero or positive
//This is the function that is outputed
short subtrct(signed short snShort, int value){
    if (snShort>=value) return snShort-value;       //12300-999 = 11301
    return snShort;
}
/* 
 * Purpose:  Searching for multiple occurrence of patterns
 * Note:  cout proceeds to null terminator, cin reads to end of line
 *        for character arrays
 * 
 */

//System Libraries Here
#include <iostream>//cin,cout,getline()
#include <cstring> //strlen()
using namespace std;

//User Libraries Here

//Global Constants Only, No Global Variables
//PI, e, Gravity, or conversions

//Function Prototypes Begins Here
//srch1 utility function Input->start position, Output->position found or not
//srch1 is a simple linear search function, repeat in srchAll till all found
//srch1 Input->sentence, pattern, start position Output-> position found
//Remember arrays start at index/position 0
//srchAll Input->sentence, pattern Output->position array
int  srch1(const char [],const char [],int);//Search for 1 occurrence
void srchAll(const char [],const char [],int []);//Search for all occurrences
void print(const char []);//Print the character arrays
void print(const int []); //Print the array of indexes where the pattern found

//Program Execution Begins Here
int main(int argc, char** argv) {
    //Declare all Variables Here
    const int LINE=81;               //Size of sentence or pattern to find
    char sntnce[LINE],pattern[LINE]; //80 + null terminator
    int match[LINE];                 //Index array where pattern was found
    
    //Input a sentence and a pattern to match
    //cin.getline reads a whole line spaces included and stores the null terminator "\0"
    cout<<"Match a pattern in a sentence."<<endl;
    cout<<"Input a sentence"<<endl;
    cin.getline(sntnce,LINE);
    cout<<"Input a pattern."<<endl;
    cin.getline(pattern,LINE);
    
    //Search for the pattern
    //Input the sentence and pattern, Output the matching positions
    //Remember, indexing starts at 0 for arrays.
    srchAll(sntnce,pattern,match);
    
    //Display the inputs and the Outputs
    cout<<endl<<"The sentence and the pattern"<<endl;
    print(sntnce);
    print(pattern);
    cout<<"The positions where the pattern matched"<<endl;
    print(match);
    
    //Exit
    return 0;
}

//Return the first index at or after start where the whole pattern matches, or -1
int srch1(const char sntnce[], const char pattern[], int start){
    int sLen=strlen(sntnce);
    int pLen=strlen(pattern);

    for (int s=start; s+pLen<=sLen; s++){
        int k=0;
        while (k<pLen && sntnce[s+k]==pattern[k])
            k++;
        if (k==pLen) return s;
    }
    return -1;
}

//Store each match. If srch1 never finds one, match[0] stays -1.
void srchAll(const char sntnce[], const char pattern[], int match[]){
    int found=srch1(sntnce,pattern,0);
    if (found==-1){
        match[0]=-1;                  //no match; print writes "None"
        return;
    }
    int count=0;
    while (found!=-1){
        match[count]=found;
        count++;
        found=srch1(sntnce,pattern,found+1);
    }
    match[count]=-1;
}

//print(sntnce) and print(pattern) use this one. cout stops at '\0'.
void print(const char array[]){
    cout<<array<<endl;
}

//print(match). A -1 in the first slot means the pattern was not found.
void print(const int array[]){
    if (array[0]==-1){
        cout<<"None"<<endl;
        return;
    }
    for (int i=0; array[i]!=-1; i++)
        cout<<array[i]<<endl;
}
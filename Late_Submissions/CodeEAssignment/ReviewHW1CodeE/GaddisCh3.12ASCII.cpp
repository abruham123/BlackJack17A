//system libraries
#include<iostream>
using namespace std;
//user libraries
//global constants
//function protoypes 
//execution begins here
int main(int argc, char** argv)
 {
    //declare variables             //Prompt,read,process,display
    char letter;
    //display the heading
    cout<<"Enter any letter: ";
    cin>>letter;
    //display the ascii table
    cout<<static_cast<int>(letter)<<endl; //Uses static cast to convert the char to an int and display ascii value
    
    return 0;
}

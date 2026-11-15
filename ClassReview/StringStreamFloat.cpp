// swapping ostringstream objects
//Used to convert and output string stream into datatypes 
//Can used the string compare example file from CIS5 to see
//How you can compare 2 strings
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
using namespace std;

//Function to check if the string is a number (true or false)
bool isNum(string);

int main () {

  stringstream ss;
  string y;
  do{
    cout<<"Type in 2 integers or floats"<<endl;
    getline(cin,y);
  }while(!isNum(y));
  ss << y;
  float foo,bar;
  ss >> foo >> bar;

  cout << "foo: " << foo << '\n';
  cout << "bar: " << bar << '\n';

  return 0;
}

bool isNum(string x){
    for(int i=0;i<x.length();i++){
        if(!(((x[i]>='0')&&(x[i]<='9'))||x[i]==' '||x[i]=='.'))return false;
    }
    return true;
}
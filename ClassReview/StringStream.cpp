// swapping ostringstream objects
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
using namespace std;

bool isNum(string);

int main () {

  std::stringstream ss;
  string y;
  do{
    cout<<"Type in 2 integers"<<endl;
    getline(cin,y);
  }while(!isNum(y));
  //Can write whole line and then put them into the ints
  ss << y;
  int foo,bar;  
  ss >> foo >> bar;   //Feed values into the 2 integers

  cout << "foo: " << foo << '\n';
  cout << "bar: " << bar << '\n';

  return 0;
}

bool isNum(string x){
    for(int i=0;i<x.length();i++){
        if(!(((x[i]>='0')&&(x[i]<='9'))||x[i]==' '))return false;
    }
    return true;
}
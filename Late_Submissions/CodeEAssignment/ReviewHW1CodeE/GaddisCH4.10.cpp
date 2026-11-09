#include<iostream>
using namespace std;
int main()
{
int sales,bonus;        //Holds whole int numbers and entered by user
float commisionRate;    //Holds decimal numbers and calculated by program

cout<<"Enter the sales amount: ";   //Prompts user before cin the amount (needs to be a whole number)
cin>>sales;

    if (sales>50000)    //Only one condition needded (when sales is greater than 50000)
    {                   //Opening brace puts all code that belongs to this condition

        //Assigned inside the if and only applies when condition is true
        commisionRate=0.25;     
        bonus=250;              
                  
        //Output is inside the if statement and is only printed when condition is true
        //If condition is on the outside it will ALWAYS print the output
        cout<<"The sales amount is: "<<sales<<endl;
        cout<<"The commision rate is: "<<commisionRate<<endl;  
        cout<<"The bonus is: "<<bonus<<endl;
    }       //Closing brace ends the if statement and code block
   

return 0;
}


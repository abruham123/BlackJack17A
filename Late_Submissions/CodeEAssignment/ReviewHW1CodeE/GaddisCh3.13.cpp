//System Libraries - Post Here
#include <iostream>
#include <iomanip>
using namespace std;

//User Libraries-Post Here
//Global Constants - Post Here
//Only Universal Physics/Math/Conversions found here
//No Global Variables
//Higher Dimension arrays requiring definition prior to prototype only.
//Function Prototypes - Post Here

int main(int argc, char** argv) //Determines structure of function every C++ program starts here
{
    //Declare variables before any calculations or output
    int int1, int2;         //Integer varaibles (will lose decimal in integer division)
    double result;          //Double variable (will keep decimal in double division) used to store division results
    
    //Initialize variables
    int1=19;    //Initialize integer 1
    int2=2;     //Initialize integer 2
    
    //Integer division will lose the decimal
    result=int1/int2;       
    
    // Displays 9 — proves that storing in a double doesn't save the lost decimal
    cout << result << endl;
   
    // CAST BEFORE DIVIDING: int1 is converted to 19.0 BEFORE division occurs
    // 19.0 / 2 = 9.5 — decimal is preserved because one operand is now double
    // result is overwritten with 9.5
    result = static_cast<double>(int1) / int2;
   
    // Displays 9.5 — proves casting the numerator BEFORE dividing saves the decimal
    cout << result << endl;
   
    // CAST AFTER DIVIDING: int1/int2 is still integer division first → 9
    // THEN that 9 is cast to double → 9.0 (too late, decimal already lost)
    // result is overwritten with 9.0
    result = static_cast<double>(int1 / int2);
   
    // Displays 9 — proves casting the RESULT after division doesn't recover the decimal
    cout << result << endl;
   
    return 0;
    //Bottom line: The / is the deciding factor whether the decimal is lost or preserved
}

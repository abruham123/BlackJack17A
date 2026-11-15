/*
 * File:   Ga9EdC11P3.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 11 - Programming Challenge 3
 * Corporate Sales Data
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Physics/Math/Conversions found here

//Structure definition - holds data for one company division
struct Division {
    string name;        // Division Name (East, West, North, South)
    float q1;           // First-Quarter Sales
    float q2;           // Second-Quarter Sales
    float q3;           // Third-Quarter Sales
    float q4;           // Fourth-Quarter Sales
    float total;        // Total Annual Sales
    float average;      // Average Quarterly Sales
};

//Function Prototypes
void getSales(Division &);
void calcSales(Division &);
void showSales(const Division &);

//Execution Begins Here
int main(int argc, char** argv) {
    //Declare four Division structure variables
    Division north, west, east, south;

    //Initialize division names
    north.name = "North";
    west.name  = "West";
    east.name  = "East";
    south.name = "South";

    //Format money output to 2 decimal places
    cout << fixed << setprecision(2);
    

    //CodeE order: North, West, East, South
    //For each division: prompt/read, calculate, then display totals
    getSales(north);
    calcSales(north);
    showSales(north);

    getSales(west);
    calcSales(west);
    showSales(west);

    getSales(east);
    calcSales(east);
    showSales(east);

    getSales(south);
    calcSales(south);
    showSales(south);
    

    //Exit stage right
    return 0;
}

//Print division name, then ask for Q1-Q4 sales
void getSales(Division &div) {
    cout << div.name << endl;

    cout << "Enter first-quarter sales:" << endl;
    cin >> div.q1;

    cout << "Enter second-quarter sales:" << endl;
    cin >> div.q2;

    cout << "Enter third-quarter sales:" << endl;
    cin >> div.q3;

    cout << "Enter fourth-quarter sales:" << endl;
    cin >> div.q4;
}

//Store total annual sales and average quarterly sales
void calcSales(Division &div) {
    div.total = div.q1 + div.q2 + div.q3 + div.q4;
    div.average = div.total / 4.0f;
}

//Display total and average only (matches CodeE expected output)
void showSales(const Division &div) {
    cout << "Total Annual sales:$" << div.total << endl;
    cout << "Average Quarterly Sales:$" << div.average << endl;
}

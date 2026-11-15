/*
 * File:   StructureDrinkMachine.cpp
 * Author: Abraham Murillo
 * Soft Drink Machine Simulator
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Constants, Math, Physics, Conversions, Higher Dimensions

const int NUM_DRINKS = 5;
const int NAME_WIDTH = 11;   // Name field width padded with spaces

//Structure for one drink in the machine
struct Drink {
    string name;    // Drink Name
    int cost;       // Drink Cost in cents
    int quantity;   // Number of Drinks in Machine
};

//Function Prototypes
void displayMenu(const Drink []);
int findDrink(const Drink [], const string &);

//Execution Begins Here
int main(int argc, char** argv) {
    //Initialize the five drinks (cost stored in cents)
    Drink machine[NUM_DRINKS] = {
        {"Cola", 75, 20},
        {"Root Beer", 75, 20},
        {"Lemon-Lime", 75, 20},
        {"Grape Soda", 80, 20},
        {"Cream Soda", 80, 20}
    };

    int earned = 0;   // Total money earned in cents
    string selection;
    int money;        // Money inserted in cents (0 to 100)

    //Loop until the user chooses Quit or input ends
    while (true) {
        displayMenu(machine);

        //Read drink name or Quit (no prompt)
        if (!getline(cin, selection)) {
            break;
        }
        if (selection == "Quit") {
            break;
        }

        //Find the selected drink by name
        int index = findDrink(machine, selection);
        if (index < 0) {
            continue;
        }

        //Sold out
        if (machine[index].quantity <= 0) {
            cout << "Sold Out" << endl;
            continue;
        }

        //Read money in cents. Validate: not negative, not greater than 100 ($1.00)
        if (!(cin >> money)) {
            break;
        }
        cin.ignore(1000, '\n');   // Clear newline for next getline

        if (money < 0 || money > 100) {
            continue;
        }
        if (money < machine[index].cost) {
            continue;
        }

        //Print change in cents, then update stock and earnings
        cout << (money - machine[index].cost) << endl;
        machine[index].quantity--;
        earned += machine[index].cost;
    }

    //Display total earnings in cents, with trailing newline
    cout << earned << endl;

    //Exit stage right!
    return 0;
}

//Display menu: Name padded with spaces (not dots), cost in cents, quantity
void displayMenu(const Drink machine[]) {
    for (int i = 0; i < NUM_DRINKS; i++) {
        cout << left << setw(NAME_WIDTH) << setfill(' ') << machine[i].name
             << machine[i].cost
             << "  " << machine[i].quantity << endl;
    }
    cout << "Quit" << endl;
}

//Return index of drink with matching name, or -1 if not found
int findDrink(const Drink machine[], const string &name) {
    for (int i = 0; i < NUM_DRINKS; i++) {
        if (machine[i].name == name) {
            return i;
        }
    }
    return -1;
}

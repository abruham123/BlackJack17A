/*
 * File:   Ga9EdC11P11.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 11 - Programming Challenge 11
 * Monthly Budget
 */

//System Libraries
#include <iostream>
#include <iomanip>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Constants, Math, Physics, Conversions, Higher Dimensions

//Structure to hold monthly expense categories
struct MonthlyBudget {
    float housing;
    float utilities;
    float household;
    float transportation;
    float food;
    float medical;
    float insurance;
    float entertainment;
    float clothing;
    float miscellaneous;
};

//Function Prototypes
void getSpent(MonthlyBudget &);
void displayReport(const MonthlyBudget &, const MonthlyBudget &);
float totalBudget(const MonthlyBudget &);
void showStatus(const char *, float, float);

//Execution Begins Here
int main(int argc, char** argv) {
    //Budget amounts from the assignment
    MonthlyBudget budget = {
        500.00f,  // Housing
        150.00f,  // Utilities
         65.00f,  // Household Expenses
         50.00f,  // Transportation
        250.00f,  // Food
         30.00f,  // Medical
        100.00f,  // Insurance
        150.00f,  // Entertainment
         75.00f,  // Clothing
         50.00f   // Miscellaneous
    };

    //Amounts actually spent this month
    MonthlyBudget spent;

    //Format money output
    cout << fixed << setprecision(2);

    //Get spending for each category
    getSpent(spent);

    //Display over/under report for each category and total
    displayReport(budget, spent);

    //Exit stage right!
    return 0;
}

//Ask the user for amounts spent in each budget category
void getSpent(MonthlyBudget &spent) {
    cout << "Enter housing cost for the month: $" << endl;
    cin >> spent.housing;

    cout << "Enter utilities cost for the month: $" << endl;
    cin >> spent.utilities;

    cout << "Enter household expenses cost for the month: $" << endl;
    cin >> spent.household;

    cout << "Enter transportation cost for the month: $" << endl;
    cin >> spent.transportation;

    cout << "Enter food cost for the month: $" << endl;
    cin >> spent.food;

    cout << "Enter medical cost for the month: $" << endl;
    cin >> spent.medical;

    cout << "Enter insurance cost for the month: $" << endl;
    cin >> spent.insurance;

    cout << "Enter entertainment cost for the month: $" << endl;
    cin >> spent.entertainment;

    cout << "Enter clothing cost for the month: $" << endl;
    cin >> spent.clothing;

    cout << "Enter miscellaneous cost for the month: $" << endl;
    cin >> spent.miscellaneous;
}

//Sum all categories in a MonthlyBudget
float totalBudget(const MonthlyBudget &b) {
    return b.housing + b.utilities + b.household + b.transportation +
           b.food + b.medical + b.insurance + b.entertainment +
           b.clothing + b.miscellaneous;
}

//Print Even / Under / Over for one category (no dollar amount on these lines)
void showStatus(const char *name, float budgetAmt, float spentAmt) {
    cout << name << ": ";
    if (spentAmt > budgetAmt) {
        cout << "Over" << endl;
    } else if (spentAmt < budgetAmt) {
        cout << "Under" << endl;
    } else {
        cout << "Even" << endl;
    }
}

//Display status for each category, then total over/under amount
void displayReport(const MonthlyBudget &budget, const MonthlyBudget &spent) {
    showStatus("Housing", budget.housing, spent.housing);
    showStatus("Utilities", budget.utilities, spent.utilities);
    showStatus("Household Expenses", budget.household, spent.household);
    showStatus("Transportation", budget.transportation, spent.transportation);
    showStatus("Food", budget.food, spent.food);
    showStatus("Medical", budget.medical, spent.medical);
    showStatus("Insurance", budget.insurance, spent.insurance);
    showStatus("Entertainment", budget.entertainment, spent.entertainment);
    showStatus("Clothing", budget.clothing, spent.clothing);
    showStatus("Miscellaneous", budget.miscellaneous, spent.miscellaneous);

    float diff = totalBudget(spent) - totalBudget(budget);
    if (diff > 0) {
        cout << "You were $" << diff << " over budget";
    } else if (diff < 0) {
        cout << "You were $" << -diff << " under budget";
    } else {
        cout << "You were $0.00 even budget";
    }
}

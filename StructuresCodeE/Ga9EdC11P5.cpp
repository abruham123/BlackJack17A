/*
 * File:   Ga9EdC11P5.cpp
 * Author: Abraham Murillo
 * Gaddis 9th Ed. Chapter 11 - Programming Challenge 5
 * Weather Statistics Modification
 *
 * Challenge 4 weather program modified to use an enumerated
 * type for the months, and that enum to step through the array.
 */

//System Libraries
#include <iostream>
#include <string>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Constants, Math, Physics, Conversions, Higher Dimensions

const int NUM_MONTHS = 12;

//Enumerated data type for months - used to step through the weather array
enum Month {
    JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE,
    JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
};

//Structure to hold weather data for one month
struct WeatherData {
    string month;       // Month name
    float rainfall;     // Total Rainfall - Inches
    float highTemp;     // High Temperature - Degrees Fahrenheit
    float lowTemp;      // Low Temperature - Degrees Fahrenheit
    float avgTemp;      // Average Temperature - Degrees Fahrenheit (calculated)
};

//Function Prototypes
void getData(WeatherData []);
void calcAverages(WeatherData []);
void displayStats(const WeatherData []);
float getTemp();

//Execution Begins Here
int main(int argc, char** argv) {
    //Array of 12 structures for the year
    WeatherData year[NUM_MONTHS];

    //Month names for the Month member
    string names[NUM_MONTHS] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    //Initialize month names using the Month enum
    for (Month m = JANUARY; m <= DECEMBER; m = static_cast<Month>(m + 1)) {
        year[m].month = names[m];
    }

    //Get data for each month (no prompts - keeps output under CodeE 1024-byte limit)
    getData(year);

    //Calculate average temperature for each month
    calcAverages(year);

    //Display yearly statistics
    displayStats(year);

    //Exit stage right!
    return 0;
}

//Read a temperature; re-read until in [-100, 140] (no prompt text)
float getTemp() {
    float temp;
    if (!(cin >> temp)) {
        return 0.0f;
    }
    while ((temp < -100.0f || temp > 140.0f) && cin) {
        if (!(cin >> temp)) {
            break;
        }
    }
    return temp;
}

//Read rainfall, high, then low for each month
//Uses the Month enum to step through the array
void getData(WeatherData year[]) {
    for (Month m = JANUARY; m <= DECEMBER; m = static_cast<Month>(m + 1)) {
        cin >> year[m].rainfall;
        year[m].highTemp = getTemp();
        year[m].lowTemp  = getTemp();
    }
}

//Calculate and store average temperature for each month
//Uses the Month enum to step through the array
void calcAverages(WeatherData year[]) {
    for (Month m = JANUARY; m <= DECEMBER; m = static_cast<Month>(m + 1)) {
        year[m].avgTemp = (year[m].highTemp + year[m].lowTemp) / 2.0f;
    }
}

//Calculate and display yearly weather statistics (exact CodeE format)
//Uses the Month enum to step through the array
void displayStats(const WeatherData year[]) {
    float totalRain = 0.0f;
    float totalAvgTemp = 0.0f;
    float highest = year[JANUARY].highTemp;
    float lowest = year[JANUARY].lowTemp;

    for (Month m = JANUARY; m <= DECEMBER; m = static_cast<Month>(m + 1)) {
        totalRain += year[m].rainfall;
        totalAvgTemp += year[m].avgTemp;

        if (year[m].highTemp > highest) {
            highest = year[m].highTemp;
        }
        if (year[m].lowTemp < lowest) {
            lowest = year[m].lowTemp;
        }
    }

    float avgRain = totalRain / NUM_MONTHS;
    float avgTemp = totalAvgTemp / NUM_MONTHS;

    //Exact CodeE expected summary - no space after the colon
    cout << "Average monthly rainfall:" << avgRain << endl;
    cout << "High Temp:" << highest << endl;
    cout << "Low Temp:" << lowest << endl;
    cout << "Average Temp:" << avgTemp << endl;
}

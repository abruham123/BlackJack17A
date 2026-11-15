/*
 * File:   WeatherStatistics.cpp
 * Author: Abraham Murillo
 * Weather Statistics - Gaddis Ch. 11
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Constants, Math, Physics, Conversions, Higher Dimensions

const int NUM_MONTHS = 12;

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

    //Get data for each month
    //Input format per line: MonthName Rainfall LowTemp HighTemp
    getData(year);

    //Calculate average temperature for each month
    calcAverages(year);

    //Display yearly statistics
    displayStats(year);

    //Exit stage right!
    return 0;
}

//Read a temperature; skip values outside [-100, 140]
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

//Read: MonthName Rainfall LowTemp HighTemp for each of 12 months
void getData(WeatherData year[]) {
    for (int i = 0; i < NUM_MONTHS; i++) {
        cin >> year[i].month;          // e.g. January
        cin >> year[i].rainfall;       // total rainfall
        year[i].lowTemp  = getTemp();  // low temperature first
        year[i].highTemp = getTemp();  // high temperature second
    }
}

//Calculate and store average temperature for each month
void calcAverages(WeatherData year[]) {
    for (int i = 0; i < NUM_MONTHS; i++) {
        year[i].avgTemp = (year[i].highTemp + year[i].lowTemp) / 2.0f;
    }
}

//Calculate and display yearly weather statistics (exact CodeE format)
void displayStats(const WeatherData year[]) {
    float totalRain = 0.0f;
    float totalAvgTemp = 0.0f;
    float highest = year[0].highTemp;
    float lowest = year[0].lowTemp;
    string highMonth = year[0].month;
    string lowMonth = year[0].month;

    for (int i = 0; i < NUM_MONTHS; i++) {
        totalRain += year[i].rainfall;
        totalAvgTemp += year[i].avgTemp;

        if (year[i].highTemp > highest) {
            highest = year[i].highTemp;
            highMonth = year[i].month;
        }
        if (year[i].lowTemp < lowest) {
            lowest = year[i].lowTemp;
            lowMonth = year[i].month;
        }
    }

    float avgRain = totalRain / NUM_MONTHS;
    float avgTemp = totalAvgTemp / NUM_MONTHS;

    //Exact CodeE expected output formatting
    cout << fixed << setprecision(1);
    cout << "Average Rainfall " << avgRain << " inches/month" << endl;

    //"Lowest  Temperature" has two spaces so it lines up with "Highest Temperature"
    cout << "Lowest  Temperature " << lowMonth << "  "
         << static_cast<int>(lowest) << " Degrees Fahrenheit" << endl;

    cout << "Highest Temperature " << highMonth << "  "
         << static_cast<int>(highest) << " Degrees Fahrenheit" << endl;

    cout << "Average Temperature for the year "
         << static_cast<int>(avgTemp + 0.5f) << " Degrees Fahrenheit" << endl;
}

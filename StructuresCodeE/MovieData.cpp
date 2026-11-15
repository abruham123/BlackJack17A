/*
 * File:   MovieData.cpp
 * Author: Abraham Murillo
 * Movie Data - Gaddis Ch. 11
 */

//System Libraries
#include <iostream>
#include <string>
using namespace std;

//User Libraries

//Global Constants - No Global Variables
//Only Universal Constants, Math, Physics, Conversions, Higher Dimensions

//Structure to store information about a movie
struct MovieData {
    string title;       // Title
    string director;    // Director
    int year;           // Year Released
    int runTime;        // Running Time (in minutes)
};

//Function Prototypes
void getMovie(MovieData &);
void displayMovie(const MovieData &);

//Execution Begins Here
int main(int argc, char** argv) {
    int numMovies;

    //Header prompts (exact CodeE format)
    cout << "This program reviews structures" << endl;
    cout << "Input how many movies, the Title of the Movie, "
         << "Director, Year Released, "
         << "and the Running Time in (minutes)." << endl;
    cout << endl;

    //Read the number of movies
    cin >> numMovies;
    cin.ignore();   //Clear newline before getline reads

    //Create an array of MovieData structures
    MovieData *movies = new MovieData[numMovies];

    //Store values in each structure, then display it
    for (int i = 0; i < numMovies; i++) {
        getMovie(movies[i]);
        displayMovie(movies[i]);
        //Blank line between movies only (not after the last)
        if (i < numMovies - 1) {
            cout << endl;
        }
    }

    //Free the allocated array
    delete[] movies;

    //Exit stage right!
    return 0;
}

//Read one movie's data into the structure
void getMovie(MovieData &movie) {
    getline(cin, movie.title);
    getline(cin, movie.director);
    cin >> movie.year;
    cin >> movie.runTime;
    cin.ignore();   //Clear newline for the next movie's getline
}

//Display one movie's information in a clearly formatted manner
//Labels padded so values start in the same column (width 11)
void displayMovie(const MovieData &movie) {
    cout << "Title:     " << movie.title << endl;
    cout << "Director:  " << movie.director << endl;
    cout << "Year:      " << movie.year << endl;
    cout << "Length:    " << movie.runTime << endl;
}

/*
Benedict Hofmockel
4/12/2022

If I had to operate blindly without Monty Hall (like this code): I picked the strateg of switching, because the statisical chance that you will pick a goat is 2/3, and you won't know what you picked.

If i was on "Let's make a deal": Considering my first choice doesn't really matter as I will be shown a goat by Monty anyway. SO the second choice matters... and I am a man of action and would switch. 


*/

#include <iostream>
#include <random>
#include <iterator>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

//Prototypes
bool isNumber(char argument[]);

using namespace std;

int main(int argc, char** argv){

    if (argc < 2){ 
        cout << "Usage: monty num_tests" << endl;
        return(0);
    }

    char *argument = argv[1];
    if (*argument == '-'){
        cerr << "Please, do not enter a negative number of Experiments, it is impossible to run negative number of Experiments!" << endl;
        exit(1);
    }
    for (int index = 0; argument[index] != 0; index++){
        if(!isdigit(argument[index])){
            cerr << "Number of Tests is not a number." << endl;
            exit(1);
        }
    }
    
    mt19937 mt(time(nullptr));
    int numExperiments = strtol(argument, NULL, 10);
    const char *objectsBehindDoors[3] = {"goat", "car", "goat"}; //I know I could do this more efficiently, but this one is for my sanity
    std::shuffle(objectsBehindDoors, objectsBehindDoors + sizeof(objectsBehindDoors) / sizeof(objectsBehindDoors[0]), mt);

    //Player picks a door at random
    int doorPlayerSelected = 0;
    
    int shouldSwitch = 0;
    int shouldStay = 0;

    for (int i = 0; i < numExperiments ; i++){
        doorPlayerSelected = mt() % 3;  //mt() % (max - min + 1) + min; //generates a random number between 0 and 2
        if ( strcmp(objectsBehindDoors[doorPlayerSelected], "car") == 0){ //if the player selected a door with a car behind it, he should obviously stick and stay with that choice
            shouldStay++;
        }
        else{ //the player picked a door with a goat; he should switch his choice 
            shouldSwitch++;
        }
    }
    float switchWins = ((float)shouldSwitch / (float)numExperiments) * 100;
    cout << "Switch would win " << switchWins << " percent of experiments." << endl;

    float stayWins = ((float)shouldStay / (float)numExperiments) * 100;
    cout << "Stay would win " << stayWins << " percent of experiments." << endl;

    return 0;
}

//Header file contains declaration of include libraries and functions for all display purposes
#ifndef VIEW_H
#define VIEW_H
#include <iostream>
#include <map>
#include "BitUtilities.h"

using namespace std;

class View {
    
private:
    //initialize constants
    static const char boardParts[2]; //Characters that are used in building the board
    static const char pieces[2]; //player pieces options    

public:
    static void View::displayBoard(unsigned int* board); //declare the function that displays the board state
    static void View::displayBoard(unsigned int* board, unsigned int moveBoard, map<int, char>& indexToChar); //declare the function that displays the moves available board
    static void pause(); //declare pause screen
    static void clear(); //declare clear screen
};

#endif
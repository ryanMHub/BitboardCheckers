//Header file contains declaration of include libraries and functions for all display purposes
#ifndef VIEW_H
#define VIEW_H
#include <iostream>
#include <map>
#include <string>
#include "BitUtilities.h"
#include "Types.h"

using namespace std;

class View {
    
private:
    //initialize constants
    static const char boardParts[2]; //Characters that are used in building the board
    static const char pieces[2]; //player pieces options    
    static const char kingPieces[2]; //player king options    
    static const string colorSequence[4]; //contains the const ansi escape codes for colors
    static void displayCurrentPlayer(PlayerCode);
    static string getColorCode(Color); //return the escape sequence for the color

public:
    static void View::displayPlayerMoves(PlayerCode, unsigned int* board, unsigned int* kings, unsigned int moveBoard, map<int, char>& indexToChar, int score[]); //declare the function that will display the players moves
    static void View::displayCurrentBoard(PlayerCode, unsigned int* board, unsigned int* kings, int score[]); //declare the function that displays the board state
    static void View::displayBoard(unsigned int* board, unsigned int* kings, unsigned int moveBoard, map<int, char>& indexToChar, int score[]); //declare the function that displays the board based on given parameters
    static void displayWinner(string player, string message); //displays the winner of the game and given message
    static void pause(); //declare pause screen
    static void clear(); //declare clear screen
};

#endif
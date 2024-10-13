#ifndef TYPES_H
#define TYPES_H
#include<vector>

//contains total piece value
const int TOTAL_PIECES = 12;

//Enum defines Player code for index selection
//This enum is used to switch between player state
enum PlayerCode : int {
    PLAYERONE = 0,
    PLAYERTWO = 1
};

//This enum is used to control boundary state
enum Border {
    TOP,       
    BOTTOM,
    LEFT,
    LEFT_BOTTOM,
    RIGHT,
    RIGHT_TOP,
    OPEN
};

//These enums values will represent the color code
enum Color {
    GREEN,
    BLUE,
    RED,
    DEFAULT
};

//this struct will store the coordinates of a move
struct Move {
    int start; //starting position of the move
    int end; //ending location of the move
    bool jump; //signifies that is a jump move over an opponent
    bool isKing; //signifies that the players piece is a king
    std::vector<int> opponent; //stores the index of all opponenets that are captured during the move

    //constructor for Move struct
    Move(int start, int end, bool jump, bool king, std::vector<int> opponent) : start(start), end(end), jump(jump), isKing(king), opponent(opponent) {}
};

#endif
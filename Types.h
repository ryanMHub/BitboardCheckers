#ifndef TYPES_H
#define TYPES_H
#include<vector>

//contains total piece value
const int TOTAL_PIECES = 24;

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
    RIGHT,
    OPEN
};

//this struct will store the coordinates of a move
struct Move {
    int start;
    int end;
    bool jump;
    std::vector<int> opponent;

    //constructor for Move struct
    Move(int start, int end, bool jump, std::vector<int> opponent) : start(start), end(end), jump(jump), opponent(opponent) {}
};

#endif
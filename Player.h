//Player Class establishes the main board, king board, and player code.
#ifndef PLAYER_H
#define PLAYER_H
#include "Types.h"

class Player {
private:
    unsigned int main; //main player board for regular pieces
    unsigned int king; //king player board for all kinged pieces
    PlayerCode playerCode; //playerCode is used for index selection

public:
    Player(PlayerCode code) : main(0), king(0), playerCode(code) {}

    //get the reference of the main player board
    unsigned int& getMain() { return main; }
    //return the reference of the king board
    unsigned int& getKing() { return king; }
    //return playerCode
    PlayerCode getCode() { return playerCode; } 
};

#endif

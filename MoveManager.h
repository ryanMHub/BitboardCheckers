#ifndef MOVEMANAGER_H
#define MOVEMANAGER_H

#include "Types.h"
#include "BitUtilities.h"
#include "View.h"
#include <vector>
#include <map>
#include <tuple>
#include <string>

class MoveManager {
private:
    //The distance that each checker can move based on the player, row, left or right piece is stored in this 3D
    const int steps[2][2][2] = { 
                                    {
                                        {4, 5}, 
                                        {3, 4} 
                                    },

                                    { 
                                        {-4, -3},
                                        {-5, -4}
                                    } 
                                };
    //private constructor for singleton
    MoveManager() {}
    //declare singleton instance
    static MoveManager* instance;

    //private move functions    
    void buildMovesMap(PlayerCode, unsigned int*, unsigned int*, unsigned int&, std::map<int, char>&, std::map<char, std::vector<Move>>&); //Responsible for building the data structures responsible for determining the moves available
    void updateMovesParameters(const std::vector<Move>&, unsigned int&, std::map<int, char>&, std::map<char, std::vector<Move>>&, char&); //Does the actual modification to these data structures
    std::tuple<char, int> getUserSelection(std::map<char, std::vector<Move>>&); //gets the move decision from the user
    void checkMove(PlayerCode, bool, Border, std::vector<Move>&, unsigned int*, unsigned int*, int, int); //checks if there is an available move at the given destination, If jump is avaiable create recursion map of the move graph
    void getJumpMoves(std::vector<Move>&, Move, PlayerCode, unsigned int*, unsigned int*,  int, int, unsigned int); //generate the move graph for the availbe jump moves from each location from a starting point
    std::tuple<bool, std::vector<int>> checkForOpponent(PlayerCode, bool, unsigned int, int); //check if there is an avaiable opponent within distance    
    bool registerOpponent(int, unsigned int, std::vector<int>&); //Adds an opponents index to the vector of relative opponents if there is one at the localPoint
    std::tuple<bool, int> checkJump(PlayerCode, unsigned int*, unsigned int*, int, int); //check if the jump was successful
    void executeMove(PlayerCode, unsigned int*, unsigned int*, Move); //Actually executes the select move plan   
    void swapPosition(PlayerCode, unsigned int*, Move);  //swaps the position of each position from start to dest
    void kingPiece(PlayerCode, unsigned int*, unsigned int*, int);//king the player's piece    
    Border checkBorder(int); //get the position of piece relative to border 
    std::string getCoor(int); //Return the xy location for the index of the piece that was passed to it
    
public:
    //Prevent copying of singleton
    MoveManager(const MoveManager&) = delete;
    MoveManager& operator=(const MoveManager&) = delete;

    //Static to access singleton
    static MoveManager* getInstance() {
        if(instance == nullptr){
            instance = new MoveManager();
        }
        return instance;
    }

    bool moveController(PlayerCode, unsigned int*, unsigned int*, int[]); //Declare the primary driver of the MoveManager singleton 

};

#endif
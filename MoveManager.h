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
    void buildMovesMap(PlayerCode, unsigned int*, unsigned int&, std::map<int, char>&, std::map<char, std::vector<Move>>&);
    void updateMovesParameters(const std::vector<Move>&, unsigned int&, std::map<int, char>&, std::map<char, std::vector<Move>>&, char&);
    std::tuple<char, int> getUserSelection(std::map<char, std::vector<Move>>&);
    void checkMove(PlayerCode, Border, std::vector<Move>&, unsigned int*, int, int);
    void getJumpMoves(std::vector<Move>&, Move, PlayerCode, unsigned int*, int, int);
    std::tuple<bool, std::vector<int>> checkForOpponent(PlayerCode, unsigned int*, int);
    std::tuple<bool, int> checkJump(PlayerCode, unsigned int*, int, int);
    void executeMove(PlayerCode, unsigned int*, Move);
    Border checkBorder(int); //get the position of piece relative to border
    std::string getCoor(int);
    
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

    void moveController(PlayerCode, unsigned int*); //Declare the primary driver of the MoveManager singleton 

};

#endif
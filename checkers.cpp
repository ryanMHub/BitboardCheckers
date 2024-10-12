#include "BitUtilities.h" //implements all of the bit manipulation methods
#include "Player.h" //Player Objects contains each players important data
#include "Types.h" //defines the shared type objects
#include "View.h" //contains all display functionality
#include "MoveManager.h" //controls all of the move processing

#include <iostream>
#include <map>
#include <vector>
#include <optional> //TODO: possibly not used
#include <tuple>

using namespace std;

//initialize functions
void initializeBoard(unsigned int*); //intitialize player boards

//game management
void gameController(unsigned int*);
std::tuple<int, int> checkScore(unsigned int*);

//Test Functions
void testMoveMapParts(unsigned int, std::map<int, char>&, std::map<char, vector<Move>>&);
void testSelection(std::tuple<char, int>);

unsigned int board[2]; //this array will store the boards for player 1 = board[0] and player 2 = board[1]

int main(){    
    gameController(board);
    return 0;    
}

//initialize the game boards of each player with the given array
void initializeBoard(unsigned int* board){
    board[0] = 0x00000FFF; //initialize player 1
    board[1] = 0xFF00F000; //initialize player 2
}

//This function is used to drive the control of the game
void gameController(unsigned int* board){
    Player playerOne = Player(PlayerCode::PLAYERONE);
    Player playerTwo = Player(PlayerCode::PLAYERTWO);
    board[playerOne.getCode()] = playerOne.getMain();
    board[playerTwo.getCode()] = playerTwo.getMain();
    MoveManager* moveManager = MoveManager::getInstance();
    initializeBoard(board); //initialize the the boards of each player using board array
    PlayerCode current = playerOne.getCode();
    bool running = true;
    int scores[] = {0, 0};

    while(running){
        View::displayBoard(board);
        View::pause();
        View::clear();
        moveManager->moveController(current, board);
        std::tie(scores[0], scores[1]) = checkScore(board);
        running = (scores[0] == TOTAL_PIECES || scores[1] == TOTAL_PIECES)?false:true;
        current = static_cast<PlayerCode>(BitUtilities::flipNumber(current));
    }
}

//return each players score by counting 
std::tuple<int, int> checkScore(unsigned int* board){
    int playerOne = BitUtilities::countBits(0, board);
    int playerTwo = BitUtilities::countBits(1, board);
    //TODO: add king check
    return std::make_tuple(playerOne, playerTwo);
}

//test selection function
void testSelection(std::tuple<char, int> selection){
    char row;
    int col;
    std::tie(row, col) = selection;
    cout << "Selected Coordinate: " << row << " " << col;
}
//Test if a parameters are functioning for the moveMapParameters
void testMoveMapParts(unsigned int movesBoard, std::map<int, char>& indexToChar, std::map<char, vector<Move>>& charToPiece){
    cout << movesBoard << endl;

    for(const auto& pair : indexToChar){
        cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
    }

    for(const auto& pair : charToPiece){
        cout << "Key: " << pair.first << endl;
        cout << "Moves: " << endl;

        for(const auto& move : pair.second){
            cout << " Start: " << move.start
                << " End: " << move.end
                << " Jump: " << (move.jump?"Yes":"No")
                << " Opponent: " << " Add Opponents" << endl;
        }
    }
}
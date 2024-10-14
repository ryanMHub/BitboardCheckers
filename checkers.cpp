#include "BitUtilities.h" //implements all of the bit manipulation methods
#include "Player.h" //Player Objects contains each players important data
#include "Types.h" //defines the shared type objects
#include "View.h" //contains all display functionality
#include "MoveManager.h" //controls all of the move processing

#include <iostream>
#include <map>
#include <vector>
#include <tuple>

using namespace std;

//initialize functions
void initializeBoard(unsigned int*); //intitialize player boards

//game management
void gameController(unsigned int*, unsigned int*); //main game control
void gameOver(int scores[], PlayerCode current); //builds the necessary strings to display at gameover
std::tuple<int, int> checkScore(unsigned int*, unsigned int*); //gets the score for each user

//Test Functions
void testMoveMapParts(unsigned int, std::map<int, char>&, std::map<char, vector<Move>>&);
void testSelection(std::tuple<char, int>);

//Hi!! My name is main
int main(){    
    unsigned int board[2]; //this array will store the boards for player 1 = board[0] and player 2 = board[1]
    unsigned int kings[2]; //this array will store the boards specifically for the pieces that have been kinged
    gameController(board, kings); //execute the game
    return 0;    
}

//initialize the game boards of each player with the given array
void initializeBoard(unsigned int* board){
    board[0] = 0x00000FFF; //initialize player 1
    board[1] = 0xFFF00000; //initialize player 2
}

//This function is used to drive the control of the game
void gameController(unsigned int* board, unsigned int* kings){
    //Initialize players using their player codes
    Player playerOne = Player(PlayerCode::PLAYERONE);
    Player playerTwo = Player(PlayerCode::PLAYERTWO);

    //Intialize the arrays that will store each players main board of pieces and their kings, which will be references
    board[playerOne.getCode()] = playerOne.getMain();
    board[playerTwo.getCode()] = playerTwo.getMain();
    kings[playerOne.getCode()] = playerOne.getKing();
    kings[playerTwo.getCode()] = playerTwo.getKing();

    //Initialize the moveManager singleton which will be used to process all of the required moves
    MoveManager* moveManager = MoveManager::getInstance();

    initializeBoard(board); //initialize the starting positions for each players main boards
    PlayerCode current = playerOne.getCode(); //initialize the starting player
    bool running = true; //initialize the state of the game
    int scores[] = {0, 0}; //start each player at a score of 0
     
    //loop while running is true, changes when a player collects all pieces, or player has no move.
    while(running){
        View::displayCurrentBoard(current, board, kings, scores); //display state of game before player moves
        View::pause();
        View::clear();
        running = moveManager->moveController(current, board, kings, scores); //execute the move process for current player, if false current player has no moves. Forfeit
        std::tie(scores[0], scores[1]) = checkScore(board, kings); //update the current scores for each player
        running &= (scores[0] == TOTAL_PIECES || scores[1] == TOTAL_PIECES)?false:true; //Check if there is a winner and update running 
        current = static_cast<PlayerCode>(BitUtilities::flipNumber(current)); //change to the next player by flip LSD in current playerCode
        View::clear();
    }

    gameOver(scores, current); //display to the user who won
    cout << '\n';
    View::pause();
}

//return each players score by counting 
std::tuple<int, int> checkScore(unsigned int* board, unsigned int* kings){
    //accumulate all of the on bits that each player has in each unsigned int
    int playerOne = BitUtilities::countBits(0, board); 
    playerOne += BitUtilities::countBits(0, kings);
    int playerTwo = BitUtilities::countBits(1, board);
    playerTwo += BitUtilities::countBits(1, kings);
    
    return std::make_tuple((TOTAL_PIECES-playerTwo), (TOTAL_PIECES-playerOne)); //return as a tuple once current of opposite player is deducted from Total pieces
}

//displays the winner of the game
void gameOver(int scores[], PlayerCode current){
    string player = "";
    string message = "";

    //If a player forfeited record the winner by the opposite of the current player
    if(scores[0] < TOTAL_PIECES && scores[1] < TOTAL_PIECES){
        player = (current == PLAYERONE)?"One":"Two";
        string loser = ((current == PLAYERONE)?"Two":"One");
        message = "                  Player " + loser +" Forfeits";
    } else {
        //check who has no more pieces and then declare winner
        player = (scores[0] == TOTAL_PIECES)?"One":"Two";
        string left = (scores[0] == TOTAL_PIECES)?std::to_string(TOTAL_PIECES-scores[1]):std::to_string(TOTAL_PIECES-scores[0]);
        message = "             Player " + player + " Has " + left + " Pieces Left";
    }
    View::displayWinner(player, message);
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
#include "BitUtilities.h" //implements all of the bit manipulation methods
#include <iostream>
#include <map>
#include <vector>
#include <optional> //TODO: possibly not used
#include <tuple>

using namespace std;

//This enum is used to switch between player state
enum Player {
    PLAYERONE,
    PLAYERTWO
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
    vector<int> opponent;

    //constructor for Move struct
    Move(int start, int end, bool jump, vector<int> opponent) : start(start), end(end), jump(jump), opponent(opponent) {}
};

//initialize functions
void initializeBoard(unsigned int*); //intitialize player boards
void displayBoard(unsigned int*); //display board state
void displayBoard(unsigned int*, unsigned int, map<int, char>&);

//game management utility functions
Border checkBorder(int); //get the position of piece relative to border
string getCoor(int);

//Move Mapping Functions
void moveController(Player, unsigned int*);
void buildMovesMap(Player, unsigned int*, unsigned int&, std::map<int, char>&, std::map<char, vector<Move>>&);
void updateMovesParameters(const std::vector<Move>&, unsigned int&, std::map<int, char>&, std::map<char, vector<Move>>&, char&);
std::tuple<char, int> getUserSelection(map<char, vector<Move>>&);
void checkMove(Player, Border, std::vector<Move>&, unsigned int*, int, int);
void getJumpMoves(std::vector<Move>&, Move, Player, unsigned int*, int, int);
tuple<bool, std::vector<int>> checkForOpponent(Player, unsigned int*, int);
std::tuple<bool, int> checkJump(Player, unsigned int*, int, int);
void executeMove(Player, unsigned int*, Move);

//game management
void gameController(unsigned int*);
std::tuple<int, int> checkScore(unsigned int*);
int countBits(int, unsigned int*);
void pause();
void clear();

//Test Functions
void testMoveMapParts(unsigned int, std::map<int, char>&, std::map<char, vector<Move>>&);
void testSelection(std::tuple<char, int>);

//initialize constants
const char boardParts[2] = {176, 178}; //Characters that are used in building the board
const char pieces[2] = {224,234}; //player pieces options
unsigned int board[2]; //this array will store the boards for player 1 = board[0] and player 2 = board[1]

//The distance that each checker can move based on the player, row, left or right piece is stored in this 3D
const int steps[2][2][2] = { 
                                { {4, 5}, {3, 4} },  
                                { {-4, -3}, {-5, -4}} 
                            };




int main(){   
    
    gameController(board);
    return 0;    
}

//initialize the game boards of each player with the given array
void initializeBoard(unsigned int* board){
    board[0] = 0x00000FFF; //initialize player 1
    board[1] = 0xFF00F000; //initialize player 2
}


//display board
void displayBoard(unsigned int* board){
    cout << "Player {insert variable here}\n";
    int row = 0;
    cout << " =====================";
    for(int i = 0 ; i < 32 ; i++){
        
        if(i%4 == 0){
            (i==0)?cout << "\n || ": cout << "||\n || "; //every 4 steps start a new line and print borders
            row++; //increment the row index every 4 steps
        }       

        if(BitUtilities::checkBit(board[0],i)) (row%2==1)?cout << boardParts[1] << " " << pieces[0] << " " : cout << pieces[0] << " " << boardParts[1] << " ";
        else if(BitUtilities::checkBit(board[1],i)) (row%2==1)?cout << boardParts[1] << " " << pieces[1] << " " : cout << pieces[1] << " " << boardParts[1] << " ";
        else (row%2==1)?cout << boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";
    }    
    cout << "||\n =====================" << endl;
}

//display board including the available moves
void displayBoard(unsigned int* board, unsigned int moveBoard, map<int, char>& indexToChar){
    cout << "Player {insert variable here}\n";
    int row = 0;
    char rowMarker = 'A';
    cout << "   ===0=1=2=3=4=5=6=7===" << endl;
    cout << "   =====================";
    for(int i = 0 ; i < 32 ; i++){
        
        if(i%4 == 0){
            (i==0)?cout << "\n A-|| ": cout << "||\n "<< rowMarker <<"-|| "; //every 4 steps start a new line and print borders
            rowMarker++;
            row++; //increment the row index every 4 steps
        }       

        if(BitUtilities::checkBit(board[0], i)) (row%2==1)?cout << boardParts[1] << " " << pieces[0] << " " : cout << pieces[0] << " " << boardParts[1] << " ";
        else if(BitUtilities::checkBit(board[1], i)) (row%2==1)?cout << boardParts[1] << " " << pieces[1] << " " : cout << pieces[1] << " " << boardParts[1] << " ";
        else if(BitUtilities::checkBit(moveBoard, i)) (row%2==1)?cout << boardParts[1] << " " << indexToChar[i] << " " : cout << indexToChar[i] << " " << boardParts[1] << " ";
        else (row%1==1)?cout << boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";
    }    
    cout << "||\n   =====================" << endl;
}

void moveController(Player current, unsigned int* board){
    //initialize int moves, map index to 'Char' increment using above idea, map 'Char' to set of struct move
    unsigned int movesBoard = 0;
    std::map<int, char> indexToChar;
    std::map<char, vector<Move>> charToPiece;
    
    //call function to get moves passing all above as reference.
    buildMovesMap(current, board, movesBoard, indexToChar, charToPiece);    

    //display moves with modified displayBoard
    displayBoard(board, movesBoard, indexToChar);

    char key;
    int index;

    //prompt user for selection
    std::tie(key, index) = getUserSelection(charToPiece);

    //make move on the board
    executeMove(current, board, charToPiece[key].at(index));
    
    //Do I need to destroy all of above after using
    displayBoard(board);

    //return selection
}

void gameController(unsigned int* board){
    const int TOTAL_PIECES = 24;
    initializeBoard(board); //initialize the the boards of each player using board array
    Player current = PLAYERONE;
    bool running = true;
    int scores[] = {0, 0};

    while(running){
        displayBoard(board);
        pause();
        clear();
        moveController(current, board);
        std::tie(scores[0], scores[1]) = checkScore(board);
        running = (scores[0] == TOTAL_PIECES || scores[1] == TOTAL_PIECES)?false:true;
        current = static_cast<Player>(BitUtilities::flipNumber(current));
    }
}

std::tuple<int, int> checkScore(unsigned int* board){
    int playerOne = countBits(0, board);
    int playerTwo = countBits(1, board);
    //TODO: add king check
    return std::make_tuple(playerOne, playerTwo);
}

int countBits(int player, unsigned int* board){
    int count = 0;
    for(int i = 0 ; i < 32 ; i++){
        count += (BitUtilities::checkBit(board[player], i))?1:0;
    }
    return count;
}


void pause(){
    cout << "Press Any Key..................";
    cin.get();
}

void clear(){
    cout << "\033[2J\033[H";
}

//test selection function
void testSelection(std::tuple<char, int> selection){
    char row;
    int col;
    std::tie(row, col) = selection;
    cout << "Selected Coordinate: " << row << " " << col;
}

//TODO: I need to come up with a way to test the jump tree and display of the opponents when displaying move options
//prompt user to select a letter of the board that has an open move. Then select the specific token to move. Returning a tuple containing the coordinates.
std::tuple<char, int> getUserSelection(map<char, vector<Move>>& charToPiece){
    string keyInput;

    while(true){
        cout << "\nEnter the letter of the desired move. _>>> ";
        cin >> keyInput;
        if(charToPiece.find(toupper(keyInput[0])) != charToPiece.end()){
            break;
        }
        cout << "\nNot a valid move. Try again! =>>> ";
    }

    string pieceInput;
    cout << "\nBelow is a list of pieces that can move to your selection.\n";
    int i = 0;
    for(auto& move : charToPiece[toupper(keyInput[0])]){
        cout << "\n" << i << " -- " << getCoor(move.start);
        if(!move.opponent.empty()) {
            cout << " Captured Opponents: ";
            for(auto& opp : move.opponent){
                cout << getCoor(opp) << " ";
            }
        }
        i++;
    }
    cout << "\n-------> ";

    while(true){
        cin >> pieceInput;
        if(!isdigit(pieceInput[0]) && (isdigit(pieceInput[0]) < 0 || isdigit(pieceInput[0]) >= charToPiece[toupper(keyInput[0])].size())){
            cout << "Invalid -------->";
        } else {
            break;
        }
    }
    
    return std::make_tuple(toupper(keyInput[0]), (pieceInput[0]-'0'));
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

//build all of the possible moves for player based on Player current
void buildMovesMap(Player current, unsigned int* board, unsigned int& movesBoard, std::map<int, char>& indexToChar, std::map<char, vector<Move>>& charToPiece) {
    char moveMarkerCounter = 'A';
    for(int i = 0 ; i < 32 ; i++){
        if(BitUtilities::checkBit(board[current], i) == 0) continue; //check if there is a piece at the position, if not continue to the next location     
        Border border = checkBorder(i); //check the position of the piece relative to the borders and store its state        
        std::vector<Move> moves; //TODO: Do I need to destroy Move Struct
                
        if(border == TOP && current == PLAYERTWO) {
            continue;
        } else if(border == BOTTOM && current == PLAYERONE){
            continue;
        } else if(border == LEFT || border == RIGHT){
            //cout << "I have been Called     " << i << endl;
            checkMove(current, border, moves, board, i, ((current == PLAYERONE)?(4+i):(-4+i)));            
        } else {
            //cout << "\nmove: " << steps[current][((i/4)%2)][0];
            checkMove(current, border, moves, board, i, (i + steps[current][((i/4)%2)][0])); //TODO: can I reduce some of these optional if statements, Is there a cleaner way to approach   
            //cout << "\nmove: " << steps[current][((i/4)%2)][1];                  
            checkMove(current, border, moves, board, i, (i + steps[current][((i/4)%2)][1]));            
        } 

        //call function that will map available moves if moves vector is not empty
        if(!moves.empty()){
            updateMovesParameters(moves, movesBoard, indexToChar, charToPiece, moveMarkerCounter);
        }    
    }    
}

//Update the MovesCollection values that will be used to control moves in game
void updateMovesParameters(const std::vector<Move>& moves, unsigned int& movesBoard, std::map<int, char>& indexToChar, std::map<char, vector<Move>>& charToPiece, char& moveMarker){    
    for(auto& move : moves){
        if(BitUtilities::checkBit(movesBoard, move.end)){
            charToPiece[indexToChar[move.end]].push_back(move);
        } else {
            movesBoard = BitUtilities::setBit(movesBoard, move.end);
            indexToChar[move.end] = moveMarker;
            charToPiece[moveMarker] = std::vector<Move>();
            charToPiece[moveMarker].push_back(move);
            moveMarker++;
        }
    }
}

//TODO: Maybe I don't need the pointer. Also I need to chop this up so that jump is seperate from checking for the move, or just send back a vector or pass the vector from caller. Drop the optional and switch to a void function that updates the moves at it goes
//check the specified move, determine if it is suitable move orif a jump is required and succesful
void checkMove(Player current, Border border, std::vector<Move>& moves, unsigned int* board, int i, int movePosition){
    if(BitUtilities::checkBit(board[current], movePosition)){        
        return ;
    } else if(BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], movePosition)){ //TODO: This may not work as I intend
        std::vector<int> opponents;
        getJumpMoves(moves, Move(i, -1, false, opponents), current, board, i, movePosition);
    } else {
        std::vector<int> opponents;
        moves.push_back(Move(i, movePosition, false, opponents));
    }   
}

//recursive function that builds the jump tree for each piece. Stores the individual Move struct for each jump in the moves vector<Move>.
void getJumpMoves(std::vector<Move>& moves, Move move, Player current, unsigned int* board, int i, int opponent){
    //if no open space or out or bounds return
    int dest;
    bool success;
    //cout << "\nPre jump " << i << " opponent: " << opponent;
    std::tie(success, dest) = checkJump(current, board, i, opponent);
    if(!success){
        //cout <<"\nindex = " << i << " opponent = " << opponent << " destination = " << dest;
        return;
    }

    //create the current Move
    //store the current Move
    //cout << "\n I succeded " << i << " dest: " << dest << " opp: " << opponent;
    move.end = dest;
    move.jump = true;
    move.opponent.push_back(opponent);
    moves.push_back(move);

    //get available opponents to capture
    std::vector<int> opponents;
    std::tie(success, opponents) = checkForOpponent(current, board, dest);
  
    //Cycle through available opponents using recursion
    if(success){
        for(auto& opp : opponents){
            cout << "\n" << i << " " << opp << " " << move.start;
            getJumpMoves(moves, move, current, board, dest, opp);
        }
    }   

}

//checks if there are available opponents for the piece to jump over. A success flag and a vector filled with the indexes of the available opponents is returned.
tuple<bool, std::vector<int>> checkForOpponent(Player current, unsigned int* board, int i) {
    bool success;
    std::vector<int> oppPositions;
    int locPoint = -1;
    Border border = checkBorder(i);

    if(border == TOP && current == PLAYERTWO) {
            success = false;
    } else if(border == BOTTOM && current == PLAYERONE){
        success = false;
    } else if(border == LEFT || border == RIGHT){
        locPoint = ((current == PLAYERONE)?(4+i):(-4+i));
        success = BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], locPoint);
        if(success) oppPositions.push_back(locPoint);           
    } else {
        locPoint = (i + steps[current][((i/4)%2)][0]);
        success = BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], locPoint);
        if(success) oppPositions.push_back(locPoint);
        
        locPoint = (i + steps[current][((i/4)%2)][1]);
        success = BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], locPoint);
        if(success) oppPositions.push_back(locPoint);
    } 

    return make_tuple(success, oppPositions);
}

//checks if the jump is successful returning a success flag and the index of the landing location in a tuple.
std::tuple<bool, int> checkJump(Player current, unsigned int* board, int i, int oppPosition){
    int dest = -1;
    bool success = true;
    Border border = checkBorder(i);
    int row = (i/4)%2;
        int diff = abs(oppPosition-i);

    if(border == LEFT){
        dest = (current == PLAYERONE)?(i+9):(i-7);        
    } else if( border == RIGHT){
        dest = (current == PLAYERONE)?(i+7):(i-9);        
    } else {        
        if(diff > 4) {
            dest = (current == PLAYERONE)?(i+9):(i-9);
        } else if(diff < 4) {
            dest = (current == PLAYERONE)?(i+7):(i-7);
        } else {
            dest = (current == PLAYERONE)?((row == 0)?(i+7):(i+9)):((row==0)?(i-9):(i-7));
            success &= (!(i%4 == 0 || i%4 == 3)); //TODO: NOt sure if works. prevents the edge cases that are not on the border but the adjacent side preventing attempt to jump over opponent out of bounds 
        }        
    }
    success &= (current == PLAYERONE && dest > 31)||(current == PLAYERTWO && dest < 0)?0:1;
    success &= (BitUtilities::checkBit(board[current], dest) == 0 && BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], dest) == 0)?1:0;
    return std::make_tuple(success, dest);
}

//flips all the required bit per move based on the Move struct that is passed to the func
void executeMove(Player current, unsigned int* board, Move move){
    board[current] = BitUtilities::flipBit(board[current], move.start);
    board[current] = BitUtilities::flipBit(board[current], move.end);
    if(move.jump){
        for(auto& opp : move.opponent){
            board[BitUtilities::flipNumber(current)] = BitUtilities::flipBit(board[BitUtilities::flipNumber(current)], opp);
        }
    }
}


//TODO: display board overloaded to display available moves to the user that are mapped to letters

//TODO: display all moves on the board for each player with letters mapped to integer location, use L or R if two pieces have a move.
//returns a text based version of the coordinate of the point of the bit on the checker board
string getCoor(int position){
    string coordinate;

    int row = position/4;
    int oddEven = row%2;
    
    char rowLetter = 65+row;
    coordinate.push_back(rowLetter);
    char col = ((oddEven)?((position%4)*2):((position%4)*2+1))+'0'; 
    coordinate.push_back(col);
    return coordinate;    
}

//return the state of the position of the current integer relative to the border
Border checkBorder(int position){
    if(position >= 0 && position <= 3) {        
        return (position%4==3)?RIGHT:TOP;
    } else if(position >= 28 && position <= 31){
        return (position%4==0)?LEFT:BOTTOM;
    } else if(position%4 == 0  && (position/4)%2 == 1) {
        return LEFT;
    } else if(position%4 == 3 && (position/4)%2 == 0) {
        return RIGHT;
    } else {
        return OPEN;
    }
}





#include <iostream>
#include <map>
#include <vector>
#include <optional>

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

//initialize functions
void initializeBoard(unsigned int*); //intitialize player boards
void displayBoard(unsigned int*); //display board state


Border checkBorder(int); //get the position of piece relative to border

//utility functions
int checkBit(unsigned int, int);


//initialize constants
const char boardParts[2] = {176, 178}; //Characters that are used in building the board
const char pieces[2] = {224,234}; //player pieces options
unsigned int board[2]; //this array will store the boards for player 1 = board[0] and player 2 = board[1]

//this struct will store the coordinates of a move
struct Move {
    int start;
    int end;
    bool jump;
    int opponent;

    //constructor for Move struct
    Move(int start, int end, bool jump, int opponent) : start(start), end(end), jump(jump), opponent(opponent) {}
};


int main(){
    Player currentPlayer = PLAYERONE;
    initializeBoard(board); //initialize the the boards of each player using board array
    displayBoard(board); //display the board using the board array for data of players location
    return 0;    
}

//initialize the game boards of each player with the given array
void initializeBoard(unsigned int* board){
    board[0] = 0x00000FFF; //initialize player 1
    board[1] = 0xFFF00000; //initialize player 2
}

//TODO: *************************************************************Allow players move as parameter, if null don't print moves. Store moves in integer like the pieces mapped to 'A' the characters can increment through an integer ch = 65; ch++
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

        if(board[0] & (1 << i)) (row%2==0)?cout << boardParts[1] << " " << pieces[0] << " " : cout << pieces[0] << " " << boardParts[1] << " ";
        else if(board[1] & (1 << i)) (row%2==0)?cout << boardParts[1] << " " << pieces[1] << " " : cout << pieces[1] << " " << boardParts[1] << " ";
        else (row%2==0)?cout << boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";
    }    
    cout << "||\n =====================" << endl;
}

void moveController(Player current){
    //initialize int moves, map index to 'Char' increment using above idea, map 'Char' to set of struct move
    unsigned int movesBoard = 0;
    std::map<int, char> indexToChar;
    std::map<char, vector<Move>> charToPiece;

    //call function to get moves passing all above as reference.

    //display moves with modified displayBoard

    //prompt user for selection


    //Do I need to destroy all of above after using

    //return selection
}

void buildMovesMap(Player current, unsigned int* board, unsigned int& movesBoard, std::map<int, char>& indexToChar, std::map<char, vector<Move>>& charToPiece) {

    for(int i = 0 ; i < 32 ; i++){
        if(checkBit(board[current], i) == 0) continue; //check if there is a piece at the position, if not continue to the next location
        
        Border border = checkBorder(i); //check the position of the piece relative to the borders and store its state
        int direction = (current == PLAYERONE)?1:-1; //allows maniputlation of the pieces moving direction based on player

        if(border == TOP && current == PLAYERTWO) {
            continue;
        } else if(border == BOTTOM && current == PLAYERONE){
            continue;
        } else if(border == LEFT || border == RIGHT){

        } else {

        }
        
    }
}

//TODO: display board overloaded to display available moves to the user that are mapped to letters

//TODO: display all moves on the board for each player with letters mapped to integer location, use L or R if two pieces have a move.


//build class for utilities
int checkBit(unsigned int num, int position){
    return (num & (1 << position));
}

//return the state of the position of the current integer relative to the border
Border checkBorder(int position){
    if(position >= 0 && position <= 3) {        
        return (position%4==3)?RIGHT:TOP;
    } else if(position >= 28 && position <= 31){
        return (position%4==0)?LEFT:BOTTOM;
    } else if(position%4 == 0) {
        return LEFT;
    } else if(position%4 == 3) {
        return RIGHT;
    } else {
        return OPEN;
    }
}

//TODO: Maybe I don't need the pointer
//check the specified move, determine if it is suitable move orif a jump is required and succesful
std::optional<Move> checkMove(Player current, Border border, unsigned int* board, int i, int movePosition){
    if(checkBit(board[current], movePosition)){
        return std::nullopt;
    } else if(checkBit(board[current&1], movePosition)){
        return checkJump(current, border, board, i, movePosition);
    }
    return Move(i, movePosition, false, -1);
}

//returns the move generated when jumping an opponent, nullopt when no move
std::optional<Move> checkJump(Player current, Border border, unsigned int* board, int i, int oppPosition){

    int dest = -1;

    if(border == LEFT){
        dest = (current == PLAYERONE)?(i+9):(i-7);        
    } else if( border == RIGHT){
        dest = (current == PLAYERONE)?(i+7):(i-9);        
    } else {
        int row = (i/4)%2;
        int diff = abs(oppPosition-i);
        if(diff > 4) {
            dest = i + (current == PLAYERONE)?(9):(-9);
        } else if(diff < 4) {
            dest = i + (current == PLAYERONE)?(7):(-7);
        } else {
            dest = i + ((current == PLAYERONE)?((row == 0)?7:9):((row==0)?-9:-7));
        }        
    }

    return (checkBit(board[current], dest)==0)?std::optional<Move>(Move(i, dest, true, oppPosition)):std::nullopt;
}



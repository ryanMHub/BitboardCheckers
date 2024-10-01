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

//this struct will store the coordinates of a move
struct Move {
    int start;
    int end;
    bool jump;
    int opponent;

    //constructor for Move struct
    Move(int start, int end, bool jump, int opponent) : start(start), end(end), jump(jump), opponent(opponent) {}
};

//initialize functions
void initializeBoard(unsigned int*); //intitialize player boards
void displayBoard(unsigned int*); //display board state
void displayBoard(unsigned int*, unsigned int, map<int, char>&);


Border checkBorder(int); //get the position of piece relative to border

//utility functions
int checkBit(unsigned int, int);
unsigned int setBit(unsigned int, int);

//Move Mapping Functions
void moveController(Player);
void buildMovesMap(Player, unsigned int*, unsigned int&, std::map<int, char>&, std::map<char, vector<Move>>&);
void updateMovesParameters(const std::vector<Move>&, unsigned int&, std::map<int, char>&, std::map<char, vector<Move>>&, char&);
std::optional<Move> checkMove(Player, Border, unsigned int*, int, int);
std::optional<Move> checkJump(Player, Border, unsigned int*, int, int);

//Test Functions
void testMoveMapParts(unsigned int, std::map<int, char>&, std::map<char, vector<Move>>&);


//initialize constants
const char boardParts[2] = {176, 178}; //Characters that are used in building the board
const char pieces[2] = {224,234}; //player pieces options
unsigned int board[2]; //this array will store the boards for player 1 = board[0] and player 2 = board[1]

//The distance that each checker can move based on the player, row, left or right piece is stored in this 3D
const int steps[2][2][2] = { { {4, 5}, {3, 4} },  { {-4, -3}, {-5, -4} } };




int main(){
    Player currentPlayer = PLAYERONE;
    initializeBoard(board); //initialize the the boards of each player using board array
    displayBoard(board); //display the board using the board array for data of players location
    moveController(currentPlayer);
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

        if(checkBit(board[0], i)) (row%2==0)?cout << boardParts[1] << " " << pieces[0] << " " : cout << pieces[0] << " " << boardParts[1] << " ";
        else if(checkBit(board[1], i)) (row%2==0)?cout << boardParts[1] << " " << pieces[1] << " " : cout << pieces[1] << " " << boardParts[1] << " ";
        else if(checkBit(moveBoard, i)) (row%2==0)?cout << boardParts[1] << " " << indexToChar[i] << " " : cout << indexToChar[i] << " " << boardParts[1] << " ";
        else (row%2==0)?cout << boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";
    }    
    cout << "||\n   =====================" << endl;
}

void moveController(Player current){
    //initialize int moves, map index to 'Char' increment using above idea, map 'Char' to set of struct move
    unsigned int movesBoard = 0;
    std::map<int, char> indexToChar;
    std::map<char, vector<Move>> charToPiece;

    //call function to get moves passing all above as reference.
    buildMovesMap(current, board, movesBoard, indexToChar, charToPiece);    

    //display moves with modified displayBoard
    displayBoard(board, movesBoard, indexToChar);

    //prompt user for selection


    //Do I need to destroy all of above after using

    //return selection
}

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
                << " Opponent: " << move.opponent << endl;
        }
    }
}

void buildMovesMap(Player current, unsigned int* board, unsigned int& movesBoard, std::map<int, char>& indexToChar, std::map<char, vector<Move>>& charToPiece) {
    char moveMarkerCounter = 'A';
    for(int i = 0 ; i < 32 ; i++){
        if(checkBit(board[current], i) == 0) continue; //check if there is a piece at the position, if not continue to the next location     
        Border border = checkBorder(i); //check the position of the piece relative to the borders and store its state        
        std::vector<Move> moves; //TODO: Do I need to destroy Move Struct
        std::optional<Move> move; //Store the move that is generated by checkMove
        
        if(border == TOP && current == PLAYERTWO) {
            continue;
        } else if(border == BOTTOM && current == PLAYERONE){
            continue;
        } else if(border == LEFT || border == RIGHT){
            //cout << "I have been Called     " << i << endl;
            move = checkMove(current, border, board, i, ((current == PLAYERONE)?(4+i):(-4+i)));
            if(move.has_value()){
                //cout << "This is the index added " << i << endl;
                moves.push_back(move.value());
            }
        } else {
            move = checkMove(current, border, board, i, (i + steps[current][((i/4)%2)][0])); //TODO: can I reduce some of these optional if statements, Is there a cleaner way to approach
            if(move.has_value()){
                moves.push_back(move.value());
            }
            move = checkMove(current, border, board, i, (i + steps[current][((i/4)%2)][1]));
            if(move.has_value()){
                moves.push_back(move.value());
            }
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
        if(checkBit(movesBoard, move.end)){
            charToPiece[indexToChar[move.end]].push_back(move);
        } else {
            movesBoard = setBit(movesBoard, move.end);
            indexToChar[move.end] = moveMarker;
            charToPiece[moveMarker] = std::vector<Move>();
            charToPiece[moveMarker].push_back(move);
            moveMarker++;
        }
    }
}

//TODO: Maybe I don't need the pointer
//check the specified move, determine if it is suitable move orif a jump is required and succesful
std::optional<Move> checkMove(Player current, Border border, unsigned int* board, int i, int movePosition){
    if(checkBit(board[current], movePosition)){
        //cout << "The move is null " << i << " " << movePosition << endl;
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



//TODO: display board overloaded to display available moves to the user that are mapped to letters

//TODO: display all moves on the board for each player with letters mapped to integer location, use L or R if two pieces have a move.


//build class for utilities
//check value of bit return 1 or 0
int checkBit(unsigned int num, int position){
    return (num & (1 << position));
}

//set bit at given location to 1
unsigned int setBit(unsigned int value, int position){
    return value | (1 << position);
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





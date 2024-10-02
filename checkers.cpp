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

//utility functions
int checkBit(unsigned int, int);
unsigned int setBit(unsigned int, int);
int flipBit(unsigned int);

//Move Mapping Functions
void moveController(Player);
void buildMovesMap(Player, unsigned int*, unsigned int&, std::map<int, char>&, std::map<char, vector<Move>>&);
void updateMovesParameters(const std::vector<Move>&, unsigned int&, std::map<int, char>&, std::map<char, vector<Move>>&, char&);
std::tuple<char, int> getUserSelection(map<char, vector<Move>>&);
void checkMove(Player, Border, std::vector<Move>, unsigned int*, int, int);
void getJumpMoves(vector<Move>, Move, Player, unsigned int*, int, int);
tuple<bool, std::vector<int>> checkForOpponent(Player, unsigned int*, int);
std::tuple<bool, int> checkJump(Player, unsigned int*, int, int);
//std::optional<Move> checkJump(Player, Border, unsigned int*, int, int);

//Test Functions
void testMoveMapParts(unsigned int, std::map<int, char>&, std::map<char, vector<Move>>&);
void testSelection(std::tuple<char, int>);

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

        if(checkBit(board[0],i)) (row%2==1)?cout << boardParts[1] << " " << pieces[0] << " " : cout << pieces[0] << " " << boardParts[1] << " ";
        else if(checkBit(board[1],i)) (row%2==1)?cout << boardParts[1] << " " << pieces[1] << " " : cout << pieces[1] << " " << boardParts[1] << " ";
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

        if(checkBit(board[0], i)) (row%2==1)?cout << boardParts[1] << " " << pieces[0] << " " : cout << pieces[0] << " " << boardParts[1] << " ";
        else if(checkBit(board[1], i)) (row%2==1)?cout << boardParts[1] << " " << pieces[1] << " " : cout << pieces[1] << " " << boardParts[1] << " ";
        else if(checkBit(moveBoard, i)) (row%2==1)?cout << boardParts[1] << " " << indexToChar[i] << " " : cout << indexToChar[i] << " " << boardParts[1] << " ";
        else (row%1==0)?cout << boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";
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
    std::tuple<char, int> selection = getUserSelection(charToPiece);
    
    //Do I need to destroy all of above after using

    //return selection
}

//test selection function
void testSelection(std::tuple<char, int> selection){
    char row;
    int col;
    std::tie(row, col) = selection;
    cout << "Selected Coordinate: " << row << " " << col;
}

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
            checkMove(current, border, moves, board, i, ((current == PLAYERONE)?(4+i):(-4+i)));            
        } else {
            checkMove(current, border, moves, board, i, (i + steps[current][((i/4)%2)][0])); //TODO: can I reduce some of these optional if statements, Is there a cleaner way to approach            
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

//TODO: Maybe I don't need the pointer. Also I need to chop this up so that jump is seperate from checking for the move, or just send back a vector or pass the vector from caller. Drop the optional and switch to a void function that updates the moves at it goes
//check the specified move, determine if it is suitable move orif a jump is required and succesful
void checkMove(Player current, Border border, std::vector<Move> moves, unsigned int* board, int i, int movePosition){
    if(checkBit(board[current], movePosition)){        
        return ;
    } else if(checkBit(board[flipBit(current)], movePosition)){ //TODO: This may not work as I intend
        std::vector<int> opponents;
        getJumpMoves(moves, Move(i, -1, false, opponents), current, board, i, movePosition);
    } else {
        std::vector<int> opponents;
        moves.push_back(Move(i, movePosition, false, opponents));
    }   
}

void getJumpMoves(vector<Move> moves, Move move, Player current, unsigned int* board, int i, int opponent){
    //if no open space or out or bounds return
    int dest;
    bool success;
    std::tie(success, dest) = checkJump(current, board, i, opponent);
    if(!success){
        return;
    }

    //create the current Move
    //store the current Move
    move.end = dest;
    move.jump = true;
    move.opponent.push_back(opponent);
    moves.push_back(move);

    //get available opponents to capture
    std::vector<int> opponents;
    std::tie(success, opponents) = checkForOpponent(current, board, i);
  
    //Cycle through available opponents using recursion
    if(success){
        for(auto& opp : opponents){
            getJumpMoves(moves, move, current, board, dest, opp);
        }
    }   

}

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
        success = checkBit(board[flipBit(current)], locPoint);
        if(success) oppPositions.push_back(locPoint);           
    } else {
        locPoint = (i + steps[current][((i/4)%2)][0]);
        success = checkBit(board[flipBit(current)], locPoint);
        if(success) oppPositions.push_back(locPoint);
        
        locPoint = (i + steps[current][((i/4)%2)][1]);
        success = checkBit(board[flipBit(current)], locPoint);
        if(success) oppPositions.push_back(locPoint);
    } 

    return make_tuple(success, oppPositions);
}

std::tuple<bool, int> checkJump(Player current, unsigned int* board, int i, int oppPosition){
    int dest = -1;
    bool success = false;
    Border border = checkBorder(i);

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
    success = (current == PLAYERONE && dest > 31)||(current == PLAYERTWO && dest < 0)?0:1;
    success &= (checkBit(board[current], dest) == 0 && checkBit(board[flipBit(current)], dest) == 0)?1:0;
    return std::make_tuple(success, dest);
}

// //returns the move generated when jumping an opponent, nullopt when no move
// std::optional<Move> checkJump(Player current, Border border, unsigned int* board, int i, int oppPosition){

//     int dest = -1;

//     if(border == LEFT){
//         dest = (current == PLAYERONE)?(i+9):(i-7);        
//     } else if( border == RIGHT){
//         dest = (current == PLAYERONE)?(i+7):(i-9);        
//     } else {
//         int row = (i/4)%2;
//         int diff = abs(oppPosition-i);
//         if(diff > 4) {
//             dest = i + (current == PLAYERONE)?(9):(-9);
//         } else if(diff < 4) {
//             dest = i + (current == PLAYERONE)?(7):(-7);
//         } else {
//             dest = i + ((current == PLAYERONE)?((row == 0)?7:9):((row==0)?-9:-7));
//         }        
//     }

//     return (checkBit(board[current], dest)==0)?std::optional<Move>(Move(i, dest, true, oppPosition)):std::nullopt;
// }



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

//return flipped bit value
int flipBit(unsigned int value){
    return value^1;
}

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





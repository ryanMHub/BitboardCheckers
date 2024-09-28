#include <iostream>
using namespace std;

//initialize functions
void initializeBoard(unsigned int*);
void displayBoard(unsigned int*);

enum Player {
    PlayerOne,
    PlayerTwo
};

const char boardParts[2] = {176, 178}; //Characters that are used in building the board
const char pieces[2] = {224,234}; //player pieces options
unsigned int board[2]; //this array will store the boards for player 1 = board[0] and player 2 = board[1]


int main(){
    Player currentPlayer = PlayerOne;
    initializeBoard(board); //initialize the the boards of each player using board array
    displayBoard(board); //display the board using the board array for data of players location
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

        if(board[0] & (1 << i)) (row%2==0)?cout << boardParts[1] << " " << pieces[0] << " " : cout << pieces[0] << " " << boardParts[1] << " ";
        else if(board[1] & (1 << i)) (row%2==0)?cout << boardParts[1] << " " << pieces[1] << " " : cout << pieces[1] << " " << boardParts[1] << " ";
        else (row%2==0)?cout << boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";
    }    
    cout << "||\n =====================" << endl;
}

//display board overloaded to display available moves to the user that are mapped to letters

//display all moves on the board for each player with letters mapped to integer location, use L or R if two pieces have a move.



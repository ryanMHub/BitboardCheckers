#include <iostream>
using namespace std;

//initialize functions
void initializeBoard(unsigned int*);
void displayBoard(unsigned int*);

unsigned int board[2]; //this array will store the boards for player 1 = board[0] and player 2 = board[1]

int main(){
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

    for(int i = 0 ; i < 32 ; i++){
        if(i%4 == 0){
            cout << "\n"; //every 8 steps start a new line
            row++;
        }       

        if(board[0] & (1 << i)) (row%2==0)?cout << "X P1 ": cout <<"P1 X ";
        else if(board[1] & (1 << i)) (row%2==0)?cout << "X P2 ": cout <<"P2 X ";
        else (row%2==0)?cout << "X  . ": cout <<".  X ";;
    }
    cout << "\n";
}

//display board overloaded to display available moves to the user that are mapped to letters



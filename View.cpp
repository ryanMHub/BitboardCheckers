#include "View.h"

//initialize constants
const char View::boardParts[2] = {176, 178}; //Characters that are used in building the board
const char View::pieces[2] = {224,234}; //player pieces options

//display board
void View::displayBoard(unsigned int* board){
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
        else (row%2==1)?cout << View::boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";
    }    
    cout << "||\n =====================" << endl;
}

//display board including the available moves
void View::displayBoard(unsigned int* board, unsigned int moveBoard, map<int, char>& indexToChar){
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

//pause the screen and wait for user to press a key
void View::pause(){
    cout << "Press Any Key..................";
    cin.get();
}

void View::clear(){
    cout << "\033[2J\033[H";
}
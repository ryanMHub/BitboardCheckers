#include "View.h"

//initialize constants
const char View::boardParts[2] = {static_cast<char>(176), static_cast<char>(178)}; //Characters that are used in building the board
const char View::pieces[2] = {static_cast<char>(224), static_cast<char>(234)}; //player pieces options
const char View::kingPieces[2]= {static_cast<char>(157), static_cast<char>(228)}; //player pieces for kings
const string View::colorSequence[4] = {"\033[32m", "\033[34m", "\033[31m", "\033[39m"}; //contains the const ansi escape codes for colors

//display board
void View::displayCurrentBoard(PlayerCode current, unsigned int* board, unsigned int* kings, int score[]){
    displayCurrentPlayer(current); //displays what players turn it is
    map<int, char> empty; //sentinal map 
    View::displayBoard(board, kings, 0, empty, score); //display the current state of the game
}

//display board including the available moves
void View::displayPlayerMoves(PlayerCode current, unsigned int* board, unsigned int* kings, unsigned int moveBoard, map<int, char>& indexToChar, int score[]){
    displayCurrentPlayer(current); //display current player
    displayBoard(board, kings, moveBoard, indexToChar, score); //displays the current state of the game with the provided moves presented
}

//display current player
void View::displayCurrentPlayer(PlayerCode current){
    string player = (current == PLAYERONE)?"One":"Two";
    string playPiece = (current == PLAYERONE)?"{ " + std::string(1,pieces[0]) + " , " + std::string(1, kingPieces[0]) + " }":"{ " + std::string(1,pieces[1]) + " , " + std::string(1, kingPieces[1]) + " }";
    Color currColor = (current == PLAYERONE)?GREEN:BLUE;
    cout << "<_______________________Player " << player << " " << getColorCode(currColor) << playPiece << getColorCode(DEFAULT) << " Turn___________________>\n";
}

//print the board
void View::displayBoard(unsigned int* board, unsigned int* kings, unsigned int moveBoard, map<int, char>& indexToChar, int score[]) {
    int row = 0; //current row
    char rowMarker = 'A'; //marker increments after each new row
    //both of these strings are used to display the score
    string playerLabel = "             Player One      Player Two";
    string scoreStr = "                  " + std::to_string(score[0]) + "               " + std::to_string(score[1]);
    cout << "  ===0=1=2=3=4=5=6=7===" << endl; //used as a coordinate system
    cout << "  =====================";
    for(int i = 0 ; i < 32 ; i++){
        //creates the border to the game
        if(i%4 == 0){
            (i==0)?cout << "\nA-|| ": cout << "||"<<((i==8)?playerLabel:"") << ((i==16)?scoreStr:"") << "\n" << rowMarker <<"-|| "; //every 4 steps start a new line and print borders
            rowMarker++;
            row++; //increment the row index every 4 steps
        }       

        //which ever on of these unsigned integers has a 1 for the current index display its value, everything else is just used for display
        if(BitUtilities::checkBit(board[0], i)) (row%2==1)?cout << boardParts[1] << " " << getColorCode(GREEN) << pieces[0] << getColorCode(DEFAULT) << " " : cout << getColorCode(GREEN) << pieces[0] << getColorCode(DEFAULT) << " " << boardParts[1] << " ";
        else if(BitUtilities::checkBit(kings[0],i)) (row%2==1)?cout << boardParts[1] << " " << getColorCode(GREEN) << kingPieces[0] << getColorCode(DEFAULT) << " " : cout << getColorCode(GREEN) << kingPieces[0] << getColorCode(DEFAULT) << " " << boardParts[1] << " ";
        else if(BitUtilities::checkBit(board[1], i)) (row%2==1)?cout << boardParts[1] << " " << getColorCode(BLUE) << pieces[1] << getColorCode(DEFAULT) << " " : cout << getColorCode(BLUE) << pieces[1] << getColorCode(DEFAULT) << " " << boardParts[1] << " ";
        else if(BitUtilities::checkBit(kings[1],i)) (row%2==1)?cout << boardParts[1] << " " << getColorCode(BLUE) << kingPieces[1] << getColorCode(DEFAULT) << " " : cout << getColorCode(BLUE) << kingPieces[1] << getColorCode(DEFAULT) << " " << boardParts[1] << " ";
        else if(!indexToChar.empty() && BitUtilities::checkBit(moveBoard, i)) (row%2==1)?cout << boardParts[1] << " " << getColorCode(RED) << indexToChar[i] << getColorCode(DEFAULT) << " " : cout << getColorCode(RED) << indexToChar[i] << getColorCode(DEFAULT) << " " << boardParts[1] << " ";
        else (row%2==1)?cout << boardParts[1] << " " << boardParts[0] << " ": cout << boardParts[0] << " " << boardParts[1] << " ";        
    }    
    cout << "||\n  =====================" << endl;
}

//displays winner based on given values
void View::displayWinner(string player, string message){
    cout << "******************* Player " << player << " Wins ***********************\n";
    cout << message;
}

//returns the escape sequence for each color based on its enum code and location in the colorSequence array
string View::getColorCode(Color color){
    switch(color){
        case GREEN: return colorSequence[GREEN];
            break;
        case BLUE: return colorSequence[BLUE];
            break;
        case RED: return colorSequence[RED];
            break;
        case DEFAULT: return colorSequence[DEFAULT];
        default:
            return colorSequence[DEFAULT];
    }
}

//pause the screen and wait for user to press a key
void View::pause(){    
    cout << "Press Any Key..................";
    cin.get();
}

//clear screen
void View::clear(){
    cout << "\033[2J\033[H";
}
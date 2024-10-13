#include "MoveManager.h"

//Initialize the singleton instance
MoveManager* MoveManager::instance = nullptr;

//execute all of the required functionality for the move process
bool MoveManager::moveController(PlayerCode current, unsigned int* board, unsigned int* kings, int score[]){
    //initialize int moves, map index to 'Char' increment using above idea, map 'Char' to set of struct move
    unsigned int movesBoard = 0;
    std::map<int, char> indexToChar;
    std::map<char, std::vector<Move>> charToPiece;
    
    //call function to get moves passing all above as reference.
    buildMovesMap(current, board, kings, movesBoard, indexToChar, charToPiece);    

    //if there are no available moves player loses return false for unsuccessful move
    if(movesBoard == 0) return false;

    //display moves with modified displayBoard
    View::displayPlayerMoves(current, board, kings, movesBoard, indexToChar, score);

    char key;
    int index;

    //prompt user for selection
    std::tie(key, index) = getUserSelection(charToPiece);

    //make move on the board
    executeMove(current, board, kings, charToPiece[key].at(index));   
    
    //move was a success
    return true;    
}

//build all of the possible moves for player based on Player current
void MoveManager::buildMovesMap(PlayerCode current, unsigned int* board, unsigned int* kings, unsigned int& movesBoard, std::map<int, char>& indexToChar, std::map<char, vector<Move>>& charToPiece) {
    char moveMarkerCounter = 'A';
    for(int i = 0 ; i < 32 ; i++){
        if(BitUtilities::checkBit(board[current], i) == 0 && BitUtilities::checkBit(kings[current], i) == 0) continue; //check if there is a piece at the position, if not continue to the next location     
        
        Border border = checkBorder(i); //check the position of the piece relative to the borders and store its state        
        std::vector<Move> moves; //TODO: Do I need to destroy Move Struct
        bool isKing = BitUtilities::checkBit(kings[current], i); //check if the current piece is a king
        
        //Determine all directions that the current piece can move based on its location      
        if(border == TOP || border == RIGHT_TOP) {            
            if(border == TOP){
                checkMove(current, isKing, border, moves, board, kings, i, (i + steps[0][0][0]));                                   
                checkMove(current, isKing, border, moves, board, kings, i, (i + steps[0][0][1]));
            } else {
                checkMove(current, isKing, border, moves, board, kings, i, (4+i));
            }
        } else if((border == BOTTOM || border == LEFT_BOTTOM)){
            if(border == BOTTOM){
                checkMove(current, isKing, border, moves, board, kings, i, (i + steps[1][1][0]));                                   
                checkMove(current, isKing, border, moves, board, kings, i, (i + steps[1][1][1]));
            } else {
                checkMove(current, isKing, border, moves, board, kings, i, (-4+i));
            }
        } else if(border == LEFT  || border == RIGHT){            
            checkMove(current, isKing, border, moves, board, kings, i, ((current == PLAYERONE)?(4+i):(-4+i)));
            if(isKing) checkMove(current, isKing, border, moves, board, kings, i, ((current == PLAYERTWO)?(4+i):(-4+i)));
        } else {            
            checkMove(current, isKing, border, moves, board, kings, i, (i + steps[current][((i/4)%2)][0]));
            checkMove(current, isKing, border, moves, board, kings, i, (i + steps[current][((i/4)%2)][1]));
            if(isKing) {
                checkMove(current, isKing, border, moves, board, kings, i, (i + steps[BitUtilities::flipNumber(current)][((i/4)%2)][0]));
                checkMove(current, isKing, border, moves, board, kings, i, (i + steps[BitUtilities::flipNumber(current)][((i/4)%2)][1]));
            }
        } 

        //call function that will map available moves if moves vector is not empty
        if(!moves.empty()){
            updateMovesParameters(moves, movesBoard, indexToChar, charToPiece, moveMarkerCounter);
        }    
    }    
}

//Update the MovesCollection values that will be used to control moves in game
void MoveManager::updateMovesParameters(const std::vector<Move>& moves, unsigned int& movesBoard, std::map<int, char>& indexToChar, std::map<char, vector<Move>>& charToPiece, char& moveMarker){    
    for(auto& move : moves){
        //if the current moves destination has already been determined for a move previously, add that specific move to the key for that index associated with a Letter
        if(BitUtilities::checkBit(movesBoard, move.end)){
            charToPiece[indexToChar[move.end]].push_back(move);
        } else {
            //else establish this index as a potential move destination, by initializing all collection parameters associated with that index.
            movesBoard = BitUtilities::setBit(movesBoard, move.end);
            indexToChar[move.end] = moveMarker; //link the current destination index to a letter
            charToPiece[moveMarker] = std::vector<Move>(); //initialize the vector to store all moves that destination is at the given index
            charToPiece[moveMarker].push_back(move); //add the current move to the collection by Letter
            moveMarker++; //Advance the next key value
        }
    }
}

//TODO: Maybe I don't need the pointer. Also I need to chop this up so that jump is seperate from checking for the move, or just send back a vector or pass the vector from caller. Drop the optional and switch to a void function that updates the moves at it goes
//check the specified move, determine if it is suitable move orif a jump is required and succesful
void MoveManager::checkMove(PlayerCode current, bool isKing, Border border, std::vector<Move>& moves, unsigned int* board, unsigned int* kings, int i, int movePosition){
    //if there is already the currents player's piece at the destination return
    if(BitUtilities::checkBit(board[current], movePosition) || BitUtilities::checkBit(kings[current], movePosition)){        
        return ;
    } else if(BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], movePosition) || BitUtilities::checkBit(kings[BitUtilities::flipNumber(current)], movePosition)){ //if there is a main or king piece of the opponent, execute jump sequencing
        std::vector<int> opponents;
        getJumpMoves(moves, Move(i, -1, false, isKing, opponents), current, board, kings, i, movePosition, BitUtilities::mergeBits(board[BitUtilities::flipNumber(current)], kings[BitUtilities::flipNumber(current)]));
    } else {
        //else just log a normal move
        std::vector<int> opponents;
        moves.push_back(Move(i, movePosition, false, isKing, opponents));
    }   
}

//recursive function that builds the jump tree for each piece. Stores the individual Move struct for each jump in the moves vector<Move>.
void MoveManager::getJumpMoves(std::vector<Move>& moves, Move move, PlayerCode current, unsigned int* board, unsigned int* kings, int i, int opponent, unsigned int currOpps){
    //if no open space or out or bounds return
    int dest;
    bool success;    
    std::tie(success, dest) = checkJump(current, board, kings, i, opponent);
    if(!success){        
        return;
    }

    //create the current Move
    //store the current Move    
    move.end = dest;
    move.jump = true;
    move.opponent.push_back(opponent);
    moves.push_back(move);

    //update currOpps map
    currOpps = BitUtilities::flipBit(currOpps, opponent);

    //get available opponents to capture
    std::vector<int> opponents;
    std::tie(success, opponents) = checkForOpponent(current, move.isKing, currOpps, dest);
  
    //Cycle through available opponents using recursion
    if(success){
        for(auto& opp : opponents){            
            getJumpMoves(moves, move, current, board, kings, dest, opp, currOpps);
        }
    }   

}

//checks if there are available opponents for the piece to jump over. A success flag and a vector filled with the indexes of the available opponents is returned.
tuple<bool, std::vector<int>> MoveManager::checkForOpponent(PlayerCode current, bool isKing, unsigned int oppMap, int i) {
    bool success = false;
    std::vector<int> oppPositions;
    int locPoint = -1;
    Border border = checkBorder(i);

    if(border == TOP || border == RIGHT_TOP) {
        if(border == TOP){
            success |= MoveManager::registerOpponent((i + steps[0][0][0]), oppMap, oppPositions);
            success |= MoveManager::registerOpponent((i + steps[0][0][1]), oppMap, oppPositions);            
        } else {
            success |= MoveManager::registerOpponent((4+i), oppMap, oppPositions);               
        }        
    } else if((border == BOTTOM || border == LEFT_BOTTOM)) {
        if(border == BOTTOM){
            success |= MoveManager::registerOpponent((i + steps[1][1][0]), oppMap, oppPositions);
            success |= MoveManager::registerOpponent((i + steps[1][1][1]), oppMap, oppPositions); 
        } else {
            success |= MoveManager::registerOpponent((-4+i), oppMap, oppPositions);
        }
    } else if(border == LEFT || border == RIGHT){
        success |= MoveManager::registerOpponent(((current == PLAYERONE)?(4+i):(-4+i)), oppMap, oppPositions);
        if(isKing) success |= MoveManager::registerOpponent(((current == PLAYERTWO)?(4+i):(-4+i)), oppMap, oppPositions);                   
    } else {
        success |= MoveManager::registerOpponent((i + steps[current][((i/4)%2)][0]), oppMap, oppPositions);
        success |= MoveManager::registerOpponent((i + steps[current][((i/4)%2)][1]), oppMap, oppPositions);
        if(isKing){
            success |= MoveManager::registerOpponent((i + steps[BitUtilities::flipNumber(current)][((i/4)%2)][0]), oppMap, oppPositions);
            success |= MoveManager::registerOpponent((i + steps[BitUtilities::flipNumber(current)][((i/4)%2)][1]), oppMap, oppPositions);
        }
    }    

    return make_tuple(success, oppPositions);
}

//Adds an opponents index to the vector of relative opponents if there is one at the localPoint
bool MoveManager::registerOpponent(int localPoint, unsigned int oppMap, std::vector<int>& oppPositions){
    bool success = BitUtilities::checkBit(oppMap, localPoint);
    if(success) oppPositions.push_back(localPoint);
    return success;
}

//checks if the jump is successful returning a success flag and the index of the landing location in a tuple.
std::tuple<bool, int> MoveManager::checkJump(PlayerCode current, unsigned int* board, unsigned int* kings, int i, int oppPosition){
    int dest = -1;
    bool success = true;
    Border border = checkBorder(i);
    int row = (i/4)%2;
    int diff = abs(oppPosition-i);

    if(border == LEFT){
        dest = (i<oppPosition)?(i+9):(i-7);        
    } else if( border == RIGHT){
        dest = (i<oppPosition)?(i+7):(i-9);        
    } else {        
        if(diff > 4) {
            dest = (i<oppPosition)?(i+9):(i-9);
        } else if(diff < 4) {
            dest = (i<oppPosition)?(i+7):(i-7);
        } else {
            dest = (i<oppPosition)?((row == 0)?(i+7):(i+9)):((row==0)?(i-9):(i-7));
            success &= (!(i%4 == 0 || i%4 == 3)); //TODO: NOt sure if works. prevents the edge cases that are not on the border but the adjacent side preventing attempt to jump over opponent out of bounds 
        }        
    }
    //determine if dest is in bounds
    success &= (dest > 31)||(dest < 0)?0:1;
    //determine if dest is open
    success &= (BitUtilities::checkBit(board[current], dest) == 0 
            && BitUtilities::checkBit(kings[current], dest) == 0 
            && BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], dest) == 0 
            && BitUtilities::checkBit(kings[BitUtilities::flipNumber(current)], dest) == 0)?1:0;
    //return results
    return std::make_tuple(success, dest);
}

//flips all the required bit per move based on the Move struct that is passed to the func
void MoveManager::executeMove(PlayerCode current, unsigned int* board, unsigned int* kings, Move move){
    //determines if the player's piece needs to flip the kings board or main board
    if(move.isKing) swapPosition(current, kings, move);
    else swapPosition(current, board, move);

    //flip all opponents bits that are captured during the move
    if(move.jump){
        for(auto& opp : move.opponent){
            if(BitUtilities::checkBit(board[BitUtilities::flipNumber(current)], opp)) board[BitUtilities::flipNumber(current)] = BitUtilities::flipBit(board[BitUtilities::flipNumber(current)], opp); //check if opponent is on main board
            else kings[BitUtilities::flipNumber(current)] = BitUtilities::flipBit(kings[BitUtilities::flipNumber(current)], opp); //check if opponent is on kings board
        }
    }

    //king the player's piece if necessary
    if(!move.isKing){
        Border border = checkBorder(move.end);
        if((border == TOP || border == RIGHT_TOP) && current == PLAYERTWO) kingPiece(current, board, kings, move.end);
        else if((border == BOTTOM || border == LEFT_BOTTOM) && current == PLAYERONE) kingPiece(current, board, kings, move.end);
    }

}

//swaps the position of each position from start to dest
void MoveManager::swapPosition(PlayerCode current, unsigned int* board, Move move){
    board[current] = BitUtilities::flipBit(board[current], move.start);
    board[current] = BitUtilities::flipBit(board[current], move.end);
}

//king the player's piece
void MoveManager::kingPiece(PlayerCode current, unsigned int* board, unsigned int* kings, int dest){
    board[current] = BitUtilities::flipBit(board[current], dest);
    kings[current] = BitUtilities::flipBit(kings[current], dest);
}

//returns a text based version of the coordinate of the point of the bit on the checker board
string MoveManager::getCoor(int position){
    string coordinate;

    int row = position/4;
    int oddEven = row%2;
    
    char rowLetter = 65+row;
    coordinate.push_back(rowLetter);
    char col = ((oddEven)?((position%4)*2):((position%4)*2+1))+'0'; 
    coordinate.push_back(col);
    return coordinate;    
}

//TODO: Make sure I include the top right and left bottom in necessary spots
//return the state of the position of the current integer relative to the border
Border MoveManager::checkBorder(int position){
    if(position >= 0 && position <= 3) {        
        return (position%4==3)?RIGHT_TOP:TOP;
    } else if(position >= 28 && position <= 31){
        return (position%4==0)?LEFT_BOTTOM:BOTTOM;
    } else if(position%4 == 0  && (position/4)%2 == 1) {
        return LEFT;
    } else if(position%4 == 3 && (position/4)%2 == 0) {
        return RIGHT;
    } else {
        return OPEN;
    }
}


//prompt user to select a letter of the board that has an open move. Then select the specific token to move. Returning a tuple containing the coordinates.
std::tuple<char, int> MoveManager::getUserSelection(map<char, vector<Move>>& charToPiece){
    string keyInput;

    cout << "\nEnter the letter of the desired move. _>>> ";
    while(true){        
        cin >> keyInput;
        if(!isdigit(keyInput[0]) && charToPiece.find(toupper(keyInput[0])) != charToPiece.end()){
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
     cin.get();
    return std::make_tuple(toupper(keyInput[0]), (pieceInput[0]-'0'));
}
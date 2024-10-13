#include "BitUtilities.h"

//return flipped bit value
int BitUtilities::flipNumber(int value){
    return value^1;
}

//return the unsigned int after flipping the bit at the given location
unsigned int BitUtilities::flipBit(unsigned int value, int position){
    return value ^ (1 << position);
}

//set bit at given location to 1
unsigned int BitUtilities::setBit(unsigned int value, int position){    
    return value | (1 << position);
}

//check value of bit return 1 or 0
int BitUtilities::checkBit(unsigned int num, int position){
    return (num & (1 << position));
}

//counts the number of 1 bits in the given number
int BitUtilities::countBits(int player, unsigned int* board){
    int count = 0;
    for(int i = 0 ; i < 32 ; i++){
        count += (BitUtilities::checkBit(board[player], i))?1:0;
    }
    return count;
}

//merge the 1's in two unsigned integers and return a new integer.
unsigned int BitUtilities::mergeBits(unsigned int a, unsigned int b){
    unsigned int merged = 0;
    for(int i = 0 ; i < 32 ; i++){
        if(BitUtilities::checkBit(a, i) || BitUtilities::checkBit(b, i)) merged = BitUtilities::flipBit(merged, i);
    }
    return merged;
}

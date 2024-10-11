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

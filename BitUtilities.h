#ifndef BITUTILITIES_H
#define BITUTILITIES_H

class BitUtilities {
public:
    //return flipped bit value
    static int flipNumber(int);

    //return the unsigned int after flipping the bit at the given location
    static unsigned int flipBit(unsigned int, int);

    //set bit at given location to 1
    static unsigned int setBit(unsigned int, int);

    //check value of bit return 1 or 0
    static int checkBit(unsigned int, int);
};

#endif
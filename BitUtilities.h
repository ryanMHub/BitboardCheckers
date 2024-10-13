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

    //counts the number of 1 bits in the given number
    static int countBits(int, unsigned int*);

    //merge bits 1 bits of two unsigned integers
    static unsigned int mergeBits(unsigned int, unsigned int);
};

#endif
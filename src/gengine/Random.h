#ifndef HEADER_RANDOM_H
#define HEADER_RANDOM_H

/**
 * Utils for random numbers.
 */
class Random {
    private:
        static const int ARRAY_SIZE = 255;
        static unsigned char ms_randArray[ARRAY_SIZE];
    public:
        static void init();

        static int randomInt(int bound);
        static double randomReal(double bound);

        static unsigned char aByte(unsigned int index);
};

#endif

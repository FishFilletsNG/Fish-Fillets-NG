#ifndef HEADER_RANDOM_H
#define HEADER_RANDOM_H

/**
 * Utils for random numbers.
 */
class Random {
    public:
        static void init();

        static int randomInt(int bound);
        static double randomReal(double bound);
};

#endif

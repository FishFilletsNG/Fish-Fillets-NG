/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Random.h"

#include <stdlib.h>
#include <time.h>

unsigned char Random::ms_randArray[];
//----------------------------------------------------------
/**
 *  Init random generator.
 */
void
Random::init() {
    srand( static_cast<unsigned>(time(NULL)) );
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        ms_randArray[i] = randomInt(256);
    }
}

//-----------------------------------------------------------------
/*
 * Return number from interval <0,bound).
 */
    int
Random::randomInt(int bound)
{
    if (bound == 0) {
        return 0;
    }
    return rand() % bound;
}
//-----------------------------------------------------------------
/*
 * Return real number from interval <0,bound).
 */
    double
Random::randomReal(double bound)
{
    return bound * rand() / (RAND_MAX + 1.0);
}
//-----------------------------------------------------------------
/**
 * Return a value from interval <0,255>.
 * @param index index of byte, the byte at a index is alway the same
 */
    unsigned char
Random::aByte(unsigned int index)
{
    return ms_randArray[index % ARRAY_SIZE];
}


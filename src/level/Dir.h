#ifndef HEADER_DIR_H
#define HEADER_DIR_H

#include "V2.h"

/**
 * Direction.
 */
class Dir {
    public:
        enum eDir {
            DIR_NO = 0,
            DIR_UP = 1,
            DIR_DOWN = 2,
            DIR_LEFT = 3,
            DIR_RIGHT = 4
        };
    public:
        static V2 dir2xy(eDir dir);
};

#endif

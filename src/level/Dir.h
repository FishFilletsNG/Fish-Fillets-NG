#ifndef HEADER_DIR_H
#define HEADER_DIR_H

#include "V2.h"

/**
 * Direction.
 */
class Dir {
    public:
        enum eDir {
            DIR_NO,
            DIR_UP,
            DIR_DOWN,
            DIR_LEFT,
            DIR_RIGHT
        };
    public:
        static V2 dir2xy(eDir dir);
};

#endif

#ifndef HEADER_DRIVER_H
#define HEADER_DRIVER_H

class Cube;

#include "NoCopy.h"

/**
 * Driver drive object moves.
 */
class Driver : public NoCopy {
    public:
        virtual ~Driver() {}

        virtual bool drive(Cube *) { return false; }
};

#endif

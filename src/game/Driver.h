#ifndef HEADER_DRIVER_H
#define HEADER_DRIVER_H

class Cube;

#include "NoCopy.h"

class Driver : public NoCopy {
    public:
        virtual ~Driver() {}

        virtual bool drive(Cube *) { return false; }
};

#endif

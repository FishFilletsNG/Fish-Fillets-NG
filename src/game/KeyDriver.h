#ifndef HEADER_KEYDRIVER_H
#define HEADER_KEYDRIVER_H

#include "Driver.h"
#include "KeyControl.h"

#include "SDL.h"

/**
 * Driver with keyboard.
 */
class KeyDriver : public Driver {
    private:
        KeyControl m_control;
        Uint8 *m_keys;
        bool m_lastResult;
    public:
        KeyDriver(const KeyControl &control);

        virtual bool drive(Cube *model);
};

#endif

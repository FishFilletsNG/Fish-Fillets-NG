#ifndef HEADER_INPUTHANDLER_H
#define HEADER_INPUTHANDLER_H

class KeyStroke;
class MouseStroke;

#include "NoCopy.h"

/**
 * Handle input events.
 */
class InputHandler : public NoCopy {
    public:
        virtual void keyEvent(const KeyStroke &/*stroke*/) {}
        virtual void mouseEvent(const MouseStroke &/*buttons*/) {}
};

#endif

#ifndef HEADER_INPUTHANDLER_H
#define HEADER_INPUTHANDLER_H

class KeyStroke;
class MouseStroke;

#include "NoCopy.h"
#include "InputProvider.h"

/**
 * Handle input events.
 */
class InputHandler : public InputProvider, public NoCopy {
    private:
        Uint8 *m_pressed;
        V2 m_mouseLoc;
    public:
        InputHandler() : m_mouseLoc(-1, -1) { m_pressed = NULL; }
        void takePressed(Uint8 *pressed) { m_pressed = pressed; }
        void mouseLoc(const V2 &loc) { m_mouseLoc = loc; }

        virtual void keyEvent(const KeyStroke &/*stroke*/) {}
        virtual void mouseEvent(const MouseStroke &/*buttons*/) {}

        virtual bool isPressed(SDLKey key) const
        { return m_pressed && m_pressed[key]; }
        virtual V2 getMouseLoc() const { return m_mouseLoc; }
};

#endif

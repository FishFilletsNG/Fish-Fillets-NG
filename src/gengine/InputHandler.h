#ifndef HEADER_INPUTHANDLER_H
#define HEADER_INPUTHANDLER_H

class KeyStroke;
class MouseStroke;

#include "NoCopy.h"
#include "InputProvider.h"

/**
 * Handle input events.
 * Events:
 * - keyEvent() is called when new key is pressed.
 * - mouseEvent() is called when there is a mouse click.
 * - mouseState() is called every cycle to update mouse position
 *
 * Pressed keys are stored in m_pressed array, this array is shared
 * and updated by InputAgent.
 */
class InputHandler : public InputProvider, public NoCopy {
    private:
        Uint8 *m_pressed;
        Uint8 m_buttons;
        V2 m_mouseLoc;
    public:
        InputHandler();
        void takePressed(Uint8 *pressed) { m_pressed = pressed; }
        void mouseState(const V2 &loc, Uint8 buttons);

        virtual void keyEvent(const KeyStroke &/*stroke*/) {}
        virtual void keyUp(const KeyStroke &/*stroke*/) {}
        virtual void mouseEvent(const MouseStroke &/*buttons*/) {}

        virtual bool isPressed(SDLKey key) const;
        virtual bool isLeftPressed() const;
        virtual bool isMiddlePressed() const;
        virtual bool isRightPressed() const;
        virtual V2 getMouseLoc() const { return m_mouseLoc; }

        virtual std::string toString() const;
};

#endif

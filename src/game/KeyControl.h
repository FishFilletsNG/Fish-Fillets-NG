#ifndef HEADER_KEYCONTROL_H
#define HEADER_KEYCONTROL_H

#include "SDL.h"

/**
 * Keyboard controls.
 */
class KeyControl {
    private:
        SDLKey m_up;
        SDLKey m_down;
        SDLKey m_left;
        SDLKey m_right;
    public:
        KeyControl();
        void setUp(SDLKey key) { m_up = key; }
        void setDown(SDLKey key) { m_down = key; }
        void setLeft(SDLKey key) { m_left = key; }
        void setRight(SDLKey key) { m_right = key; }

        SDLKey getUp() const { return m_up; }
        SDLKey getDown() const { return m_down; }
        SDLKey getLeft() const { return m_left; }
        SDLKey getRight() const { return m_right; }
};

#endif

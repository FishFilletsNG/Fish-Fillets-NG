#ifndef HEADER_KEYSTROKE_H
#define HEADER_KEYSTROKE_H

#include "SDL.h"
#include <string>

/**
 * KeyStroke.
 */
class KeyStroke {
    private:
        static const int STROKE_IGNORE = KMOD_NUM|KMOD_CAPS|KMOD_MODE;
        SDLKey m_sym;
        int m_mod;
    private:
        static int modStrip(int mod);
    public:
        KeyStroke(SDLKey sym, int mod);

        bool less(const KeyStroke &other) const;
        bool equals(const KeyStroke &other) const;
        bool isPressed() const;
        std::string toString() const;
};

#endif

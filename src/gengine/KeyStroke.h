#ifndef HEADER_KEYSTROKE_H
#define HEADER_KEYSTROKE_H

#include "SDL.h"
#include <string>

/**
 * KeyStroke.
 */
class KeyStroke {
    private:
        SDLKey m_sym;
        int m_mod;
    public:
        KeyStroke(SDLKey sym, int mod);

        bool less(const KeyStroke &other) const;
        std::string toString() const;
};

#endif

#ifndef HEADER_KEYMAP_H
#define HEADER_KEYMAP_H

#include "KeyStroke.h"

#include "SDL.h"
#include <string>
#include <map>

/**
 * Table of defined keys.
 */
class Keymap {
    private:
        typedef std::map<std::string,KeyStroke> t_keys;
        t_keys m_keys;
    public:
        void registerKey(const std::string &key, const KeyStroke &stroke);
        bool isPressed(const std::string &key) const;
};

#endif

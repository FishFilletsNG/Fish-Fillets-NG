#ifndef HEADER_KEYBINDER_H
#define HEADER_KEYBINDER_H

class BaseMsg;

#include "NoCopy.h"
#include "KeyStroke.h"

#include <map>

/**
 * Key binder.
 */
class KeyBinder : public NoCopy {
    private:
        typedef std::map<KeyStroke,BaseMsg*,stroke_less> t_strokes;
        t_strokes m_strokes;
    public:
        ~KeyBinder();

        void addStroke(const KeyStroke &stroke, BaseMsg *msg);
        void removeStroke(const KeyStroke &stroke);

        void keyDown(const SDL_keysym &keysym) const;
};

#endif

#ifndef HEADER_OPTIONSINPUT_H
#define HEADER_OPTIONSINPUT_H

class MenuOptions;
class Keymap;

#include "InputHandler.h"

/**
 * Handle input for options menu.
 */
class OptionsInput : public InputHandler {
    private:
        static const int KEY_QUIT = 1;

        MenuOptions *m_menu;
        Keymap *m_keymap;
    public:
        OptionsInput(MenuOptions *menu);
        ~OptionsInput();

        virtual void keyEvent(const KeyStroke &stroke);
        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif

#ifndef HEADER_OPTIONSINPUT_H
#define HEADER_OPTIONSINPUT_H

class MenuOptions;

#include "StateInput.h"

/**
 * Handle input for options menu.
 */
class OptionsInput : public StateInput {
    private:
        MenuOptions *getMenu();
    protected:
        virtual void enableMenu();
    public:
        OptionsInput(MenuOptions *menu);

        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif

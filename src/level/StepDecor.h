#ifndef HEADER_STEPDECOR_H
#define HEADER_STEPDECOR_H

class View;
class StepCounter;

#include "Decor.h"
#include "Font.h"

/**
 * Draw number of steps.
 */
class StepDecor : public Decor {
    private:
        Font m_font;
        const StepCounter *m_counter;
    public:
        StepDecor(const StepCounter *counter);
        virtual void drawOnScreen(const View *view, SDL_Surface *screen);
};

#endif

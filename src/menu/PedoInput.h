#ifndef HEADER_PEDOINPUT_H
#define HEADER_PEDOINPUT_H

class Keymap;
class Pedometer;

#include "GameInput.h"

/**
 * Handle input for pedometer.
 */
class PedoInput : public GameInput {
    private:
        Pedometer *getPedo();
    protected:
        virtual void enableSubtitles() {}
        virtual void enableHelp() {}
    public:
        PedoInput(Pedometer *pedometer);

        virtual void mouseEvent(const MouseStroke &buttons);
};

#endif

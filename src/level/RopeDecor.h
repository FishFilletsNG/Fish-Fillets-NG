#ifndef HEADER_ROPEDECOR_H
#define HEADER_ROPEDECOR_H

class Cube;
class View;

#include "Decor.h"
#include "V2.h"

#include "SDL.h"

/**
 * Draw rope between two models.
 */
class RopeDecor : public Decor {
    private:
        const Cube *m_model1;
        const Cube *m_model2;
        V2 m_shift1;
        V2 m_shift2;
    public:
        RopeDecor(const Cube *model1, const Cube *model2,
                const V2 &shift1, const V2 &shift2);
        virtual void drawOnScreen(const View *view, SDL_Surface *screen);
};

#endif

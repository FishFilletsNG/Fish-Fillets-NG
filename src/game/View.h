#ifndef HEADER_VIEW_H
#define HEADER_VIEW_H

class Cube;
class Anim;

#include "IDrawer.h"
#include "NoCopy.h"
#include "Path.h"

/**
 * View for model.
 */
class View : public IDrawer, public NoCopy {
    public:
        static const int SCALE = 15;
    private:
        const Cube *m_model;
        int m_animShift;
        int m_shiftSize;
        Anim *m_anim;
    private:
        void computeShiftSize();
    public:
        View(const Path &picture);
        virtual ~View();
        void takeModel(const Cube *model) { m_model = model; }

        void prepareRound();
        void draw();

        Anim *anim() { return m_anim; }
};

#endif

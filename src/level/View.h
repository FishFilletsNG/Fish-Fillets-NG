#ifndef HEADER_VIEW_H
#define HEADER_VIEW_H

class Cube;
class PhaseLocker;

#include "IDrawer.h"
#include "ModelList.h"

/**
 * View for model.
 */
class View : public IDrawer {
    public:
        static const int SCALE = 15;
    private:
        int m_animShift;
        int m_shiftSize;
        ModelList m_models;
    private:
        void computeShiftSize(int phases);
    public:
        View(const ModelList &models);
        void noteNewRound(int phases);
        void drawModel(Cube *model);
        virtual void draw();
};

#endif

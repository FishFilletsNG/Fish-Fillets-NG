#ifndef HEADER_VIEW_H
#define HEADER_VIEW_H

class Cube;
class PhaseLocker;
class Decor;

#include "IDrawer.h"
#include "ModelList.h"
#include "V2.h"

/**
 * View for model.
 */
class View : public IDrawer {
    public:
        static const int SCALE = 15;
    private:
        typedef std::vector<Decor*> t_decors;
        t_decors m_decors;
        ModelList m_models;
        int m_animShift;
        int m_shiftSize;
    private:
        void computeShiftSize(int phases);
        void drawDecors();
    public:
        View(const ModelList &models);
        virtual ~View();
        void noteNewRound(int phases);

        void drawModel(Cube *model);
        virtual void draw();

        V2 getScreenPos(const Cube *model) const;
        void addDecor(Decor *new_decor) { m_decors.push_back(new_decor); }
        void removeDecors();
};

#endif

#ifndef HEADER_VIEW_H
#define HEADER_VIEW_H

class Cube;
class PhaseLocker;
class Decor;

#include "Drawable.h"
#include "ModelList.h"
#include "V2.h"

/**
 * View for model.
 */
class View : public Drawable {
    public:
        static const int SCALE = 15;
    private:
        typedef std::vector<Decor*> t_decors;
        t_decors m_decors;
        ModelList m_models;
        int m_animShift;
        int m_shiftSize;
        SDL_Surface *m_screen;
        V2 m_screenShift;
    private:
        void computeShiftSize(int phases);
        void drawDecors();
    public:
        View(const ModelList &models);
        virtual ~View();
        void setScreenShift(const V2 &shift) { m_screenShift = shift; }
        void noteNewRound(int phases);

        void drawModel(Cube *model);
        virtual void drawOn(SDL_Surface *screen);

        V2 getScreenPos(const Cube *model) const;
        V2 getFieldPos(const V2 &cursor) const;
        void addDecor(Decor *new_decor) { m_decors.push_back(new_decor); }
        void removeDecors();
};

#endif

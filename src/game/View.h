#ifndef HEADER_VIEW_H
#define HEADER_VIEW_H

class Cube;
class ResImagePack;

#include "IDrawer.h"
#include "NoCopy.h"
#include "Path.h"

#include "SDL.h"

class View : public IDrawer, public NoCopy {
    public:
        static const int SCALE = 15;

        SDL_Surface *m_surface;
        const Cube *m_model;
        int m_animShift;
        int m_shiftSize;
        ResImagePack *m_anim;
    private:
        void computeShiftSize();
    protected:
        virtual void drawAt(int x, int y);
    public:
        View(const Path &picture);
        virtual ~View();
        void takeModel(const Cube *model) { m_model = model; }

        void prepareRound();
        void draw();

        void addAnim(const std::string &name, const Path &picture);
        void setAnim(const std::string &name, int phase=0);
};

#endif

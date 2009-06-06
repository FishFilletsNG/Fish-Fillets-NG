#ifndef HEADER_ANIM_H
#define HEADER_ANIM_H

class Path;
class ResImagePack;

#include "ViewEffect.h"
#include "NoCopy.h"
#include "V2.h"

#include "SDL.h"

/**
 * Animation sprite.
 */
class Anim : public NoCopy {
    public:
        enum eSide {
            SIDE_LEFT = 0,
            SIDE_RIGHT = 1
        };
    private:
        ViewEffect *m_effect;
        V2 m_viewShift;
        ResImagePack *m_animPack[2];
        std::string m_animName;
        int m_animPhase;
        bool m_run;
        std::string m_specialAnimName;
        int m_specialAnimPhase;
        std::string m_usedPath;
    private:
        void blit(SDL_Surface *screen, SDL_Surface *surface, int x, int y);
    public:
        Anim();
        virtual ~Anim();

        void drawAt(SDL_Surface *screen, int x, int y, eSide side);

        void addAnim(const std::string &name, const Path &picture,
                eSide side=SIDE_LEFT);
        void addAnim(const std::string &name, SDL_Surface *new_image,
                eSide side=SIDE_LEFT);
        void runAnim(const std::string &name, int start_phase=0);
        void setAnim(const std::string &name, int phase);
        void useSpecialAnim(const std::string &name, int phase);

        bool isDisintegrated() const { return m_effect->isDisintegrated(); }
        bool isInvisible() const { return m_effect->isInvisible(); }
        void changeEffect(ViewEffect *new_effect);
        void setViewShift(const V2 &shift) { m_viewShift = shift; }
        V2 getViewShift() const { return m_viewShift; };
        void setEffect(const std::string &effectName);

        int countAnimPhases(const std::string &anim,
                eSide side=SIDE_LEFT) const;
        std::string getState() const;
        void restoreState(const std::string &state);
};

#endif

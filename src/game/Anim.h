#ifndef HEADER_ANIM_H
#define HEADER_ANIM_H

class ResImagePack;

#include "NoCopy.h"
#include "Path.h"

#include "SDL.h"

/**
 * Animation sprite.
 */
class Anim : public NoCopy {
    public:
        enum eSide {
            SIDE_LEFT,
            SIDE_RIGHT
        };
    private:
        ResImagePack *m_animPack[2];
        std::string m_animName;
        int m_animPhase;
    public:
        Anim();
        virtual ~Anim();

        void drawAt(SDL_Surface *screen, int x, int y, eSide side);

        void addAnim(const std::string &name, const Path &picture);
        void addDuplexAnim(const std::string &name,
                const Path &left_picture, const Path &right_picture);
        void setAnim(const std::string &name, int phase=0);
};

#endif

#ifndef HEADER_WAVYPICTURE_H
#define HEADER_WAVYPICTURE_H

#include "Picture.h"

/**
 * Wavy picture at fixed screen position.
 */
class WavyPicture : public Picture {
    private:
        float m_amp;
        float m_periode;
        float m_speed;
    public:
        WavyPicture(const Path &file, const V2 &loc);
        void setWamp(float amplitude) { m_amp = amplitude; }
        void setWperiode(float periode) { m_periode = periode; }
        void setWspeed(float speed) { m_speed = speed; }

        virtual void drawOn(SDL_Surface *screen);
};

#endif

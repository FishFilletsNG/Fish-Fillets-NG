#ifndef HEADER_WAVYPICTURE_H
#define HEADER_WAVYPICTURE_H

#include "Picture.h"

/**
 * Wavy picture at fixed screen position.
 */
class WavyPicture : public Picture {
    private:
        double m_amp;
        double m_periode;
        double m_speed;
    public:
        WavyPicture(const Path &file, const V2 &loc);
        void setWamp(double amplitude) { m_amp = amplitude; }
        void setWperiode(double periode) { m_periode = periode; }
        void setWspeed(double speed) { m_speed = speed; }

        virtual void draw();
};

#endif

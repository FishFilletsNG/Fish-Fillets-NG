#ifndef HEADER_SLIDER_H
#define HEADER_SLIDER_H

#include "IWidget.h"

/**
 * Slider for option value.
 */
class Slider : public IWidget {
    private:
        static const int PIXELS_PER_VALUE = 2;
        static const int HEIGHT = 30;
        std::string m_param;
        int m_min;
        int m_max;
    private:
        int value2slide(int value);
        int slide2value(int slide);
        virtual void own_mouseButton(const MouseStroke &stroke);
    public:
        Slider(const std::string &param, int minValue=0, int maxValue=100);
        virtual int getW() const { return (m_max - m_min) * PIXELS_PER_VALUE; }
        virtual int getH() const { return HEIGHT; }

        virtual void drawOn(SDL_Surface *screen);
};

#endif

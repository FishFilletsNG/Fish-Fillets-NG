#ifndef HEADER_RADIOBOX_H
#define HEADER_RADIOBOX_H

#include "WiPicture.h"

/**
 * Radio box with picture background.
 */
class RadioBox : public WiPicture {
    private:
        static const int BORDER = 4;
        std::string m_param;
        std::string m_value;
    protected:
        virtual void own_mouseButton(const MouseStroke &stroke);
    public:
        RadioBox(const std::string &param, const std::string &value,
                const Path &picture);
        virtual int getW() const { return WiPicture::getW() + 2 * BORDER; }
        virtual int getH() const { return WiPicture::getH() + 2 * BORDER; }

        virtual void drawOn(SDL_Surface *screen);
};

#endif

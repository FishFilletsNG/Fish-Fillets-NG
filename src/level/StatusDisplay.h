#ifndef HEADER_STATUSDISPLAY_H
#define HEADER_STATUSDISPLAY_H

class Picture;

#include "Drawable.h"

/**
 * Show status change.
 */
class StatusDisplay : public Drawable {
    private:
        Picture *m_picture;
        int m_time;
    public:
        StatusDisplay();
        virtual ~StatusDisplay();
        void displayStatus(Picture *new_picture, int time);
        virtual void drawOn(SDL_Surface *screen);
};

#endif

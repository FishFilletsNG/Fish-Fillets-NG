#ifndef HEADER_TITLE_H
#define HEADER_TITLE_H

class Font;
class Color;

#include "Drawable.h"

#include <string>

/**
 * Subtitle.
 */
class Title : public Drawable {
    private:
        static const int TIME_PER_CHAR = 2;
        static const int TIME_MIN = 40;
        int m_screenW;
        int m_screenH;
        int m_x;
        int m_y;
        int m_mintime;
        int m_limitY;
        int m_finalY;
        const std::string m_content;
        Font *m_font;
        SDL_Surface *m_surface;
    public:
        Title(int baseY, int finalY, int bonusTime, int limitY,
                const std::string &content,
                Font *font, const Color *color);
        virtual ~Title();

        void shiftUp(int rate);
        void shiftFinalUp(int rate);
        virtual void drawOn(SDL_Surface *screen);
        bool isGone();

        int getY() const;
        std::string getContent() const { return m_content; }
};

#endif

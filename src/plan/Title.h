#ifndef HEADER_TITLE_H
#define HEADER_TITLE_H

#include "IDrawer.h"

#include "SFont.h"

#include <string>

/**
 * Subtitle.
 */
class Title : public IDrawer {
    private:
        static const int TIME_PER_CHAR = 2;
        static const int TIME_MIN = 40;
        int m_x;
        int m_y;
        int m_mintime;
        int m_limitY;
        int m_finalY;
        const std::string m_content;
        const SFont_Font *m_font;
    public:
        Title(int baseY, int finalY, int bonusTime, int limitY,
                const std::string &content, const SFont_Font *font);

        void shiftUp(int rate);
        void shiftFinalUp(int rate);
        virtual void draw();
        bool isGone();

        int getY() const;
        std::string getContent() const { return m_content; }
};

#endif

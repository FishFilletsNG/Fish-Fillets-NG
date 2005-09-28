#ifndef HEADER_FONT_H
#define HEADER_FONT_H

class Path;

#include "NoCopy.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include <string>

/**
 * TrueType UTF-8 font.
 */
class Font : public NoCopy {
    private:
        TTF_Font *m_ttfont;
        SDL_Color m_bg;
    private:
        static std::string biditize(const std::string &text);
    public:
        Font(const Path &file_ttf, int height);
        ~Font();
        static void init();
        static void shutdown();

        int getHeight() { return TTF_FontHeight(m_ttfont); }
        int calcTextWidth(const std::string &text);
        SDL_Surface *renderText(const std::string &text,
                const SDL_Color &color) const;
        SDL_Surface *renderTextOutlined(const std::string &text,
                const SDL_Color &color, int outlineWidth=1) const;
};

#endif

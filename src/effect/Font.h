#ifndef HEADER_FONT_H
#define HEADER_FONT_H

#include "NoCopy.h"
#include "Path.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include <string>

class Font : public NoCopy {
    private:
        TTF_Font *m_ttfont;
        SDL_Color m_bg;
    public:
        Font(const Path &file_ttf, int height);
        ~Font();

        int calcTextWidth(const std::string &text);
        SDL_Surface *renderText(const std::string &text,
                const SDL_Color &color);
        SDL_Surface *renderTextOutlined(const std::string &text,
                const SDL_Color &color, int outlineWidth=1);
};

#endif

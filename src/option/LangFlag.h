#ifndef HEADER_LANGFLAG_H
#define HEADER_LANGFLAG_H

class V2;
class Path;

#include "NoCopy.h"

#include "SDL.h"
#include <string>

/**
 * Flag to select prefered lang.
 */
class LangFlag : public NoCopy {
    private:
        std::string m_lang;
        SDL_Surface *m_surface;
    public:
        LangFlag(const std::string &lang, const Path &picture);
        virtual ~LangFlag();
        std::string getLang() const { return m_lang; }
        int getW() const { return m_surface->w; }
        int getH() const { return m_surface->h; }

        void drawOn(SDL_Surface *screen, const V2 &loc);
};

#endif

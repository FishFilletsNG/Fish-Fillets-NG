#ifndef HEADER_KEYCONSOLE_H
#define HEADER_KEYCONSOLE_H

#include "IDrawer.h"
#include "SFont.h"

#include <string>
#include "SDL.h"

/**
 * Debug console.
 */
class KeyConsole : public IDrawer {
    private:
        SFont_Font *m_font;
        std::string m_history;
        std::string m_input;
        std::string m_handlerName;
        Uint32 m_color;
    public:
        KeyConsole();
        ~KeyConsole();

        void keyDown(const SDL_keysym &keysym);
        void setHandler(const std::string &handlerName);

        virtual void draw();
};

#endif

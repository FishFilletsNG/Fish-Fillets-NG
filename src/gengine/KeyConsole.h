#ifndef HEADER_KEYCONSOLE_H
#define HEADER_KEYCONSOLE_H

#include "NoCopy.h"
#include "IDrawer.h"
#include "SFont.h"

#include <string>
#include "SDL.h"

/**
 * Debug console.
 */
class KeyConsole : public IDrawer, public NoCopy {
    private:
        SFont_Font *m_font;
        bool m_active;
        std::string m_history;
        std::string m_input;
        std::string m_handlerName;
        Uint32 m_color;
    private:
        void deactivate();
    public:
        KeyConsole();
        ~KeyConsole();

        bool isActive() const { return m_active; }
        void activate();

        void keyDown(const SDL_keysym &keysym);
        void setHandler(const std::string &handlerName);

        virtual void draw();
};

#endif

#ifndef HEADER_KEYCONSOLE_H
#define HEADER_KEYCONSOLE_H

class Font;

#include "IKeyConsole.h"
#include "Color.h"

#include <string>
#include "SDL.h"

/**
 * Debug console.
 */
class KeyConsole : public IKeyConsole {
    private:
        Font *m_font;
        std::string m_history;
        std::string m_input;
        std::string m_handlerName;
        Color m_color;
    public:
        KeyConsole();
        virtual ~KeyConsole();

        virtual void keyDown(const SDL_keysym &keysym);
        void setHandler(const std::string &handlerName);

        virtual void drawOn(SDL_Surface *screen);
};

#endif

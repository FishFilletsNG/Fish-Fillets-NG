#ifndef HEADER_MOUSESTROKE_H
#define HEADER_MOUSESTROKE_H

#include "V2.h"

#include "SDL.h"
#include <string>

/**
 * Contains info about mouse click.
 */
class MouseStroke {
    private:
        Uint8 m_button;
        V2 m_loc;
        bool m_doScale;
        static float scaleX, scaleY;
        static V2 letterbox, logicalRes, renderRes;
    public:
        MouseStroke(const SDL_MouseButtonEvent &event);

        bool isLeft() const { return m_button == SDL_BUTTON_LEFT; }
        bool isMiddle() const { return m_button == SDL_BUTTON_MIDDLE; }
        bool isRight() const { return m_button == SDL_BUTTON_RIGHT; }
        static void setScale(float x, float y);
        static void getScale(float& x, float& y);
        void disableScaling();
        static void setLetterbox(V2 letterbox);
        static void setResolution(V2 logical, V2 render);
        V2 getLoc() const;
        std::string toString() const;
};

#endif

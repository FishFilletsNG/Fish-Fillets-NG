#ifndef HEADER_NODEDRAWER_H
#define HEADER_NODEDRAWER_H

class LevelNode;

#include "NoCopy.h"
#include "SFont.h"

#include "SDL.h"
#include <string>

/**
 * Drawer which know how to draw nodes on path.
 */
class NodeDrawer : public NoCopy {
    private:
        SDL_Surface *m_screen;
        SDL_Surface *m_dotOpen;
        SDL_Surface *m_dotFar;
        SDL_Surface *m_dotSolved;
        SDL_Surface *m_dotSelected;
        int m_dotRadius;
        SFont_Font *m_font;
    public:
        NodeDrawer();
        virtual ~NodeDrawer();
        void setScreen(SDL_Surface *screen) { m_screen = screen; }

        void drawNode(const LevelNode *node) const;
        void drawEdge(const LevelNode *start, const LevelNode *end) const;
        void drawSelected(const std::string &levelname) const;
};

#endif

#ifndef HEADER_NODEDRAWER_H
#define HEADER_NODEDRAWER_H

class LevelNode;
class ResImagePack;

#include "NoCopy.h"
#include "SFont.h"
#include "V2.h"

#include "SDL.h"
#include <string>

/**
 * Drawer which know how to draw nodes on path.
 */
class NodeDrawer : public NoCopy {
    private:
        ResImagePack *m_imagePack;
        SDL_Surface *m_screen;
        SFont_Font *m_font;
    private:
        void drawDot(SDL_Surface *dot, const V2 &loc) const;
    public:
        NodeDrawer();
        virtual ~NodeDrawer();
        void setScreen(SDL_Surface *screen) { m_screen = screen; }

        void drawNode(const LevelNode *node) const;
        void drawEdge(const LevelNode *start, const LevelNode *end) const;
        void drawSelected(const std::string &levelname) const;
};

#endif

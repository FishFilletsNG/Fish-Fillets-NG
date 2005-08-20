#ifndef HEADER_NODEDRAWER_H
#define HEADER_NODEDRAWER_H

class V2;
class LevelNode;
class ResImagePack;
class Font;

#include "NoCopy.h"

#include "SDL.h"
#include <string>

/**
 * Drawer which know how to draw nodes on path.
 */
class NodeDrawer : public NoCopy {
    private:
        ResImagePack *m_imagePack;
        SDL_Surface *m_screen;
        Font *m_font;
    private:
        void drawDot(SDL_Surface *dot, const V2 &loc) const;
    public:
        NodeDrawer();
        virtual ~NodeDrawer();
        void setScreen(SDL_Surface *screen) { m_screen = screen; }

        void drawNode(const LevelNode *node) const;
        void drawEdge(const LevelNode *start, const LevelNode *end) const;
        void drawSelect(const V2 &loc) const;
        void drawSelected(const std::string &levelname) const;
};

#endif

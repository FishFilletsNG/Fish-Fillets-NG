#ifndef HEADER_WORLDMAP_H
#define HEADER_WORLDMAP_H

class Level;
class LevelNode;
class NodeDrawer;

#include "Path.h"
#include "NoCopy.h"
#include "IDrawer.h"


/**
 * Map with path from one level to another.
 */
class WorldMap : public IDrawer, public NoCopy {
    private:
        LevelNode *m_startNode;
        NodeDrawer *m_drawer;
        LevelNode *m_selected;
        bool m_active;
        SDL_Surface *m_bg;
    public:
        WorldMap(LevelNode *startNode, const Path &bg);
        virtual ~WorldMap();
        int getW() const { return m_bg->w; }
        int getH() const { return m_bg->h; }

        void activate();
        void deactivate();

        void markSolved();
        void watchCursor();
        Level *createSelected() const;
        virtual void draw();
};

#endif

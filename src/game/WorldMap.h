#ifndef HEADER_WORLDMAP_H
#define HEADER_WORLDMAP_H

class Level;
class LevelNode;
class NodeDrawer;
class ResDialogPack;
class LevelDesc;

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
        ResDialogPack *m_descPack;
        bool m_active;
        SDL_Surface *m_bg;
    public:
        WorldMap(const Path &bg);
        virtual ~WorldMap();
        void initWay(const Path &way, const Path &desc);
        int getW() const { return m_bg->w; }
        int getH() const { return m_bg->h; }

        void activate();
        void deactivate();

        void markSolved();
        void watchCursor();
        Level *createSelected() const;
        virtual void draw();

        void addDesc(const std::string &codename, LevelDesc *desc);
        std::string findLevelName(const std::string &codename) const;
        std::string findDesc(const std::string &codename) const;
};

#endif

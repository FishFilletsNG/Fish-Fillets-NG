#ifndef HEADER_WORLDMAP_H
#define HEADER_WORLDMAP_H

class Level;
class LevelNode;
class NodeDrawer;
class ResDialogPack;
class LevelDesc;
class LevelStatus;
class WorldInput;

#include "Path.h"
#include "IDrawer.h"
#include "GameState.h"

/**
 * Map with path from one level to another.
 */
class WorldMap : public GameState, public IDrawer {
    private:
        LevelNode *m_startNode;
        LevelNode *m_selected;
        NodeDrawer *m_drawer;
        ResDialogPack *m_descPack;
        LevelStatus *m_levelStatus;
        SDL_Surface *m_bg;
        WorldInput *m_input;
    private:
        void watchCursor();
        void watchButton();
        Level *createSelected() const;
        void markSolved();
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState();
        virtual void own_resumeState();
        virtual void own_cleanState();
    public:
        WorldMap(const Path &bg);
        virtual ~WorldMap();
        virtual const char *getName() const { return "state_worldmap"; };
        void initWay(const Path &way, const Path &desc);
        int getW() const { return m_bg->w; }
        int getH() const { return m_bg->h; }

        virtual void draw();

        void addDesc(const std::string &codename, LevelDesc *desc);
        std::string findLevelName(const std::string &codename) const;
        std::string findDesc(const std::string &codename) const;
};

#endif

#ifndef HEADER_WORLDMAP_H
#define HEADER_WORLDMAP_H

class Path;
class Level;
class LevelNode;
class NodeDrawer;
class ResDialogPack;
class LevelDesc;
class LevelStatus;
class LayeredPicture;

#include "Drawable.h"
#include "GameState.h"
#include "DescFinder.h"

/**
 * Map with path from one level to another.
 */
class WorldMap : public GameState, public DescFinder, public Drawable {
    private:
        LevelNode *m_startNode;
        LevelNode *m_selected;
        NodeDrawer *m_drawer;
        ResDialogPack *m_descPack;
        LevelStatus *m_levelStatus;

        LayeredPicture *m_bg;
        Uint32 m_activeMask;
        Uint32 m_maskIntro;
        Uint32 m_maskExit;
        Uint32 m_maskCredits;
        Uint32 m_maskOptions;
    private:
        void prepareBg();
        void watchCursor();
        Level *createSelected() const;
        void markSolved();

        void runIntro();
        void runCredits();
        void runOptions();
    protected:
        virtual void own_initState();
        virtual void own_updateState();
        virtual void own_pauseState() {}
        virtual void own_resumeState();
        virtual void own_cleanState();
    public:
        WorldMap();
        virtual ~WorldMap();
        virtual const char *getName() const { return "state_worldmap"; };
        void initWay(const Path &way, const Path &desc);

        virtual void drawOn(SDL_Surface *screen);
        void runSelected();

        void addDesc(const std::string &codename, LevelDesc *desc);
        virtual std::string findLevelName(const std::string &codename) const;
        virtual std::string findDesc(const std::string &codename) const;
};

#endif

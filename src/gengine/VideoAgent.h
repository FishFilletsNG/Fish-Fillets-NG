#ifndef HEADER_VIDEOAGENT_H
#define HEADER_VIDEOAGENT_H

class IDrawer;

#include "BaseAgent.h"
#include "Name.h"
#include "Path.h"

#include "SDL.h"

#include <vector>


/**
 * Video utils.
 */
class VideoAgent : public BaseAgent {
    AGENT(VideoAgent, Name::VIDEO_NAME);
    private:
        typedef std::vector<IDrawer*> t_drawers;
        t_drawers m_drawers;
        SDL_Surface *m_screen;
        bool m_fullscreen;

    private:
        void registerWatcher(const std::string &param);
        void setIcon(const Path &file);
        void initVideoMode();
        int getVideoFlags();
        void toggleFullScreen();
        static void setTransparent(SDL_Surface *surface);
        void updateDrawersScreen();

    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
    public:
        SDL_Surface *screen() { return m_screen; }
        virtual void receiveSimple(const SimpleMsg *msg);
        virtual void receiveString(const StringMsg *msg);

        void acceptDrawer(IDrawer *drawer);
        void removeDrawer(const IDrawer *drawer);
};

#endif

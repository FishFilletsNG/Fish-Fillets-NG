/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "VideoAgent.h"

#include "Log.h"
#include "ImgException.h"
#include "SDLException.h"
#include "LogicException.h"
#include "AgentPack.h"
#include "SimpleMsg.h"
#include "StringMsg.h"
#include "UnknownMsgException.h"
#include "IDrawer.h"
#include "OptionAgent.h"

#include "SDL_image.h"
#include <stdlib.h>
#include <algorithm>
#include <functional>

//-----------------------------------------------------------------
/**
 * Init SDL and grafic window.
 * Register watcher for "fullscren" and "screen_*" options.
 * @throws SDLException if there is no usuable video mode
 */
void
VideoAgent::own_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLException(ExInfo("Init"));
    }
    atexit(SDL_Quit);

    registerWatcher("fullscreen");
    registerWatcher("screen_width");
    registerWatcher("screen_height");
    registerWatcher("screen_bpp");

    SDL_WM_SetCaption(
            OptionAgent::agent()->getParam("caption", "A game").c_str(),
            NULL);
    setIcon(Path::dataReadPath("images/icon.png"));

    initVideoMode();
}
//-----------------------------------------------------------------
/**
 * Draw all drawer from list.
 * First will be drawed first.
 */
void
VideoAgent::own_update()
{
    std::for_each(m_drawers.begin(), m_drawers.end(),
            std::mem_fun(&IDrawer::draw));

    SDL_Flip(m_screen);
}
//-----------------------------------------------------------------
/**
 * Shutdown SDL.
 */
void
VideoAgent::own_shutdown()
{
    SDL_Quit();
}

//-----------------------------------------------------------------
/**
 * Register self as watcher for param
 */
void
VideoAgent::registerWatcher(const std::string &param)
{
    StringMsg *event = new StringMsg(this, "param_changed", param);
    OptionAgent::agent()->addWatcher(param, event);
}
//-----------------------------------------------------------------
/**
 * Load and set icon.
 * @throws ImgException
 */
    void
VideoAgent::setIcon(const Path &file)
{
    SDL_Surface *icon = IMG_Load(file.getNative().c_str());
    if (NULL == icon) {
        throw ImgException(ExInfo("Load")
                .addInfo("file", file.getNative()));
    }

    SDL_WM_SetIcon(icon, NULL);
    SDL_FreeSurface(icon);
}

//-----------------------------------------------------------------
/**
 * Init video mode along options.
 * @throws SDLException when video mode cannot be made,
 * the old video mode remain usable
 */
    void
VideoAgent::initVideoMode()
{
    int videoFlags = getVideoFlags();

    OptionAgent *options = OptionAgent::agent();
    int screen_width = options->getAsInt("screen_width", 640);
    int screen_height = options->getAsInt("screen_height", 480);
    int screen_bpp = options->getAsInt("screen_bpp", 32);
    int fullscreen = options->getAsInt("fullscreen", 0);
    if (fullscreen) {
        m_fullscreen = true;
        videoFlags |= SDL_FULLSCREEN;
    }

    //TODO: check VideoModeOK and available ListModes
    SDL_Surface *newScreen =
        SDL_SetVideoMode(screen_width, screen_height, screen_bpp, videoFlags);
    if (newScreen) {
        m_screen = newScreen;
        updateDrawersScreen();
    }
    else {
        throw SDLException(ExInfo("SetVideoMode")
                .addInfo("width", screen_width)
                .addInfo("height", screen_height)
                .addInfo("bpp", screen_bpp));
    }
}
//-----------------------------------------------------------------
/**
 * Obtain video information about best video mode.
 * @return best video flags
 */
    int
VideoAgent::getVideoFlags()
{
    int videoFlags;
    const SDL_VideoInfo *videoInfo;

    videoInfo = SDL_GetVideoInfo();
    if (NULL == videoInfo) {
        throw SDLException(ExInfo("GetVideoInfo"));
    }

    // the flags to pass to SDL_SetVideoMode
    videoFlags  = 0;
    videoFlags |= SDL_HWPALETTE;
    //NOTE: any bpp
    videoFlags |= SDL_ANYFORMAT;
    //TODO: can we set RLE in videoMode? Is RLE used in IMG_Load?
    //videoFlags |= SDL_RLEACCEL;

    // This checks to see if surfaces can be stored in memory
    if (videoInfo->hw_available) {
        videoFlags |= SDL_HWSURFACE;
        videoFlags |= SDL_DOUBLEBUF;
    }
    else {
        videoFlags |= SDL_SWSURFACE;
    }

    // This checks if hardware blits can be done
    if (videoInfo->blit_hw) {
        videoFlags |= SDL_HWACCEL;
    }

    return videoFlags;
}
//-----------------------------------------------------------------
/**
 *  Toggle fullscreen.
 */
    void
VideoAgent::toggleFullScreen()
{
    SDL_WM_ToggleFullScreen(m_screen);
    m_fullscreen = !m_fullscreen;
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - fullscreen ... toggle fullscreen
 *
 * @throws UnknownMsgException
 */
void
VideoAgent::receiveSimple(const SimpleMsg *msg)
{
    if (msg->equalsName("fullscreen")) {
        OptionAgent *options = OptionAgent::agent();
        if (options->getAsInt("fullscreen")) {
            options->setParam("fullscreen", "0");
        }
        else {
            options->setParam("fullscreen", "1");
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}
//-----------------------------------------------------------------
/**
 * Handle incoming message.
 * Messages:
 * - param_changed(fullscreen) ... handle fullscreen
 * - param_changed(screen_width) ... handle screen_width
 * - param_changed(screen_height) ... handle screen_height
 * - param_changed(screen_bpp) ... handle screen_bpp
 *
 * @throws UnknownMsgException
 */
void
VideoAgent::receiveString(const StringMsg *msg)
{
    if (msg->equalsName("param_changed")) {
        std::string param = msg->getValue();
        if ("fullscreen" == param) {
            bool fs = OptionAgent::agent()->getAsInt("fullscreen");
            if (fs != m_fullscreen) {
                toggleFullScreen();
            }
        }
        else if ("screen_width" == param
                || "screen_height" == param
                || "screen_bpp" == param)
        {
            initVideoMode();
        }
    }
    else {
        throw UnknownMsgException(msg);
    }
}

//-----------------------------------------------------------------
/**
 * Store pointer to drawer at the end of draw list.
 * Drawer will obtain pointer to screen.
 */
void
VideoAgent::acceptDrawer(IDrawer *drawer)
{
    m_drawers.push_back(drawer);
    drawer->takeScreen(m_screen);
}
//-----------------------------------------------------------------
/**
 * Remove drawer from list.
 * Drawer will not be deleted.
 */
void
VideoAgent::removeDrawer(const IDrawer *drawer)
{
    t_drawers::iterator end = m_drawers.end();
    for (t_drawers::iterator i = m_drawers.begin(); i != end; ++i) {
        if (*i == drawer) {
            m_drawers.erase(i);
            return;
        }
    }
}
//-----------------------------------------------------------------
/**
 * Switch drawers to new screen.
 */
void
VideoAgent::updateDrawersScreen()
{
    t_drawers::iterator end = m_drawers.end();
    for (t_drawers::iterator i = m_drawers.begin(); i != end; ++i) {
        (*i)->takeScreen(m_screen);
    }
}



/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "SysVideo.h"

#include "Log.h"

#include "SDL.h"
#ifdef HAVE_X11
#include "SDL_syswm.h"
#include <X11/Xutil.h>
#endif

//-----------------------------------------------------------------
/**
 * Set window title.
 * @param title UTF-8 string
 */
    void
SysVideo::setCaption(const std::string &title)
{
    bool done = false;
#ifdef HAVE_X11
#ifdef X_HAVE_UTF8_STRING
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);

    if (SDL_GetWMInfo(&info) > 0) {
        if (info.subsystem == SDL_SYSWM_X11) {
            info.info.x11.lock_func();

            XTextProperty titleprop;
            char *text_list = const_cast<char*>(title.c_str());
            int error = Xutf8TextListToTextProperty(info.info.x11.display,
                        &text_list, 1, XUTF8StringStyle, &titleprop);
            if (!error) {
                XSetWMName(info.info.x11.display, info.info.x11.wmwindow,
                        &titleprop);
                XFree(titleprop.value);
                done = true;
            }
            else {
                LOG_DEBUG(ExInfo("not supported conversion")
                        .addInfo("error", error)
                        .addInfo("title", title));
            }

            info.info.x11.unlock_func();
        }
    }
#endif
#endif

    if (!done) {
        SDL_WM_SetCaption(title.c_str(), NULL);
    }
}


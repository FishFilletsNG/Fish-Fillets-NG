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
#if !defined(HAVE_X11) && !defined(WIN32)
    void
SysVideo::setCaption(const std::string &title)
{
    SDL_WM_SetCaption(title.c_str(), NULL);
}
#else
#include "SDL_syswm.h"
static bool sysSetCaption(SDL_SysWMinfo *info, const std::string &title);

//-----------------------------------------------------------------
/**
 * Set window title.
 * @param title UTF-8 string
 */
    void
SysVideo::setCaption(const std::string &title)
{
    bool done = false;
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWMInfo(&info) > 0) {
        done = sysSetCaption(&info, title);
    }

    if (!done) {
        SDL_WM_SetCaption(title.c_str(), NULL);
    }
}
//-----------------------------------------------------------------
/**
 * Set window title.
 * @param info system dependent info
 * @param title utf-8 string
 * @return true for success
 */
#if defined(HAVE_X11) && !defined(DISABLE_X11) && defined(unix)
#include <X11/Xutil.h>
bool
sysSetCaption(SDL_SysWMinfo *info, const std::string &title)
{
    bool result = false;
#ifdef X_HAVE_UTF8_STRING
    if (info->subsystem == SDL_SYSWM_X11) {
        info->info.x11.lock_func();

        XTextProperty titleprop;
        char *text_list = const_cast<char*>(title.c_str());
        int error = Xutf8TextListToTextProperty(info->info.x11.display,
                &text_list, 1, XUTF8StringStyle, &titleprop);
        if (!error) {
            XSetWMName(info->info.x11.display, info->info.x11.wmwindow,
                    &titleprop);
            XFree(titleprop.value);
            result = true;
        }
        else {
            LOG_DEBUG(ExInfo("not supported conversion")
                    .addInfo("error", error)
                    .addInfo("title", title));
        }

        info->info.x11.unlock_func();
    }
#endif
    return result;
}
#elif defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
bool
sysSetCaption(SDL_SysWMinfo *info, const std::string &title)
{
    bool result = false;
    LPWSTR lpszW = new WCHAR[title.size()];
    if (MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1,
                lpszW, title.size()))
    {
        result = SetWindowTextW(info->window, lpszW);
    }
    else {
        LOG_DEBUG(ExInfo("not supported conversion")
                .addInfo("error", GetLastError())
                .addInfo("title", title));
    }
    delete[] lpszW;
    return result;
}
#else
bool
sysSetCaption(SDL_SysWMinfo * /*info*/, const std::string &/*title*/)
{
    return false;
}
#endif
#endif // HAVE_X11 || WIN32


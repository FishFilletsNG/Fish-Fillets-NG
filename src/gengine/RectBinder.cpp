/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "RectBinder.h"

#include "Log.h"
#include "BaseMsg.h"
#include "StringTool.h"
#include "NameException.h"

#include <string>

//-----------------------------------------------------------------
RectBinder::~RectBinder()
{
    t_rects::iterator end = m_rects.end();
    for (t_rects::iterator i = m_rects.begin(); i != end; ++i) {
        delete i->second;
    }
}
//-----------------------------------------------------------------
/**
 * Bind mouse button press on SDL_Rect to message action.
 * Rects are layered according thier addition,
 * the last added will be checked fist.
 *
 * @param rect rect
 * @param msg message to raise, will be deleted
 */
    void
RectBinder::addRect(const SDL_Rect &rect, BaseMsg *msg)
{
    m_rects.push_front(std::pair<SDL_Rect,BaseMsg*>(rect, msg));
    LOG_DEBUG(ExInfo("binding rect")
                .addInfo("rect", rectToString(rect))
                .addInfo("msg", msg->toString()));
}
//-----------------------------------------------------------------
    std::string
RectBinder::rectToString(const SDL_Rect &rect) const
{
    std::string result = "[" + StringTool::toString(rect.x);
    result.append("+" + StringTool::toString(rect.w) + "]x[");
    result.append(StringTool::toString(rect.y) + "+");
    result.append(StringTool::toString(rect.h) + "]");
    return result;
}
//-----------------------------------------------------------------
/**
 * Find rect in which left mouse button was pressed.
 * The last added rect will be checked fist
 * (it is in the front).
 * Only one rect is activated.
 * When listener is not available, it will be removed.
 *
 * NOTE: only useable for small count of rects.
 */
void
RectBinder::lbuttonDown(const SDL_MouseButtonEvent &button)
{
    t_rects::iterator end = m_rects.end();
    for (t_rects::iterator i = m_rects.begin(); i != end; ++i) {
        const SDL_Rect &rect = i->first;
        if (button.x >= rect.x && button.x <= rect.x + rect.w
            && button.y >= rect.y && button.y <= rect.y + rect.h)
        {
            try {
                i->second->sendClone();
            }
            catch (NameException &e) {
                LOG_WARNING(e.info());
                delete i->second;
                m_rects.erase(i);
            }
            return;
        }
    }
}


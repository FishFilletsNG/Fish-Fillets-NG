/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MessagerAgent.h"

#include "Log.h"
#include "BaseMsg.h"
#include "BaseListener.h"
#include "NameException.h"

#include <memory> // for auto_ptr

//-----------------------------------------------------------------
/**
 * Remember listener under his name.
 * NOTE: listeners may be added before own_init()
 *
 * @param listener listener, borowed pointer
 */
    void
MessagerAgent::addListener(BaseListener *listener)
{
    m_listeners[listener->getName()] = listener;
}
//-----------------------------------------------------------------
/**
 * Forget listener.
 * @param listenerName listener name
 */
    void
MessagerAgent::removeListener(const std::string &listenerName)
{
    m_listeners.erase(listenerName);
}
//-----------------------------------------------------------------
/**
 * Forward message to her destination.
 * @param msg message, will be deleted
 * @throws NameException when listener cannot be found
 */
void
MessagerAgent::forwardNewMsg(BaseMsg *msg)
{
    std::auto_ptr<BaseMsg> sure_delete(msg);

    const std::string &listenerName = msg->getListenerName();
    LOG_DEBUG(ExInfo("received new message")
            .addInfo("msg", msg->toString()));

    t_listeners::iterator it = m_listeners.find(listenerName);
    if (m_listeners.end() == it) {
        throw NameException(ExInfo("cannot find listener")
                .addInfo("name", listenerName));
    }

    msg->sendActual(it->second);
}



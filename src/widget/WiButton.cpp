/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WiButton.h"

#include "BaseMsg.h"
#include "MouseStroke.h"

//-----------------------------------------------------------------
/**
 * Create button which will send given message on mouse press.
 */
    WiButton::WiButton(IWidget *new_content, BaseMsg *new_msg)
: WiContainer(new_content)
{
    m_msg = new_msg;
}
//-----------------------------------------------------------------
/**
 * Release remain message.
 */
WiButton::~WiButton()
{
    delete m_msg;
}
//-----------------------------------------------------------------
    void
WiButton::own_mouseButton(const MouseStroke &stroke)
{
    if (stroke.isLeft()) {
        m_msg->sendClone();
    }
}


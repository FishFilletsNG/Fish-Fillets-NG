/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WidgetContainer.h"

//-----------------------------------------------------------------
WidgetContainer::WidgetContainer(IWidget *new_widget)
{
    m_widget = new_widget;
}
//-----------------------------------------------------------------
WidgetContainer::~WidgetContainer()
{
    delete m_widget;
}
//-----------------------------------------------------------------
void
WidgetContainer::draw()
{
    m_widget->drawOn(m_screen);
}
//-----------------------------------------------------------------
void
WidgetContainer::mouseButton(const MouseStroke &stroke)
{
    m_widget->mouseButton(stroke);
}


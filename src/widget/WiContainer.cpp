/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "WiContainer.h"

//-----------------------------------------------------------------
/**
 * Create new container around given widget.
 * @param new_content subwidget inside
 * @param border border around subwidget
 */
WiContainer::WiContainer(IWidget *new_content, int border)
{
    m_content = new_content;
    m_border = border;
}
//-----------------------------------------------------------------
/**
 * Release subwidget.
 */
WiContainer::~WiContainer()
{
    delete m_content;
}
//-----------------------------------------------------------------
int
WiContainer::getW() const
{
    return 2 * m_border + m_content->getW();
}
//-----------------------------------------------------------------
int
WiContainer::getH() const
{
    return 2 * m_border + m_content->getH();
}
//-----------------------------------------------------------------
void
WiContainer::setShift(const V2 &shift)
{
    m_shift = shift;
    m_content->setShift(m_shift.plus(V2(m_border, m_border)));
}
//-----------------------------------------------------------------
/**
 * Let subwidget to draw.
 */
void
WiContainer::drawOn(SDL_Surface *screen)
{
    m_content->drawOn(screen);
}
//-----------------------------------------------------------------
/**
 * Default action is to propagate mouse press to subwidget.
 */
    void
WiContainer::own_mouseButton(const MouseStroke &stroke)
{
    m_content->mouseButton(stroke);
}


/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ModelList.h"

#include "View.h"
//-----------------------------------------------------------------
/**
 * Create new wrapper.
 * @param models wrapped models.
 */
ModelList::ModelList(const t_models *models)
{
    m_models = models;
}
//-----------------------------------------------------------------
void
ModelList::drawOn(View *view) const
{
    t_models::const_iterator end = m_models->end();
    for (t_models::const_iterator i = m_models->begin(); i != end; ++i) {
        view->drawModel(*i);
    }
}


/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MarkMask.h"

#include "Cube.h"
#include "Shape.h"
#include "Field.h"

#include <algorithm>

//-----------------------------------------------------------------
MarkMask::MarkMask(Cube *model, Field *field)
{
    m_model = model;
    m_field = field;
}
//-----------------------------------------------------------------
/**
 * Return others which resist us to move this direction.
 * Pointers to NULL and own model are not included.
 *
 * @return unique pointers, not NULL
 */
Cube::t_models
MarkMask::getResist(Rules::eDir dir) const
{
    Cube::t_models models;
    V2 shift = Rules::dir2xy(dir);
    V2 shift_loc = shift.composition(m_model->getLocation());

    const Shape *shape = m_model->shape();
    Shape::const_iterator end = shape->end();
    for (Shape::const_iterator i = shape->begin(); i != end; ++i) {
        V2 mark = shift_loc.composition(*i);

        Cube *resist = m_field->getModel(mark);
        if (NULL != resist && m_model != resist) {
            models.push_back(resist);
        }
    }

    std::sort(models.begin(), models.end());
    Cube::t_models::iterator last = std::unique(models.begin(), models.end());
    models.erase(last, models.end());

    return models;
}
//-----------------------------------------------------------------
/**
 * Write our position to the field.
 */
void
MarkMask::mask()
{
    writeModel(m_model);
}
//-----------------------------------------------------------------
/**
 * Clear our position from the field.
 */
void
MarkMask::unmask()
{
    writeModel(NULL);
}
//-----------------------------------------------------------------
void
MarkMask::writeModel(Cube *model)
{
    V2 loc = m_model->getLocation();
    const Shape *shape = m_model->shape();
    Shape::const_iterator end = shape->end();
    for (Shape::const_iterator i = shape->begin(); i != end; ++i) {
        V2 mark = loc.composition(*i);
        m_field->setModel(mark, model);
    }
}

//-----------------------------------------------------------------
/**
 * Returns true when object is at [0,.] or [.,0] or [w-1,.] or [.,h-1].
 * @return true when object go out
 */
    bool
MarkMask::isAtBorder()
{
    V2 loc = m_model->getLocation();
    const Shape *shape = m_model->shape();
    Shape::const_iterator end = shape->end();
    for (Shape::const_iterator i = shape->begin(); i != end; ++i) {
        V2 mark = loc.composition(*i);
        if ((mark.getX() == 0 || mark.getX() == m_field->getW() - 1)
                || (mark.getY() == 0 || mark.getY() == m_field->getH() - 1))
        {
            return true;
        }
    }

    return false;
}


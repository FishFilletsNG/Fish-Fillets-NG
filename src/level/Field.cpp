/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Field.h"

#include "V2.h"
#include "Cube.h"
#include "Rules.h"
#include "ModelFactory.h"

#include <string.h> // memset()

//-----------------------------------------------------------------
/**
 * Two dimensional array of Cube pointers.
 */
Field::Field(int w, int h)
{
    m_w = w;
    m_h = h;

    m_border = ModelFactory::createBorder();
    m_border->rules()->takeField(this);

    //NOTE: [y][x] indexes
    m_marks = new Cube**[m_h];
    for (int y = 0; y < m_h; ++y) {
        m_marks[y] = new Cube*[m_w];
        memset(m_marks[y], 0, sizeof(Cube *) * m_w);
    }
}
//-----------------------------------------------------------------
Field::~Field()
{
    for (int y = 0; y < m_h; ++y) {
        delete [] m_marks[y];
    }
    delete [] m_marks;
    delete m_border;
}
//-----------------------------------------------------------------
/**
 * Get model which occupied this location.
 * Empty locations are NULL filled.
 * Locations out of field are filled with border object.
 */
Cube *
Field::getModel(const V2 &loc)
{
    int x = loc.getX();
    int y = loc.getY();

    //NOTE: hack border everywhere in outher space
    Cube *result = m_border;
    if ((0 <= x && x < m_w) && (0 <= y && y < m_h)) {
        result = m_marks[y][x];
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Mark this location as occupied by model.
 * Locations out of field will not be filled.
 * @param loc write location
 * @param model model to put on given location 
 * @param toOverride allowed model to overwrite or NULL
 */
void
Field::setModel(const V2 &loc, Cube *model, Cube *toOverride)
{
    int x = loc.getX();
    int y = loc.getY();

    if ((0 <= x && x < m_w) && (0 <= y && y < m_h)) {
        if (toOverride == NULL || m_marks[y][x] == toOverride) {
            m_marks[y][x] = model;
        }
    }
}



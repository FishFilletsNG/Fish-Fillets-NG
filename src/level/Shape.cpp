/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "Shape.h"

#include "LayoutException.h"
#include "minmax.h"

//-----------------------------------------------------------------
/**
 * Read shape in format:
 * "XXX...\n"
 * ".XXXXX\n"
 * "XX...X\n"
 *
 * NOTE: rows does not need to have the same length
 *
 * @throws LayoutException when shape has bad format
 */
Shape::Shape(const std::string &shape)
{
    int x = 0;
    int y = 0;
    int max_x = -1;
    int max_y = -1;

    for (unsigned int i = 0; i < shape.size(); ++i) {
        switch (shape[i]) {
            case '\n':
                ++y;
                x = 0;
                break;
            case 'X':
                m_marks.push_back(V2(x, y));
                max_x = max(max_x, x);
                max_y = max(max_y, y);
                ++x;
                break;
            case '.':
                ++x;
                break;
            default:
                throw LayoutException(ExInfo("bad shape char")
                        .addInfo("char", shape[i])
                        .addInfo("shape", shape));
        }
    }

    m_w = max_x + 1;
    m_h = max_y + 1;
}

//-----------------------------------------------------------------
std::string
Shape::toString() const
{
    std::string result;

    t_marks::const_iterator end = m_marks.end();
    for (t_marks::const_iterator i = m_marks.begin(); i != end; ++i) {
        result.append(i->toString());
    }
    return result;
}





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

//-----------------------------------------------------------------
/**
 * Read shape in format:
 * "XXX   \n"
 * " XXXXX\n"
 * "XX   X\n"
 *
 * NOTE: rows does not need to have the same length
 *
 * @throws LayoutException when shape has bad format
 */
Shape::Shape(const std::string &shape)
{
    int x = 0;
    int y = 0;

    for (unsigned int i = 0; i < shape.size(); ++i) {
        switch (shape[i]) {
            case '\n':
                ++y;
                x = 0;
                break;
            case 'X':
                m_marks.push_back(V2(x, y));
                ++x;
                break;
            case ' ':
                ++x;
                break;
            default:
                throw LayoutException(ExInfo("bad shape char")
                        .addInfo("char", shape[i])
                        .addInfo("shape", shape));
        }
    }
}

//-----------------------------------------------------------------
std::string
Shape::toString() const
{
    std::string result;

    const_iterator this_end = end();
    for (const_iterator i = begin(); i != this_end; ++i) {
        result.append(i->toString());
    }
    return result;
}





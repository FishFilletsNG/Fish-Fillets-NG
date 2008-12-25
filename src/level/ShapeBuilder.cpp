/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ShapeBuilder.h"

#include "Shape.h"
#include "View.h"
#include "SurfaceTool.h"

//-----------------------------------------------------------------
/**
 * Prepare color values
 * based on the given shape and weight.
 */
    void
ShapeBuilder::prepareColor(SDL_Color *color, const Shape *shape,
        Cube::eWeight weight)
{
    if (NULL == color) {
        return;
    }

    color->r = 0;
    color->g = 0;
    color->b = 0;
    color->unused = 255;

    switch (weight) {
        case Cube::LIGHT:
            color->g = ShapeBuilder::calcShapeHash(shape) % 255;
            color->r = 255 - color->g;
            break;
        case Cube::HEAVY:
            color->b = 50 + ShapeBuilder::calcShapeHash(shape) % (255 - 50);
            break;
        default:
            color->r = 128;
            color->g = 128;
            color->b = 128;
            break;
    }
}
//-----------------------------------------------------------------
/**
 * Calc an almost unique hash of the shape.
 */
    Uint32
ShapeBuilder::calcShapeHash(const Shape *shape)
{
    Uint32 hash = 0;
    Shape::const_iterator end = shape->marksEnd();
    for (Shape::const_iterator i = shape->marksBegin(); i != end; ++i) {
        hash = 31 * hash + i->getX();
        hash = 31 * hash + i->getY();
    }
    return hash;
}
//-----------------------------------------------------------------
/**
 * Create new image for the given shape.
 */
    SDL_Surface *
ShapeBuilder::createImage(const Shape *shape, Cube::eWeight weight)
{
    static const SDL_Color TRANSPARENT = {255, 0, 255, 255};

    SDL_Surface *surface = SurfaceTool::createTransparent(
            shape->getW() * View::SCALE, shape->getH() * View::SCALE,
            TRANSPARENT);

    SDL_Rect rect;
    rect.w = View::SCALE;
    rect.h = View::SCALE;

    SDL_Color color;
    prepareColor(&color, shape, weight);

    Shape::const_iterator end = shape->marksEnd();
    for (Shape::const_iterator i = shape->marksBegin(); i != end; ++i) {
        rect.x = i->getX() * View::SCALE;
        rect.y = i->getY() * View::SCALE;
        SurfaceTool::alphaFill(surface, &rect, color);
    }

    return surface;
}


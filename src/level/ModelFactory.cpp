/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ModelFactory.h"

#include "V2.h"
#include "Unit.h"
#include "Shape.h"
#include "LogicException.h"
#include "StringTool.h"

//-----------------------------------------------------------------
/**
 * Add model at scene.
 * @param kind kind of item (e.g. "fish_big", "item_light", ...)
 * @param loc placement location
 * @param shape see Shape for format
 * @return model
 *
 * @throws LogicException for unknown kind
 * @throws LayoutException when shape or location is bad
 */
Cube *
ModelFactory::createModel(const std::string &kind, const V2 &loc,
        const std::string &shape)
{
    if (StringTool::startsWith(kind, "output_")) {
        return createOutputItem(kind, loc, shape);
    }

    Cube::eWeight weight;
    Cube::eWeight power;
    bool alive;
    createParams(kind, &weight, &power, &alive);

    Shape *newShape = new Shape(shape);
    Cube *model = new Cube(loc,
            weight, power, alive, newShape);

    return model;
}
//-----------------------------------------------------------------
/**
 * Determine object params.
 * @throws LogicException when kind is unkown
 */
void
ModelFactory::createParams(const std::string &kind,
        Cube::eWeight *out_weight, Cube::eWeight *out_power, bool *out_alive)
{
    if ("fish_small" == kind) {
        *out_weight = Cube::LIGHT;
        *out_power = Cube::LIGHT;
        *out_alive = true;
    }
    else if ("fish_big" == kind) {
        *out_weight = Cube::LIGHT;
        *out_power = Cube::HEAVY;
        *out_alive = true;
    }
    else if (StringTool::startsWith(kind, "fish_extra")) {
        *out_weight = Cube::LIGHT;
        *out_power = Cube::LIGHT;
        *out_alive = true;
    }
    else if (StringTool::startsWith(kind, "fish_EXTRA")) {
        *out_weight = Cube::LIGHT;
        *out_power = Cube::HEAVY;
        *out_alive = true;
    }
    else {
        *out_power = Cube::NONE;
        *out_alive = false;
        if ("item_light" == kind) {
            *out_weight = Cube::LIGHT;
        }
        else if ("item_heavy" == kind) {
            *out_weight = Cube::HEAVY;
        }
        else if ("item_fixed" == kind) {
            *out_weight = Cube::FIXED;
        }
        else {
            throw LogicException(ExInfo("unknown model kind")
                    .addInfo("kind", kind));
        }
    }
}
//-----------------------------------------------------------------
/**
 * Create unit for driveable fish.
 * @param kind kind of item (e.g. "fish_big", "item_light", ...)
 * @return new unit or NULL
 */
Unit *
ModelFactory::createUnit(const std::string &kind)
{
    Unit *result = NULL;
    if ("fish_small" == kind) {
        KeyControl smallfish;
        smallfish.setUp(SDLK_i);
        smallfish.setDown(SDLK_k);
        smallfish.setLeft(SDLK_j);
        smallfish.setRight(SDLK_l);
        result = new Unit(smallfish, ControlSym('u', 'd', 'l', 'r'), true);
    }
    else if ("fish_big" == kind) {
        KeyControl bigfish;
        bigfish.setUp(SDLK_w);
        bigfish.setDown(SDLK_s);
        bigfish.setLeft(SDLK_a);
        bigfish.setRight(SDLK_d);
        result = new Unit(bigfish, ControlSym('U', 'D', 'L', 'R'));
    }
    else if (StringTool::startsWith(kind, "fish_extra") ||
        StringTool::startsWith(kind, "fish_EXTRA"))
    {
        KeyControl extrafish;
        extrafish.setUp(SDLK_LAST);
        extrafish.setDown(SDLK_LAST);
        extrafish.setLeft(SDLK_LAST);
        extrafish.setRight(SDLK_LAST);
        result = new Unit(extrafish, parseExtraControlSym(kind));
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Create special model, which will be used for outher space.
 * NOTE: hack border around field
 */
    Cube *
ModelFactory::createBorder()
{
    Cube *border = new Cube(V2(-1,-1), Cube::FIXED, Cube::NONE, false,
            new Shape("X\n"));
    return border;
}
//-----------------------------------------------------------------
/**
 * Create one way output out of room.
 * @throws LogicException when output_DIR is not known
 */
Cube *
ModelFactory::createOutputItem(const std::string &kind, const V2 &loc,
        const std::string &shape)
{
    Dir::eDir outDir = Dir::DIR_NO;
    if ("output_left" == kind) {
        outDir = Dir::DIR_LEFT;
    }
    else if ("output_right" == kind) {
        outDir = Dir::DIR_RIGHT;
    }
    else if ("output_up" == kind) {
        outDir = Dir::DIR_UP;
    }
    else if ("output_down" == kind) {
        outDir = Dir::DIR_DOWN;
    }
    else {
        throw LogicException(ExInfo("unknown border dir")
                .addInfo("kind", kind));
    }

    Cube *model = new Cube(loc,
            Cube::FIXED, Cube::NONE, false, new Shape(shape));
    model->setOutDir(outDir);
    return model;
}
//-----------------------------------------------------------------
/**
 * Define controls symbols for extra fish.
 * Format: "fish_extra-UDLR"
 * @throws LogicException when symbols are not specified
 */
ControlSym
ModelFactory::parseExtraControlSym(const std::string &kind)
{
    static const std::string PREFIX = "fish_extra-";
    if (kind.size() != PREFIX.size() + 4) {
        throw LogicException(ExInfo("you must specify control symbols")
                .addInfo("kind", kind));
    }

    char up = kind[PREFIX.size()];
    char down = kind[PREFIX.size() + 1];
    char left = kind[PREFIX.size() + 2];
    char right = kind[PREFIX.size() + 3];
    return ControlSym(up, down, left, right);
}


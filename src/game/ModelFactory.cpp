/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "ModelFactory.h"

#include "Unit.h"
#include "Shape.h"
#include "View.h"
#include "LogicException.h"

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
    Cube::eWeight weight;
    Cube::eWeight power;
    bool alive;
    createParams(kind, &weight, &power, &alive);

    Shape *newShape = new Shape(shape);
    View *newView = new View();
    Cube *model = new Cube(loc,
            weight, power, alive,
            newView, newShape);

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
    return result;
}

/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "game-script.h"

#include "Log.h"
#include "KeyControl.h"
#include "Path.h"
#include "V2.h"
#include "Anim.h"
#include "Shape.h"
#include "Cube.h"
#include "Rules.h"
#include "LevelScript.h"
#include "ModelFactory.h"

#include "def-script.h"

//-----------------------------------------------------------------
    inline LevelScript *
getLevelScript(lua_State *L)
{
    return static_cast<LevelScript*>(script_getLeader(L));
}
//-----------------------------------------------------------------
    inline Cube *
getModel(lua_State *L, int model_index)
{
    return getLevelScript(L)->getModel(model_index);
}

//-----------------------------------------------------------------
/**
 * int game_addModel(kind, x, y, shape)
 * Return model index.
 *
 *  table = addModel("light", 10, 30, "table.bmp",
 *  [[
 *  XXXXX
 *  ..X
 *  ..X
 *  ]])
 */
    int
script_game_addModel(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *kind = luaL_checkstring(L, 1);
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    const char *shape = luaL_checkstring(L, 4);

    Cube *model = ModelFactory::createModel(kind, V2(x, y), shape);
    Unit *unit = ModelFactory::createUnit(kind);
    int model_index = getLevelScript(L)->addModel(model, unit);
    lua_pushnumber(L, model_index);
    END_NOEXCEPTION;
    //NOTE: return model_index
    return 1;
}
//-----------------------------------------------------------------
/**
 * int game_getCycles()
 */
    int
script_game_getCycles(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int cycles = getLevelScript(L)->getCycles();
    lua_pushnumber(L, cycles);
    END_NOEXCEPTION;
    //NOTE: return cycles
    return 1;
}

//-----------------------------------------------------------------
/**
 * void model_addAnim(model_index, anim_name, picture)
 */
    int
script_model_addAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    const char *picture = luaL_checkstring(L, 3);

    Cube *model = getModel(L, model_index);
    model->anim()->addAnim(anim_name, Path::dataReadPath(picture));
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_addDuplexAnim(model_index, anim_name, left_picture, right_picture)
 */
    int
script_model_addDuplexAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    const char *left_picture = luaL_checkstring(L, 3);
    const char *right_picture = luaL_checkstring(L, 4);

    Cube *model = getModel(L, model_index);
    model->anim()->addDuplexAnim(anim_name,
            Path::dataReadPath(left_picture),
            Path::dataReadPath(right_picture));
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_runAnim(model_index, anim_name, phase=0)
 */
    int
script_model_runAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    int phase = luaL_optint(L, 3, 0);

    Cube *model = getModel(L, model_index);
    model->anim()->runAnim(anim_name, phase);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_setAnim(model_index, anim_name, phase)
 */
    int
script_model_setAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    int phase = luaL_checkint(L, 3);

    Cube *model = getModel(L, model_index);
    model->anim()->setAnim(anim_name, phase);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_useSpecialAnim(model_index, anim_name, phase)
 *
 * Set special anim for one phase.
 */
    int
script_model_useSpecialAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    int phase = luaL_checkint(L, 3);

    Cube *model = getModel(L, model_index);
    model->anim()->useSpecialAnim(anim_name, phase);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_setEffect(model_index, effect_name)
 *
 * Set special view effect.
 * available effects: "none", "mirror", "invisible"
 */
    int
script_model_setEffect(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    std::string effect_name = luaL_checkstring(L, 2);

    Cube *model = getModel(L, model_index);
    if ("none" == effect_name) {
        model->anim()->setEffect(ViewEffect::EFFECT_NONE);
    }
    else if ("mirror" == effect_name) {
        model->anim()->setEffect(ViewEffect::EFFECT_MIRROR);
    }
    else if ("invisible" == effect_name) {
        model->anim()->setEffect(ViewEffect::EFFECT_INVISIBLE);
    }
    else if ("reverse" == effect_name) {
        model->anim()->setEffect(ViewEffect::EFFECT_REVERSE);
    }
    else {
        ExInfo error = ExInfo("unknown view effect")
            .addInfo("effect", effect_name);
        LOG_WARNING(error);
        luaL_error(L, error.what());
    }
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * (x, y) model_getLoc(model_index)
 */
    int
script_model_getLoc(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);

    Cube *model = getModel(L, model_index);
    V2 loc = model->getLocation();

    lua_pushnumber(L, loc.getX());
    lua_pushnumber(L, loc.getY());
    END_NOEXCEPTION;
    //NOTE: return (x, y)
    return 2;
}

//-----------------------------------------------------------------
/**
 * string model_getAction(model_index)
 */
    int
script_model_getAction(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    std::string action = model->rules()->getAction();

    lua_pushlstring(L, action.c_str(), action.size());
    END_NOEXCEPTION;
    //NOTE: return action
    return 1;
}
//-----------------------------------------------------------------
/**
 * string model_getState(model_index)
 */
    int
script_model_getState(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    std::string state = model->rules()->getState();

    lua_pushlstring(L, state.c_str(), state.size());
    END_NOEXCEPTION;
    //NOTE: return state
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool model_isAlive(model_index)
 */
    int
script_model_isAlive(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    bool alive = model->isAlive();

    lua_pushboolean(L, alive);
    END_NOEXCEPTION;
    //NOTE: return alive
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool model_isOut(model_index)
 *
 * Returns true when model is out of room.
 */
    int
script_model_isOut(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    bool out = model->isOut();

    lua_pushboolean(L, out);
    END_NOEXCEPTION;
    //NOTE: return out
    return 1;
}
//-----------------------------------------------------------------
/**
 * bool model_isLeft(model_index)
 *
 * Returns true when model is looking to the left.
 */
    int
script_model_isLeft(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    bool left = model->isLeft();

    lua_pushboolean(L, left);
    END_NOEXCEPTION;
    //NOTE: return left
    return 1;
}
//-----------------------------------------------------------------
/**
 * int model_getW(model_index)
 *
 * Returns model width.
 */
    int
script_model_getW(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    int width = model->shape()->getW();

    lua_pushnumber(L, width);
    END_NOEXCEPTION;
    //NOTE: return width
    return 1;
}
//-----------------------------------------------------------------
/**
 * int model_getH(model_index)
 *
 * Returns model height.
 */
    int
script_model_getH(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    int height = model->shape()->getH();

    lua_pushnumber(L, height);
    END_NOEXCEPTION;
    //NOTE: return height
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_setGoal(model_index, goalname)
 * Choose:
 * - "goal_no" .. no goal
 * - "goal_out" ... go out
 * - "goal_escape" ... go alive out
 */
    int
script_model_setGoal(lua_State *L) throw()
{
    //NOTE: (const char*)== does not compare string equality
    static const std::string GOAL_NO = "goal_no";
    static const std::string GOAL_OUT = "goal_out";
    static const std::string GOAL_ESCAPE = "goal_escape";

    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *goalname = luaL_checkstring(L, 2);

    Cube *model = getModel(L, model_index);
    Goal goal = Goal::noGoal();
    if (GOAL_NO == goalname) {
        goal = Goal::noGoal();
    }
    else if (GOAL_OUT == goalname) {
        goal = Goal::outGoal();
    }
    else if (GOAL_ESCAPE == goalname) {
        goal = Goal::escapeGoal();
    }
    else {
        ExInfo error = ExInfo("unknown goal")
            .addInfo("goal", goalname);
        LOG_WARNING(error);
        luaL_error(L, error.what());
    }

    model->setGoal(goal);

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_change_turnSide(model_index)
 *
 * Change look side.
 */
    int
script_model_change_turnSide(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    model->change_turnSide();

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_change_setLocation(model_index, x, y)
 *
 * Change X,Y.
 */
    int
script_model_change_setLocation(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    Cube *model = getModel(L, model_index);
    model->change_setLocation(V2(x, y));

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_setBusy(model_index, value)
 */
    int
script_model_setBusy(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    bool busy = luaL_checkint(L, 2);
    Cube *model = getModel(L, model_index);
    model->setBusy(busy);

    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * void model_equals(model_index, x, y)
 *
 * Returns whether object at location(x, y) is equal.
 * NOTE: model_index can be -1 for empty water.
 */
    int
script_model_equals(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    Cube *other = getLevelScript(L)->askField(V2(x, y));

    int other_index = -1;
    if (other) {
        other_index = other->getIndex();
    }
    bool equals = (other_index == model_index);

    lua_pushboolean(L, equals);
    END_NOEXCEPTION;
    //NOTE: return equals
    return 1;
}


//-----------------------------------------------------------------
/**
 * void sound_addSound(name, file)
 * 
 * Store this sound resource under this name.
 */
    int
script_sound_addSound(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    const char *file = luaL_checkstring(L, 2);

    getLevelScript(L)->addSound(name, Path::dataReadPath(file));
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void sound_playSound(name, volume)
 */
    int
script_sound_playSound(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *name = luaL_checkstring(L, 1);
    int volume = luaL_optint(L, 2, 100);

    getLevelScript(L)->playSound(name, volume);
    END_NOEXCEPTION;
    return 0;
}


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
#include "Room.h"
#include "RopeDecor.h"
#include "ShapeBuilder.h"

#include "def-script.h"

//-----------------------------------------------------------------
    inline LevelScript *
getLevelScript(lua_State *L)
{
    return dynamic_cast<LevelScript*>(script_getLeader(L));
}
//-----------------------------------------------------------------
    inline Cube *
getModel(lua_State *L, int model_index)
{
    return getLevelScript(L)->getModel(model_index);
}

//-----------------------------------------------------------------
/**
 * void game_setRoomWaves(amplitude, periode, speed)
 */
    int
script_game_setRoomWaves(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    float amp = luaL_checknumber(L, 1);
    float periode = luaL_checknumber(L, 2);
    float speed = luaL_checknumber(L, 3);

    getLevelScript(L)->room()->setWaves(amp, periode, speed);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * int game_addModel(kind, x, y, shape)
 * Return model index.
 *
 *  table = addModel("light", 10, 30,
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
    int cycles = getLevelScript(L)->room()->getCycles();
    lua_pushnumber(L, cycles);
    END_NOEXCEPTION;
    //NOTE: return cycles
    return 1;
}
//-----------------------------------------------------------------
/**
 * void game_addDecor(decor_name, params...)
 *
 * decor_name:
 * "rope" ... draw rope between models
 *          params = (model_index1, model_index2,
 *              shift_x1, shift_y1, shift_x2, shift_y2)
 */
    int
script_game_addDecor(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    std::string decor_name = luaL_checkstring(L, 1);
    if ("rope" == decor_name) {
        int model_index1 = luaL_checkint(L, 2);
        int model_index2 = luaL_checkint(L, 3);
        int shift_x1 = luaL_checkint(L, 4);
        int shift_y1 = luaL_checkint(L, 5);
        int shift_x2 = luaL_checkint(L, 6);
        int shift_y2 = luaL_checkint(L, 7);

        Cube *model1 = getModel(L, model_index1);
        Cube *model2 = getModel(L, model_index2);
        getLevelScript(L)->room()->addDecor(new RopeDecor(model1, model2,
                    V2(shift_x1, shift_y1), V2(shift_x2, shift_y2)));
    }
    else {
        LOG_WARNING(ExInfo("unknown decor")
                .addInfo("decor_name", decor_name));
    }

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void game_setScreenShift(x, y)
 */
    int
script_game_setScreenShift(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int x = luaL_checkint(L, 1);
    int y = luaL_checkint(L, 2);
    getLevelScript(L)->room()->setScreenShift(V2(x, y));
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void game_changeBg(picture)
 */
    int
script_game_changeBg(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *picture = luaL_checkstring(L, 1);
    getLevelScript(L)->room()->changeBg(picture);
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * string game_getBg()
 * Return the name of the currently used bg picture.
 */
    int
script_game_getBg(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    lua_pushstring(L, getLevelScript(L)->room()->getBg().c_str());
    END_NOEXCEPTION;
    //NOTE: return bg
    return 1;
}
//-----------------------------------------------------------------
/**
 * void game_checkActive()
 * Check active fish, switch to non busy alive fish.
 */
    int
script_game_checkActive(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    getLevelScript(L)->room()->checkActive();
    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void game_setFastFalling(value)
 * Value==true sets fast falling for all objets.
 */
    int
script_game_setFastFalling(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    bool value = lua_toboolean(L, 1);
    getLevelScript(L)->room()->setFastFalling(value);
    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * void model_addAnim(model_index, anim_name, picture, lookDir)
 * Sides:
 * LOOK_LEFT = 0
 * LOOK_RIGHT = 1
 */
    int
script_model_addAnim(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);
    const char *picture = luaL_checkstring(L, 3);
    Anim::eSide lookDir = static_cast<Anim::eSide>(
            luaL_optint(L, 4, Anim::SIDE_LEFT));

    Cube *model = getModel(L, model_index);
    if (picture[0] == '\0') {
        model->anim()->addAnim(anim_name,
                ShapeBuilder::createImage(model->shape(), model->getWeight()),
                lookDir);
    } else {
        model->anim()->addAnim(anim_name, Path::dataReadPath(picture), lookDir);
    }
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
 * int model_countAnims(model_index, anim_name)
 */
    int
script_model_countAnims(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    const char *anim_name = luaL_checkstring(L, 2);

    Cube *model = getModel(L, model_index);
    int anims = model->anim()->countAnimPhases(anim_name);
    lua_pushnumber(L, anims);
    END_NOEXCEPTION;
    //NOTE: return anims
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_setEffect(model_index, effect_name)
 *
 * Set special view effect.
 * available effects: "none", "mirror", "invisible", "reverse", "zx"
 */
    int
script_model_setEffect(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    std::string effectName = luaL_checkstring(L, 2);
    Cube *model = getModel(L, model_index);
    model->anim()->setEffect(effectName);

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
 * Dir::eDir model_getDir(model_index)
 */
    int
script_model_getDir(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    Dir::eDir dir = model->getLastMoveDir();

    lua_pushnumber(L, dir);
    END_NOEXCEPTION;
    //NOTE: return dir
    return 1;
}
//-----------------------------------------------------------------
/**
 * Dir::eDir model_getTouchDir(model_index)
 */
    int
script_model_getTouchDir(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    Dir::eDir dir = model->rules()->getTouchDir();

    lua_pushnumber(L, dir);
    END_NOEXCEPTION;
    //NOTE: return dir
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
 * bool model_isAtBorder(model_index)
 *
 * Returns true when model is at room border.
 */
    int
script_model_isAtBorder(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    bool atBorder = model->rules()->isAtBorder();

    lua_pushboolean(L, atBorder);
    END_NOEXCEPTION;
    //NOTE: return atBorder
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
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    std::string goalname = luaL_checkstring(L, 2);

    Cube *model = getModel(L, model_index);
    Goal goal = Goal::noGoal();
    if ("goal_no" == goalname) {
        goal = Goal::noGoal();
    }
    else if ("goal_out" == goalname) {
        goal = Goal::outGoal();
    }
    else if ("goal_escape" == goalname) {
        goal = Goal::escapeGoal();
    }
    else if ("goal_alive" == goalname) {
        goal = Goal::aliveGoal();
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
 * Change model position (used to load undo).
 */
    int
script_model_change_setLocation(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    Cube *model = getModel(L, model_index);
    model->rules()->change_setLocation(V2(x, y));

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * void model_setViewShift(model_index, shift_x, shift_y)
 * Shift view (used for obsolete animation effects).
 */
    int
script_model_setViewShift(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    int shift_x = luaL_checkint(L, 2);
    int shift_y = luaL_checkint(L, 3);
    Cube *model = getModel(L, model_index);
    model->anim()->setViewShift(V2(shift_x, shift_y));

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * shift_x, shift_y model_getViewShift(model_index)
 */
    int
script_model_getViewShift(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);
    V2 shift = model->anim()->getViewShift();

    lua_pushnumber(L, shift.getX());
    lua_pushnumber(L, shift.getY());
    END_NOEXCEPTION;
    //NOTE: return shift_x, shift_y
    return 2;
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
    bool busy = lua_toboolean(L, 2);
    Cube *model = getModel(L, model_index);
    model->setBusy(busy);

    END_NOEXCEPTION;
    return 0;
}
//-----------------------------------------------------------------
/**
 * table model_getExtraParams(model_index)
 *
 * Returns extra parameters needed to restore after undo.
 */
    int
script_model_getExtraParams(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    Cube *model = getModel(L, model_index);

    lua_newtable(L);
    lua_pushstring(L, "outDir");
    lua_pushnumber(L, model->getOutDir());
    lua_settable(L, -3);

    lua_pushstring(L, "outCapacity");
    lua_pushnumber(L, model->getOutCapacity());
    lua_settable(L, -3);

    lua_pushstring(L, "weight");
    lua_pushnumber(L, model->getWeight());
    lua_settable(L, -3);

    lua_pushstring(L, "anim");
    lua_pushstring(L, model->anim()->getState().c_str());
    lua_settable(L, -3);

    END_NOEXCEPTION;
    return 1;
}
//-----------------------------------------------------------------
/**
 * void model_change_setExtraParams(model_index, table)
 *
 * Restores extra parameters after undo.
 */
    int
script_model_change_setExtraParams(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);

    lua_pushstring(L, "outDir");
    lua_gettable(L, 2);
    int outDir = luaL_checkint(L, -1);

    lua_pushstring(L, "outCapacity");
    lua_gettable(L, 2);
    int outCapacity = luaL_checkint(L, -1);

    lua_pushstring(L, "weight");
    lua_gettable(L, 2);
    int weight = luaL_checkint(L, -1);

    lua_pushstring(L, "anim");
    lua_gettable(L, 2);
    std::string animState = luaL_checkstring(L, -1);

    Cube *model = getModel(L, model_index);
    model->setOutDir((Dir::eDir)outDir, outCapacity, (Cube::eWeight)weight);
    model->setExtraParams();
    model->anim()->restoreState(animState);

    END_NOEXCEPTION;
    return 0;
}

//-----------------------------------------------------------------
/**
 * void model_equals(model_index, x, y)
 *
 * Returns whether object at location(x, y) is equal.
 * NOTE: model_index can be -1 for empty water.
 * NOTE: boder is as wall (even thought border.index == -1)
 */
    int
script_model_equals(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    int model_index = luaL_checkint(L, 1);
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    Cube *other = getLevelScript(L)->askField(V2(x, y));

    bool equals = false;
    if (other) {
        if (model_index == -1) {
            equals = false;
        }
        else {
            equals = (model_index == other->getIndex());
        }
    }
    else {
        if (model_index == -1) {
            equals = true;
        }
    }

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


#ifndef HEADER_GAME_SCRIPT_H
#define HEADER_GAME_SCRIPT_H

extern "C" {
#include "lua.h"
}

extern int script_game_createRoom(lua_State *L) throw();
extern int script_game_addModel(lua_State *L) throw();
extern int script_game_save(lua_State *L) throw();
extern int script_game_load(lua_State *L) throw();

extern int script_game_action_move(lua_State *L) throw();
extern int script_game_action_save(lua_State *L) throw();
extern int script_game_action_load(lua_State *L) throw();
extern int script_game_action_restart(lua_State *L) throw();

extern int script_model_addAnim(lua_State *L) throw();
extern int script_model_addDuplexAnim(lua_State *L) throw();
extern int script_model_runAnim(lua_State *L) throw();
extern int script_model_setAnim(lua_State *L) throw();
extern int script_model_useSpecialAnim(lua_State *L) throw();
extern int script_model_getLoc(lua_State *L) throw();
extern int script_model_getAction(lua_State *L) throw();
extern int script_model_getState(lua_State *L) throw();
extern int script_model_isAlive(lua_State *L) throw();
extern int script_model_isOut(lua_State *L) throw();
extern int script_model_isLeft(lua_State *L) throw();
extern int script_model_getW(lua_State *L) throw();
extern int script_model_getH(lua_State *L) throw();
extern int script_model_setGoal(lua_State *L) throw();
extern int script_model_change_turnSide(lua_State *L) throw();
extern int script_model_equals(lua_State *L) throw();

extern int script_game_getRestartCounter(lua_State *L) throw();
extern int script_game_getDepth(lua_State *L) throw();
extern int script_game_getCycles(lua_State *L) throw();
extern int script_game_newDemo(lua_State *L) throw();
extern int script_sound_addSound(lua_State *L) throw();
extern int script_sound_playSound(lua_State *L) throw();

#endif

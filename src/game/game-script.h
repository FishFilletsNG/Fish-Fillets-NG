#ifndef HEADER_GAME_SCRIPT_H
#define HEADER_GAME_SCRIPT_H

extern "C" {
#include "lua.h"
}

/**
 * Fuctions for script.
 */
extern int script_file_include(lua_State *L) throw();
extern int script_file_exists(lua_State *L) throw();

extern int script_game_createRoom(lua_State *L) throw();
extern int script_game_addModel(lua_State *L) throw();
extern int script_game_getRestartCounter(lua_State *L) throw();

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

extern int script_dialog_empty(lua_State *L) throw();
extern int script_dialog_addFont(lua_State *L) throw();
extern int script_dialog_addDialog(lua_State *L) throw();
extern int script_model_isTalking(lua_State *L) throw();
extern int script_model_planDialog(lua_State *L) throw();

extern int script_timer_getCycles(lua_State *L) throw();
extern int script_sound_playMusic(lua_State *L) throw();
extern int script_sound_addSound(lua_State *L) throw();

#endif

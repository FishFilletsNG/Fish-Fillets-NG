#ifndef HEADER_GAME_SCRIPT_H
#define HEADER_GAME_SCRIPT_H

extern "C" {
#include "lua.h"
}

/**
 * Fuctions for ScriptAgent.
 */
extern int script_createRoom(lua_State *L) throw();
extern int script_addModel(lua_State *L) throw();
extern int script_model_addAnim(lua_State *L) throw();
extern int script_model_addDuplexAnim(lua_State *L) throw();
extern int script_model_setAnim(lua_State *L) throw();
extern int script_model_getLoc(lua_State *L) throw();
extern int script_model_getAction(lua_State *L) throw();
extern int script_model_isAlive(lua_State *L) throw();

#endif

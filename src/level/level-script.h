#ifndef HEADER_LEVEL_SCRIPT_H
#define HEADER_LEVEL_SCRIPT_H

extern "C" {
#include "lua.h"
}

extern int script_level_save(lua_State *L) throw();
extern int script_level_load(lua_State *L) throw();

extern int script_level_action_move(lua_State *L) throw();
extern int script_level_action_save(lua_State *L) throw();
extern int script_level_action_load(lua_State *L) throw();
extern int script_level_action_restart(lua_State *L) throw();

extern int script_level_createRoom(lua_State *L) throw();
extern int script_level_getRestartCounter(lua_State *L) throw();
extern int script_level_getDepth(lua_State *L) throw();
extern int script_level_isNewRound(lua_State *L) throw();
extern int script_level_isSolved(lua_State *L) throw();

extern int script_level_newDemo(lua_State *L) throw();
extern int script_level_planShow(lua_State *L) throw();
extern int script_level_isShowing(lua_State *L) throw();

#endif

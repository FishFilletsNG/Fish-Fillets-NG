#ifndef HEADER_OPTIONS_SCRIPT_H
#define HEADER_OPTIONS_SCRIPT_H

extern "C" {
#include "lua.h"
}

extern int script_options_sendMsg(lua_State *L) throw();
extern int script_options_setParam(lua_State *L) throw();
extern int script_options_getParam(lua_State *L) throw();

#endif

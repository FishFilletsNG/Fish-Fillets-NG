#ifndef HEADER_WORLDMAP_SCRIPT_H
#define HEADER_WORLDMAP_SCRIPT_H

extern "C" {
#include "lua.h"
}

/**
 * Fuctions for script.
 */
extern int script_worldmap_addDesc(lua_State *L) throw();

#endif

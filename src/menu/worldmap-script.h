#ifndef HEADER_WORLDMAP_SCRIPT_H
#define HEADER_WORLDMAP_SCRIPT_H

extern "C" {
#include "lua.h"
}

/**
 * Fuctions for worldmap.
 */
extern int script_worldmap_addDesc(lua_State *L) throw();
extern int script_branch_addNode(lua_State *L) throw();
extern int script_branch_setEnding(lua_State *L) throw();
extern int script_node_bestSolution(lua_State *L) throw();

#endif

#ifndef HEADER_WORLDWAY_H
#define HEADER_WORLDWAY_H

class LevelNode;

#include "NoCopy.h"
#include "Path.h"

extern "C" {
#include "lua.h"
}

/**
 * Can read way of level nodes.
 */
class WorldWay : public NoCopy {
    private:
        static LevelNode *ms_startNode;
    private:
        static void addChildren(lua_State *L, LevelNode *parent,
                int table_index);
        static LevelNode *parseNode(lua_State *L, int depth);
        static int script_parseWay(lua_State *L) throw();
    public:
        static LevelNode* createWay(const Path &file);
};

#endif

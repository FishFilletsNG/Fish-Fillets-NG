/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "demo-script.h"

#include "Path.h"
#include "DemoMode.h"
#include "Picture.h"

#include "def-script.h"

//-----------------------------------------------------------------
    inline DemoMode *
getDemo(lua_State *L)
{
    return dynamic_cast<DemoMode*>(script_getLeader(L));
}

//-----------------------------------------------------------------
/**
 * void demo_display(picture, x, y)
 */
    int
script_demo_display(lua_State *L) throw()
{
    BEGIN_NOEXCEPTION;
    const char *filename = luaL_checkstring(L, 1);
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);

    Picture *picture = new Picture(Path::dataReadPath(filename), V2(x, y));
    getDemo(L)->action_display(picture);
    END_NOEXCEPTION;
    return 0;
}


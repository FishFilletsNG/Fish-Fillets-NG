SFONT_CFLAGS = -I@top_srcdir@/src/SFont
SDL_GFX_CFLAGS = -I@top_srcdir@/src/SDL_gfx

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect -I@top_srcdir@/src/plan -I@top_srcdir@/src/level $(SFONT_CFLAGS) $(SDL_GFX_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS)

noinst_LIBRARIES = libmenu.a

libmenu_a_SOURCES = LevelDesc.cpp LevelDesc.h LevelNode.cpp LevelNode.h LuaTable.cpp LuaTable.h NodeDrawer.cpp NodeDrawer.h WorldMap.cpp WorldMap.h WorldWay.cpp WorldWay.h WorldInput.cpp WorldInput.h worldmap-script.cpp worldmap-script.h


SDL_GFX_CFLAGS = -I@top_srcdir@/src/SDL_gfx

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect -I@top_srcdir@/src/widget -I@top_srcdir@/src/plan -I@top_srcdir@/src/option -I@top_srcdir@/src/state -I@top_srcdir@/src/level $(SDL_GFX_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS) $(FRIBIDI_CFLAGS)

noinst_LIBRARIES = libmenu.a

libmenu_a_SOURCES = LevelDesc.cpp LevelDesc.h LevelNode.cpp LevelNode.h NodeDrawer.cpp NodeDrawer.h PedoInput.cpp PedoInput.h Pedometer.cpp Pedometer.h WorldInput.cpp WorldInput.h WorldMap.cpp WorldMap.h WorldBranch.cpp WorldBranch.h worldmap-script.cpp worldmap-script.h SolverDrawer.cpp SolverDrawer.h


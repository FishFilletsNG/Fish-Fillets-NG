SDL_GFX_CFLAGS = -I@top_srcdir@/src/SDL_gfx
SDL_GFX_LIBS = ../SDL_gfx/libSDL_gfx.a

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect -I@top_srcdir@/src/plan -I@top_srcdir@/src/level -I@top_srcdir@/src/menu $(SDL_GFX_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS)

bin_PROGRAMS = fillets

fillets_SOURCES = Application.cpp Application.h GameAgent.cpp GameAgent.h main.cpp KeyConsole.cpp KeyConsole.h

fillets_LDADD = ../menu/libmenu.a ../level/liblevel.a ../plan/libplan.a ../effect/libeffect.a ../gengine/libgengine.a $(SDL_GFX_LIBS) $(SDL_LIBS) $(LUA_LIBS) $(BOOST_LIBS)


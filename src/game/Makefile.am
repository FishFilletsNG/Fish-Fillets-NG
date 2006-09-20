SDL_GFX_CFLAGS = -I@top_srcdir@/src/SDL_gfx
SDL_GFX_LIBS = ../SDL_gfx/libSDL_gfx.a

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect -I@top_srcdir@/src/widget -I@top_srcdir@/src/plan -I@top_srcdir@/src/option -I@top_srcdir@/src/state -I@top_srcdir@/src/level -I@top_srcdir@/src/menu $(SDL_GFX_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS) $(FRIBIDI_CFLAGS)

bin_PROGRAMS = fillets

fillets_SOURCES = Application.cpp Application.h GameAgent.cpp GameAgent.h main.cpp

EXTRA_DIST = fillets.rc fillets.ico

if HAVE_WINDRES
ICON_LIBS = fillets_ico.o
fillets_ico.o: fillets.rc fillets.ico
	@WINDRES@ fillets.rc -O coff -o $@
else
ICON_LIBS =
endif


fillets_LDADD = $(ICON_LIBS) ../menu/libmenu.a ../level/liblevel.a ../state/libstate.a ../option/liboption.a ../plan/libplan.a ../widget/libwidget.a ../effect/libeffect.a ../gengine/libgengine.a $(SDL_GFX_LIBS) $(SDL_LIBS) $(LUA_LIBS) $(BOOST_LIBS) $(FRIBIDI_LIBS) $(X_LIBS)

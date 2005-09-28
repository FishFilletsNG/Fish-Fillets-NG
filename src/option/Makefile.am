SDL_GFX_CFLAGS = -I@top_srcdir@/src/SDL_gfx

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect -I@top_srcdir@/src/widget -I@top_srcdir@/src/plan $(SDL_GFX_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS) $(FRIBIDI_CFLAGS)

noinst_LIBRARIES = liboption.a

liboption_a_SOURCES = MenuOptions.cpp MenuOptions.h OptionsInput.cpp OptionsInput.h SelectLang.cpp SelectLang.h RadioBox.cpp RadioBox.h MenuHelp.cpp MenuHelp.h HelpInput.cpp HelpInput.h


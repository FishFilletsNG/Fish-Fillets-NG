SFONT_CFLAGS = -I@top_srcdir@/src/SFont
SFONT_LIBS = ../SFont/libSFont.a

INCLUDES = -I@top_srcdir@/src/gengine $(SFONT_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS)

bin_PROGRAMS = game

game_SOURCES = Application.cpp Application.h Cube.cpp Cube.h Driver.h Field.cpp Field.h GameAgent.cpp GameAgent.h KeyControl.cpp KeyControl.h KeyDriver.cpp KeyDriver.h LayoutException.h MarkMask.cpp MarkMask.h Picture.cpp Picture.h Shape.cpp Shape.h View.cpp View.h game-script.cpp game-script.h main.cpp Room.cpp Room.h Anim.cpp Anim.h Rules.cpp Rules.h SubTitleAgent.cpp SubTitleAgent.h Title.cpp Title.h FishDialog.cpp FishDialog.h Goal.cpp Goal.h

game_LDADD = ../gengine/libgengine.a $(SFONT_LIBS) $(SDL_LIBS) $(LUA_LIBS) $(BOOST_LIBS)


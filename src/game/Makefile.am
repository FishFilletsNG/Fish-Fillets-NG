SFONT_CFLAGS = -I@top_srcdir@/src/SFont
SFONT_LIBS = ../SFont/libSFont.a

INCLUDES = -I@top_srcdir@/src/gengine $(SFONT_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS)

bin_PROGRAMS = fillets

fillets_SOURCES = Application.cpp Application.h Cube.cpp Cube.h Field.cpp Field.h GameAgent.cpp GameAgent.h KeyControl.cpp KeyControl.h LayoutException.h MarkMask.cpp MarkMask.h Picture.cpp Picture.h Shape.cpp Shape.h View.cpp View.h game-script.cpp game-script.h main.cpp Room.cpp Room.h Anim.cpp Anim.h Rules.cpp Rules.h SubTitleAgent.cpp SubTitleAgent.h Title.cpp Title.h FishDialog.cpp FishDialog.h Goal.cpp Goal.h Controls.cpp Controls.h ControlSym.h Unit.cpp Unit.h LoadException.h CommandQueue.cpp CommandQueue.h Command.h ScriptCmd.cpp ScriptCmd.h Level.cpp Level.h ModelFactory.cpp ModelFactory.h ViewEffect.cpp ViewEffect.h SurfaceLock.cpp SurfaceLock.h LevelNode.cpp LevelNode.h WorldMap.cpp WorldMap.h NodeDrawer.cpp NodeDrawer.h WorldWay.cpp WorldWay.h LuaTable.cpp LuaTable.h

fillets_LDADD = ../gengine/libgengine.a $(SFONT_LIBS) $(SDL_LIBS) $(LUA_LIBS) $(BOOST_LIBS)


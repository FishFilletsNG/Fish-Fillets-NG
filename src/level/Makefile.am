SFONT_CFLAGS = -I@top_srcdir@/src/SFont

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect -I@top_srcdir@/src/plan $(SFONT_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS)

noinst_LIBRARIES = liblevel.a

liblevel_a_SOURCES = Anim.cpp Anim.h ControlSym.h Controls.cpp Controls.h Cube.cpp Cube.h Field.cpp Field.h Goal.cpp Goal.h KeyControl.cpp KeyControl.h LayoutException.h Level.cpp Level.h LoadException.h MarkMask.cpp MarkMask.h ModelFactory.cpp ModelFactory.h Room.cpp Room.h Rules.cpp Rules.h Shape.cpp Shape.h Unit.cpp Unit.h View.cpp View.h PhaseLocker.cpp PhaseLocker.h LevelStatus.h LevelScript.cpp LevelScript.h ModelList.cpp ModelList.h LevelInput.cpp LevelInput.h game-script.cpp game-script.h level-script.cpp level-script.h


SDL_GFX_CFLAGS = -I@top_srcdir@/src/SDL_gfx

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect -I@top_srcdir@/src/widget -I@top_srcdir@/src/plan -I@top_srcdir@/src/option -I@top_srcdir@/src/state $(SDL_GFX_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS) $(FRIBIDI_CFLAGS)

noinst_LIBRARIES = liblevel.a

liblevel_a_SOURCES = Anim.cpp Anim.h ControlSym.h Controls.cpp Controls.h Cube.cpp Cube.h Field.cpp Field.h Goal.cpp Goal.h KeyControl.cpp KeyControl.h LayoutException.h Level.cpp Level.h LoadException.h MarkMask.cpp MarkMask.h ModelFactory.cpp ModelFactory.h Room.cpp Room.h Rules.cpp Rules.h Shape.cpp Shape.h ShapeBuilder.cpp ShapeBuilder.h Unit.cpp Unit.h View.cpp View.h PhaseLocker.cpp PhaseLocker.h LevelStatus.cpp LevelStatus.h LevelScript.cpp LevelScript.h ModelList.cpp ModelList.h LevelInput.cpp LevelInput.h OnCondition.h OnStack.h OnWall.h OnStrongPad.h Decor.h RopeDecor.cpp RopeDecor.h StepCounter.h StepDecor.cpp StepDecor.h game-script.cpp game-script.h level-script.cpp level-script.h DescFinder.h StatusDisplay.cpp StatusDisplay.h Landslip.cpp Landslip.h LevelLoading.cpp LevelLoading.h LevelCountDown.cpp LevelCountDown.h RoomAccess.cpp RoomAccess.h Dir.cpp Dir.h MouseControl.cpp MouseControl.h FinderAlg.cpp FinderAlg.h FinderPlace.h FinderField.cpp FinderField.h CountAdvisor.h


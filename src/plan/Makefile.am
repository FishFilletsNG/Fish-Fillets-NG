SFONT_CFLAGS = -I@top_srcdir@/src/SFont

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect $(SFONT_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS)

noinst_LIBRARIES = libplan.a

libplan_a_SOURCES = Command.h CommandQueue.cpp CommandQueue.h DemoMode.cpp DemoMode.h FishDialog.cpp FishDialog.h Planner.cpp Planner.h ScriptCmd.cpp ScriptCmd.h SubTitleAgent.cpp SubTitleAgent.h Title.cpp Title.h StateManager.cpp StateManager.h GameState.cpp GameState.h DemoInput.cpp DemoInput.h KeyDesc.cpp KeyDesc.h Keymap.cpp Keymap.h demo-script.cpp demo-script.h dialog-script.cpp dialog-script.h


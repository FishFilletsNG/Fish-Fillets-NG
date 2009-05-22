
INCLUDES = $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS)
AM_CPPFLAGS = -DSYSTEM_DATA_DIR=\""@datadir@"\"

if WITH_BOOST
FSPATH_IMPL = FsPath_boost.cpp
else
FSPATH_IMPL = FsPath.cpp
endif

EXTRA_DIST = FsPath.cpp FsPath_boost.cpp

noinst_LIBRARIES = libgengine.a

libgengine_a_SOURCES = AgentPack.cpp AgentPack.h BaseAgent.cpp BaseAgent.h BaseException.cpp BaseException.h BaseListener.cpp BaseListener.h BaseMsg.cpp BaseMsg.h Dialog.cpp Dialog.h DialogStack.cpp DialogStack.h DummySoundAgent.h ExInfo.cpp ExInfo.h INamed.h ImgException.cpp ImgException.h InputAgent.cpp InputAgent.h IntMsg.cpp IntMsg.h KeyBinder.cpp KeyBinder.h KeyStroke.cpp KeyStroke.h Log.cpp Log.h HelpException.h LogicException.h MessagerAgent.cpp MessagerAgent.h MixException.cpp MixException.h Name.cpp Name.h NameException.h NoCopy.h OptionAgent.cpp OptionAgent.h OptionParams.cpp OptionParams.h Path.cpp Path.h Random.cpp Random.h ResDialogPack.cpp ResDialogPack.h ResImagePack.cpp ResImagePack.h ResourceException.h ResourcePack.h ResCache.h SDLException.cpp SDLException.h SDLSoundAgent.cpp SDLSoundAgent.h ScriptAgent.cpp ScriptAgent.h ScriptException.h ScriptState.cpp ScriptState.h SimpleMsg.h SoundAgent.cpp SoundAgent.h StringMsg.cpp StringMsg.h StringTool.cpp StringTool.h TimerAgent.cpp TimerAgent.h UnknownMsgException.h V2.h VideoAgent.cpp VideoAgent.h PlannedDialog.cpp PlannedDialog.h minmax.h ResSoundPack.cpp ResSoundPack.h Environ.cpp Environ.h InputHandler.cpp InputHandler.h InputProvider.h MouseStroke.cpp MouseStroke.h def-script.cpp def-script.h options-script.cpp options-script.h SysVideo.cpp SysVideo.h Drawable.h MultiDrawer.cpp MultiDrawer.h PathException.h Scripter.cpp Scripter.h FsPath.h $(FSPATH_IMPL)

#NOTE: OptionAgent depends on SYSTEM_DATA_DIR
OptionAgent.o: Makefile

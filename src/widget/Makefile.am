SDL_GFX_CFLAGS = -I@top_srcdir@/src/SDL_gfx

INCLUDES = -I@top_srcdir@/src/gengine -I@top_srcdir@/src/effect $(SDL_GFX_CFLAGS) $(SDL_CFLAGS) $(LUA_CFLAGS) $(BOOST_CFLAGS) $(FRIBIDI_CFLAGS)

noinst_LIBRARIES = libwidget.a

libwidget_a_SOURCES = HBox.cpp HBox.h IWidget.cpp IWidget.h Slider.cpp Slider.h VBox.cpp VBox.h WiBox.cpp WiBox.h WiPicture.cpp WiPicture.h WiSpace.h WiContainer.cpp WiContainer.h WiButton.cpp WiButton.h WiLabel.cpp WiLabel.h Labels.cpp Labels.h WiStatusBar.cpp WiStatusBar.h WiPara.cpp WiPara.h


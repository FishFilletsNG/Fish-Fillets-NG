# Process this file with autoconf to produce a configure script.
AC_INIT(Makefile.cvs)
AC_CANONICAL_TARGET

# The AM_INIT_AUTOMAKE macro tells automake the name and version number
# of the software package so it can generate rules for building a source
# archive.
AM_INIT_AUTOMAKE(fillets-ng, 0.1.0)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

###################################################
# Check for lua50, and define LUA_CFLAGS and LUA_LIBS
PKG_CHECK_MODULES(LUA, lua50)
AC_SUBST(LUA_CFLAGS)
AC_SUBST(LUA_LIBS)

# Check for lualib50, and define LUALIB_CFLAGS and LUALIB_LIBS
PKG_CHECK_MODULES(LUALIB, lualib50)
AC_SUBST(LUALIB_CFLAGS)
AC_SUBST(LUALIB_LIBS)

###################################################
# Check for XML, and define SDL_CFLAGS and SDL_LIBS
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
        :,
        AC_MSG_ERROR([*** library SDL-devel version $SDL_VERSION not found!])
)

###################################################
# Checks for C libraries.
# SDL_mixer
AC_CHECK_LIB([SDL_mixer], [main],
             LIBS="-lSDL_mixer $LIBS",
             AC_MSG_ERROR([*** SDL_mixer-devel library not found!])
)
# SDL_image
AC_CHECK_LIB([SDL_image], [main],
             LIBS="-lSDL_image $LIBS",
             AC_MSG_ERROR([*** SDL_image-devel library not found!])
)
# m = math
AC_CHECK_LIB([m], [main],
             LIBS="-lm $LIBS",
             AC_MSG_ERROR([*** math libm library not found!])
)

###################################################
# enable C++ language
AC_LANG(C++)

###################################################
# Checks for C++ libraries.
# boost_filesystem
AC_CHECK_LIB([boost_filesystem], [main],
             LIBS="-lboost_filesystem $LIBS",
             AC_MSG_ERROR([*** boost_filesystem-devel library not found!])
 )

###################################################
# Debug on/off
AC_ARG_ENABLE(debug,
              [  --enable-debug          Print debug messages],
              [case "${enableval}" in
                 yes) debug=true ;;
                 no)  debug=false ;;
                 *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
               esac],[debug=false]
               )
CXXFLAGS="$CXXFLAGS -Wall -W -Wpointer-arith -Wcast-qual \
-Wcast-align -Wmissing-prototypes -Woverloaded-virtual \
-Wunused-parameter -Wconversion -Wshadow \
-Wstrict-prototypes"
if test "x$debug" = "xtrue"; then
    CXXFLAGS="$CXXFLAGS -pipe -g3"
else
    CXXFLAGS="$CXXFLAGS -pipe -O2"
fi

###################################################
AC_OUTPUT([
           Makefile
           src/Makefile
           src/gengine/Makefile
           src/SFont/Makefile
           src/game/Makefile
           ])


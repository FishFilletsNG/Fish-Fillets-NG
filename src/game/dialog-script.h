#ifndef HEADER_DIALOG_SCRIPT_H
#define HEADER_DIALOG_SCRIPT_H

extern "C" {
#include "lua.h"
}

extern int script_file_include(lua_State *L) throw();
extern int script_file_exists(lua_State *L) throw();

extern int script_game_planAction(lua_State *L) throw();
extern int script_game_isPlanning(lua_State *L) throw();

extern int script_dialog_empty(lua_State *L) throw();
extern int script_dialog_addFont(lua_State *L) throw();
extern int script_dialog_addDialog(lua_State *L) throw();
extern int script_model_isTalking(lua_State *L) throw();
extern int script_model_planDialog(lua_State *L) throw();

extern int script_sound_playMusic(lua_State *L) throw();
extern int script_sound_stopMusic(lua_State *L) throw();

#endif
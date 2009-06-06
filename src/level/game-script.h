#ifndef HEADER_GAME_SCRIPT_H
#define HEADER_GAME_SCRIPT_H

extern "C" {
#include "lua.h"
}

extern int script_game_setRoomWaves(lua_State *L) throw();
extern int script_game_addModel(lua_State *L) throw();
extern int script_game_getCycles(lua_State *L) throw();
extern int script_game_addDecor(lua_State *L) throw();
extern int script_game_setScreenShift(lua_State *L) throw();
extern int script_game_changeBg(lua_State *L) throw();
extern int script_game_getBg(lua_State *L) throw();
extern int script_game_checkActive(lua_State *L) throw();
extern int script_game_setFastFalling(lua_State *L) throw();

extern int script_model_addAnim(lua_State *L) throw();
extern int script_model_runAnim(lua_State *L) throw();
extern int script_model_setAnim(lua_State *L) throw();
extern int script_model_useSpecialAnim(lua_State *L) throw();
extern int script_model_countAnims(lua_State *L) throw();
extern int script_model_setEffect(lua_State *L) throw();
extern int script_model_getLoc(lua_State *L) throw();
extern int script_model_getAction(lua_State *L) throw();
extern int script_model_getState(lua_State *L) throw();
extern int script_model_getDir(lua_State *L) throw();
extern int script_model_getTouchDir(lua_State *L) throw();
extern int script_model_isAlive(lua_State *L) throw();
extern int script_model_isOut(lua_State *L) throw();
extern int script_model_isLeft(lua_State *L) throw();
extern int script_model_isAtBorder(lua_State *L) throw();
extern int script_model_getW(lua_State *L) throw();
extern int script_model_getH(lua_State *L) throw();
extern int script_model_setGoal(lua_State *L) throw();
extern int script_model_change_turnSide(lua_State *L) throw();
extern int script_model_change_setLocation(lua_State *L) throw();
extern int script_model_setViewShift(lua_State *L) throw();
extern int script_model_getViewShift(lua_State *L) throw();
extern int script_model_setBusy(lua_State *L) throw();
extern int script_model_getExtraParams(lua_State *L) throw();
extern int script_model_change_setExtraParams(lua_State *L) throw();
extern int script_model_equals(lua_State *L) throw();

extern int script_sound_addSound(lua_State *L) throw();
extern int script_sound_playSound(lua_State *L) throw();

#endif

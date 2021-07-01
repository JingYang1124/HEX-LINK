#include "Game_Controller.h"


#if (GAME == Sekiro)
    KeyInfoType KeyInfo[KEY_NUM] = {
        {KEY_GAME_UP,           KEY_PRESS_MAX_TIME},
        {KEY_GAME_DOWN,         KEY_PRESS_MAX_TIME},
        {KEY_GAME_LEFT,         KEY_PRESS_MAX_TIME},
        {KEY_GAME_RIGHT,        KEY_PRESS_MAX_TIME},
        {KEY_GAME_USE_TOOL,     KEY_PRESS_MAX_TIME},
        {KEY_GAME_HOOK,         KEY_PRESS_MAX_TIME},
        {KEY_GAME_HAND,         KEY_PRESS_MAX_TIME},
        {KEY_GAME_SHIFT,        KEY_PRESS_MAX_TIME},  
        {KEY_GAME_SWITCH,       KEY_PRESS_MAX_TIME},  
        {KEY_GAME_ATTACK,       KEY_PRESS_MAX_TIME},
        {KEY_GAME_DEFEND,       KEY_PRESS_MAX_TIME},  
        {KEY_GAME_TARGET,       KEY_PRESS_MAX_TIME},  
        {KEY_GAME_VISION_LEFT,  KEY_PRESS_MAX_TIME},
        {KEY_GAME_VISION_RIGHT, KEY_PRESS_MAX_TIME},  
        {KEY_GAME_JUMP,         KEY_PRESS_MAX_TIME},               
    };
#endif

#if (GAME == NFS)

#endif




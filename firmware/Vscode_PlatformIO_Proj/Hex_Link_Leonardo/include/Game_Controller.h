#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Universal_Headers.h"
#include "Keyboard.h"

#define KEY_PRESS_MAX_TIME  0xFFFF

#define Sekiro 1
#define NFS    2

#define GAME Sekiro

typedef struct{
    uint8_t Key;
    uint16_t Release_Time;
}KeyInfoType;


#if (GAME == Sekiro)
    #define KEY_NUM               15

    extern KeyInfoType KeyInfo[KEY_NUM];

    #define KEY_GAME_UP           'w'
    #define KEY_GAME_DOWN         's'
    #define KEY_GAME_LEFT         'a'
    #define KEY_GAME_RIGHT        'd'

    #define KEY_GAME_USE_TOOL     'r'
    #define KEY_GAME_HOOK         'f'
    #define KEY_GAME_HAND         KEY_LEFT_CTRL
    #define KEY_GAME_SHIFT        KEY_LEFT_SHIFT
    #define KEY_GAME_SWITCH       'z'

    #define KEY_GAME_ATTACK       'v'
    #define KEY_GAME_DEFEND       'n'
    #define KEY_GAME_TARGET       'b'

    #define KEY_GAME_VISION_LEFT  'j'
    #define KEY_GAME_VISION_RIGHT 'l'
    #define KEY_GAME_JUMP         ' '

    typedef enum{
        KEY_GAME_UP_ID = 0,      
        KEY_GAME_DOWN_ID,         
        KEY_GAME_LEFT_ID,         
        KEY_GAME_RIGHT_ID,       
        KEY_GAME_USE_TOOL_ID,    
        KEY_GAME_HOOK_ID,         
        KEY_GAME_HAND_ID,        
        KEY_GAME_SHIFT_ID,       
        KEY_GAME_SWITCH_ID,      
        KEY_GAME_ATTACK_ID,      
        KEY_GAME_DEFEND_ID,      
        KEY_GAME_TARGET_ID,     
        KEY_GAME_VISION_LEFT_ID,  
        KEY_GAME_VISION_RIGHT_ID, 
        KEY_GAME_JUMP_ID       
    }KeyIdType;

#endif

#if (GAME == NFS)
    #define KEY_GAME_UP           'w'
    #define KEY_GAME_DOWN         's'
    #define KEY_GAME_LEFT         'a'
    #define KEY_GAME_RIGHT        'd'
#endif


#endif

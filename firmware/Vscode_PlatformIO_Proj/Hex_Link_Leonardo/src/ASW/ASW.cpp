#include "ASW.h"
#include "Game_Controller.h"

byte Device_Command[DeviceTypeNum];


bool Devices_Get_New_Cmd[DeviceTypeNum] = {0,0,0,0,0,0,0};

void ASW_STARTUP(void){
  for(byte index = 0; index < DeviceTypeNum; index ++){
    Device_Command[index] = DEFAULT_COMMAND;
  }
}

void ASW_NRF_RECEIVE(void){
  NRF_Read();
  if(R_Cmd.Command > DEFAULT_COMMAND){
    Serial.print("DeviceID is:");
    Serial.print(R_Cmd.ThisDeviceId);
    Serial.print("  Command is:");
    Serial.print(R_Cmd.Command);    
    Serial.print("  Index is:");
    Serial.println(R_Cmd.Operation_Index);

    if(R_Cmd.Operation_Index != Devices_Last_Index[R_Cmd.ThisDeviceId]){
      Devices_Last_Index[R_Cmd.ThisDeviceId] = R_Cmd.Operation_Index;
      Devices_Get_New_Cmd[R_Cmd.ThisDeviceId] = true;
    }
    Device_Command[R_Cmd.ThisDeviceId] = R_Cmd.Command;
  }
  R_Cmd.Command = 0;
}


void ASW_KEYBOARD_SEND(void){
  if(Device_Command[DEVICE_CHEST] == CHEST_LEFT && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.press(KEY_GAME_VISION_LEFT);  
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_CHEST] == CHEST_RIGHT && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.press(KEY_GAME_VISION_RIGHT); 
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_CHEST] == CHEST_MID && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.release(KEY_GAME_VISION_LEFT); 
      Keyboard.release(KEY_GAME_VISION_RIGHT); 
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_CHEST] == CHEST_JUMP && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.press(KEY_GAME_JUMP); 
      delay(100);
      Keyboard.release(KEY_GAME_JUMP); 
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_LEFT_LEG] == LEFT_LEG_RUN && Devices_Get_New_Cmd[DEVICE_LEFT_LEG] == true){
      Keyboard.press(KEY_GAME_UP); 
      Keyboard.press(KEY_GAME_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_LEG] = false;
  }
  if(Device_Command[DEVICE_LEFT_LEG] == LEFT_LEG_WALK && Devices_Get_New_Cmd[DEVICE_LEFT_LEG] == true){
      Keyboard.press(KEY_GAME_UP); 
      Keyboard.release(KEY_GAME_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_LEG] = false;
  }
  if(Device_Command[DEVICE_LEFT_LEG] == LEFT_LEG_STOP && Devices_Get_New_Cmd[DEVICE_LEFT_LEG] == true){
      Keyboard.release(KEY_GAME_UP); 
      Keyboard.release(KEY_GAME_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_LEG] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_HOOK && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press(KEY_GAME_HOOK); 
      delay(200);
      Keyboard.release(KEY_GAME_HOOK); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_CTRL && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press(KEY_GAME_HAND); 
      delay(200);
      Keyboard.release(KEY_GAME_HAND); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_SWITCH && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press(KEY_GAME_SWITCH); 
      delay(200);
      Keyboard.release(KEY_GAME_SWITCH); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_SEE_THROUGH && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press(KEY_GAME_SHIFT); 
      delay(100);
      Keyboard.release(KEY_GAME_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_RIGHT_ARM] == RIGHT_ARM_ATTACK && Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] == true){
      Keyboard.press(KEY_GAME_ATTACK); 
      delay(100);
      Keyboard.release(KEY_GAME_ATTACK); 
      Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] = false;
  }
  if(Device_Command[DEVICE_RIGHT_ARM] == RIGHT_ARM_DEFEND && Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] == true){
      Keyboard.press(KEY_GAME_DEFEND); 
      delay(300);
      Keyboard.release(KEY_GAME_DEFEND); 
      Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] = false;
  }
  if(Device_Command[DEVICE_RIGHT_ARM] == RIGHT_ARM_TARGET && Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] == true){
      Keyboard.press(KEY_GAME_TARGET); 
      delay(100);
      Keyboard.release(KEY_GAME_TARGET); 
      Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] = false;
  }
  if(Device_Command[DEVICE_TOOL] == TOOL_USE && Devices_Get_New_Cmd[DEVICE_TOOL] == true){
      Keyboard.press(KEY_GAME_USE_TOOL); 
      delay(100);
      Keyboard.release(KEY_GAME_USE_TOOL); 
      Devices_Get_New_Cmd[DEVICE_TOOL] = false;
  }
}

void ASW_KEYBOARD_RELEASE_CHECK(void){
  for(uint8_t index = 0; index < KEY_NUM;index ++){
    if(KeyInfo[index].Release_Time != KEY_PRESS_MAX_TIME){
      KeyInfo[index].Release_Time -= 1;
      if(KeyInfo[index].Release_Time == 0){
        Keyboard.release(KeyInfo[index].Key);
        KeyInfo[index].Release_Time = KEY_PRESS_MAX_TIME;
      }
    }
  }

}


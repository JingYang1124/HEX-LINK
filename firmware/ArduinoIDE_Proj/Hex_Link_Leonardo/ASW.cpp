#include "ASW.h"

byte Device_Command[DeviceTypeNum];
byte Device_MissCount[DeviceTypeNum];
bool Device_ConnectFlag[DeviceTypeNum];

#define MissCountMax 20

bool Devices_Get_New_Cmd[DeviceTypeNum] = {0,0,0,0,0,0,0};

void ASW_STARTUP(void){
  for(byte index = 0; index < DeviceTypeNum; index ++){
    Device_Command[index] = DEFAULT_COMMAND;
  }
}

void ASW_NRF_RECEIVE(void){
  //Serial.println("ASW_NRF_RECEIVE");
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
    Device_MissCount[R_Cmd.ThisDeviceId] = 0;
    Device_ConnectFlag[R_Cmd.ThisDeviceId] = true;
  }
  /*
  for(byte index = 0; index < DeviceTypeNum; index ++){
    if(Device_MissCount[index]<MissCountMax)
      Device_MissCount[index] += 1;
    if(Device_MissCount[index]>MissCountMax)
      Device_ConnectFlag[index] = false;
  }
  */
  R_Cmd.Command = 0;
}

byte count_w = 0;

void ASW_KEYBOARD_SEND(void){
  if(Device_Command[DEVICE_CHEST] == CHEST_LEFT && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.press('j');  
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_CHEST] == CHEST_RIGHT && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.press('l'); 
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_CHEST] == CHEST_MID && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.release('j'); 
      Keyboard.release('l'); 
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_CHEST] == CHEST_JUMP && Devices_Get_New_Cmd[DEVICE_CHEST] == true){
      Keyboard.press(' '); 
      delay(100);
      Keyboard.release(' '); 
      Devices_Get_New_Cmd[DEVICE_CHEST] = false;
  }
  if(Device_Command[DEVICE_LEFT_LEG] == LEFT_LEG_RUN && Devices_Get_New_Cmd[DEVICE_LEFT_LEG] == true){
      Keyboard.press('w'); 
      Keyboard.press(KEY_LEFT_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_LEG] = false;
  }
  if(Device_Command[DEVICE_LEFT_LEG] == LEFT_LEG_WALK && Devices_Get_New_Cmd[DEVICE_LEFT_LEG] == true){
      Keyboard.press('w'); 
      Keyboard.release(KEY_LEFT_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_LEG] = false;
  }
  if(Device_Command[DEVICE_LEFT_LEG] == LEFT_LEG_STOP && Devices_Get_New_Cmd[DEVICE_LEFT_LEG] == true){
      Keyboard.release('w'); 
      Keyboard.release(KEY_LEFT_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_LEG] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_HOOK && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press('f'); 
      delay(200);
      Keyboard.release('f'); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_CTRL && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press(KEY_LEFT_CTRL); 
      delay(200);
      Keyboard.release(KEY_LEFT_CTRL); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_SWITCH && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press('z'); 
      delay(200);
      Keyboard.release('z'); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_LEFT_ARM] == LEFT_ARM_SEE_THROUGH && Devices_Get_New_Cmd[DEVICE_LEFT_ARM] == true){
      Keyboard.press(KEY_LEFT_SHIFT); 
      delay(100);
      Keyboard.release(KEY_LEFT_SHIFT); 
      Devices_Get_New_Cmd[DEVICE_LEFT_ARM] = false;
  }
  if(Device_Command[DEVICE_RIGHT_ARM] == RIGHT_ARM_ATTACK && Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] == true){
      Keyboard.press('v'); 
      delay(100);
      Keyboard.release('v'); 
      Serial.println("Attack");
      Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] = false;
  }
  if(Device_Command[DEVICE_RIGHT_ARM] == RIGHT_ARM_DEFEND && Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] == true){
      Keyboard.press('n'); 
      delay(300);
      Keyboard.release('n'); 
      Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] = false;
  }
  if(Device_Command[DEVICE_RIGHT_ARM] == RIGHT_ARM_TARGET && Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] == true){
      Keyboard.press('b'); 
      delay(300);
      Keyboard.release('b'); 
      Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] = false;
  }
  if(Device_Command[DEVICE_RIGHT_ARM] == RIGHT_ARM_ATTACK_SP && Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] == true){
      Keyboard.press('n'); 
      delay(50);
      Keyboard.press('v'); 
      delay(250);
      Keyboard.release('n'); 
      Keyboard.release('v');
      Devices_Get_New_Cmd[DEVICE_RIGHT_ARM] = false;
  }
  if(Device_Command[DEVICE_TOOL] == TOOL_USE && Devices_Get_New_Cmd[DEVICE_TOOL] == true){
      Keyboard.press('r'); 
      delay(100);
      Keyboard.release('r'); 
      Devices_Get_New_Cmd[DEVICE_TOOL] = false;
  }
}

void ASW_DEVICE_CONFIRM(void){
  
}

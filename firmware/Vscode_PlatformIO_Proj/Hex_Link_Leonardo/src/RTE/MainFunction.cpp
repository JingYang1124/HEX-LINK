#include "MainFunction.h"
#include "ISR.h"

void Init_Setup(void){
	LED_Init();
  NRF_Init();
  OLED_Init();
  Serial.begin(19200);
  Mouse.begin();
  Keyboard.begin();
  ISR_Init();
  //Task_Init();
}

void Loop_Function(void){
    ASW_NRF_RECEIVE(); 
    ASW_KEYBOARD_SEND();
    delay(10);
}

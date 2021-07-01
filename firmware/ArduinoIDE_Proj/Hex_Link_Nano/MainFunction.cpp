#include "MainFunction.h"


void Init_Setup(void){
	LED_Init();
  IMU_Init();
  NRF_Init();
  Serial.begin(230400);
  LED_Light();
  delay(1000);
  LED_Light_Off();
  ISR_Init();

  /*Filter Init*/
  IIR_Filter_state_Init(&IIR_Filter_State_AcX);
  IIR_Filter_state_Init(&IIR_Filter_State_AcY);
  IIR_Filter_state_Init(&IIR_Filter_State_AcZ);
  IIR_Filter_state_Init(&IIR_Filter_State_GyX);
  IIR_Filter_state_Init(&IIR_Filter_State_GyY);
  IIR_Filter_state_Init(&IIR_Filter_State_GyZ);
}

void Loop_Function(void){
  ASW_Loop();
}

#include "Universal_Headers.h"
#include <MsTimer2.h>
#include "ASW.h"
#include "ISR.h"

uint16_t ISR_Count = 0;


void Key_Release_Check_Hook(){  
  sei();//Re-open the interrupts
  ASW_KEYBOARD_RELEASE_CHECK();
}

void ISR_Init(void){
	MsTimer2::set(Key_Release_Check_Period, Key_Release_Check_Hook); 
	MsTimer2::start();
}

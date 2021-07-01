#include "LED.h"
#include "Device_Cfg.h"
void LED_Init(void){
	pinMode(LED_PIN,OUTPUT);
}

void LED_Light(void){
  digitalWrite(LED_PIN, HIGH); 
}

void LED_Light_Off(void){
	digitalWrite(LED_PIN, LOW); 
}

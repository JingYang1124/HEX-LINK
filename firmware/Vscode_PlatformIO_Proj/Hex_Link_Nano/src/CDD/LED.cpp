#include "Universal_Headers.h"
#include "LED.h"
#include "Device_Cfg.h"
void LED_Init(void){
	pinMode(LED_RED_PIN,OUTPUT);
	pinMode(LED_GREEN_PIN,OUTPUT);
	pinMode(LED_BLUE_PIN,OUTPUT);
}

void LED_Light(void){
	switch(DeviceId){
		case DEVICE_CHEST:
			/*RED LIGHT*/
			digitalWrite(LED_RED_PIN, HIGH); 
			break;
		case DEVICE_LEFT_ARM:
			/*WHITE LIGHT*/
      		digitalWrite(LED_RED_PIN, HIGH); 
      		digitalWrite(LED_GREEN_PIN, HIGH); 
     		digitalWrite(LED_BLUE_PIN, HIGH); 
			break;
		case DEVICE_RIGHT_ARM:
			/*YELLOW LIGHT*/
			digitalWrite(LED_RED_PIN, HIGH); 
			digitalWrite(LED_GREEN_PIN, HIGH); 
			break;
		case DEVICE_LEFT_LEG:
			/*GREEN LIGHT*/
			digitalWrite(LED_GREEN_PIN, HIGH); 
			break;
		case DEVICE_RIGHT_LEG:
			/*GREEN LIGHT*/
			digitalWrite(LED_GREEN_PIN, HIGH); 
			break;
		case DEVICE_WEAPON:
			/*BLUE LIGHT*/
			digitalWrite(LED_BLUE_PIN, HIGH); 
			break;
		case DEVICE_TOOL:
			/*WHITE LIGHT*/
			digitalWrite(LED_RED_PIN, HIGH); 
      digitalWrite(LED_GREEN_PIN, HIGH); 
			digitalWrite(LED_BLUE_PIN, HIGH); 
			break;
    case DEVICE_TEST:
      		/*WHITE LIGHT*/
            digitalWrite(LED_RED_PIN, HIGH); 
      		digitalWrite(LED_GREEN_PIN, HIGH); 
      		digitalWrite(LED_BLUE_PIN, HIGH); 
	}
}

void LED_Light_Off(void){
	digitalWrite(LED_RED_PIN, LOW); 
	digitalWrite(LED_GREEN_PIN, LOW); 
	digitalWrite(LED_BLUE_PIN, LOW); 
}

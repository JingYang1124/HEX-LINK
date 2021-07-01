#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H



/*Debug Definitions*/
//#define ASW_API_SERIAL_PRINT   
//#define IMU_READ_SERIAL_PRINT  
//#define PRINT_ISR_COUNT

/*NRF PIN Define*/
#define NRF_CE_PIN  8
#define NRF_CSN_PIN 10

/*LED PIN Define*/
#define LED_RED_PIN   3
#define LED_GREEN_PIN 4
#define LED_BLUE_PIN  5

#define DeviceTypeNum  8

extern DeviceIdType DeviceId;


#endif

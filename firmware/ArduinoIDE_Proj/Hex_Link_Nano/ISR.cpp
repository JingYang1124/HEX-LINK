#include "Universal_Headers.h"
#include <MsTimer2.h>
#include "IMU.h"
#include "LED.h"
#include "ISR.h"
#include "Filter.h"
#include "Motion_Detect.h"

uint16_t ISR_Count = 0;
uint16_t ISR_Count_Send = 0;


void flash() {
  static unsigned char current_time = 0;
  static unsigned char current_rountine = 0;
  static unsigned char target_High_Light = 2;
  static char dir = 1;
  
  if(current_time == Period)
  {
    current_time = 0;
    current_rountine += 1;
    if(current_rountine == Duration_Period_Num)
    {
      current_rountine = 0;
      target_High_Light += dir;
      if(target_High_Light == 1 || target_High_Light == Period - 2)
        dir = dir*(-1);
    }
  }
  if(current_time < target_High_Light)
    LED_Light();
  else
    LED_Light_Off();
  current_time += 1;
  ISR_Count += 1;
}

void ISR_IMU_Measure(){  
  sei();//Re-open the interrupts
  {
    /* Call the IMU functions to obtain the AcData and GyData*/
    IMU_Read_Average_Ac(IMU_AVERAGE_READ_NUM, &Current_AcData);
    IMU_Read_Average_Gy(IMU_AVERAGE_READ_NUM, &Current_GyData);
    /* Data pre-process with Butterworth filter */      
    Current_AcData.AcX = uint8_t(IIR_Filter(&IIR_Filter_State_AcX,float(Current_AcData.AcX)));
    Current_AcData.AcY = uint8_t(IIR_Filter(&IIR_Filter_State_AcY,float(Current_AcData.AcY)));
    Current_AcData.AcZ = uint8_t(IIR_Filter(&IIR_Filter_State_AcZ,float(Current_AcData.AcZ)));
    Current_GyData.GyX = uint8_t(IIR_Filter(&IIR_Filter_State_GyX,float(Current_GyData.GyX)));
    Current_GyData.GyY = uint8_t(IIR_Filter(&IIR_Filter_State_GyY,float(Current_GyData.GyY)));
    Current_GyData.GyZ = uint8_t(IIR_Filter(&IIR_Filter_State_GyZ,float(Current_GyData.GyZ)));
    /* Update the Data batch */
    Update_Prv_AcData();
    Update_Prv_GyData();
    if(Silent_Count > 0)
      Silent_Count -= 1;
    if(Silent_Count == 0)
      ISR_Count_Send = 1;
  }
  ISR_Count += 1;
}

void ISR_Init(void){
	/* 5ms period */
  /*
  if(DeviceId == DEVICE_CHEST)
    MsTimer2::set(1, flash); 
  else
  */
	MsTimer2::set(ISR_IMU_Measure_Period, ISR_IMU_Measure); 
	MsTimer2::start();
}

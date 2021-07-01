#include "Universal_Headers.h"
#include "IMU.h"
#include "COM.h"
#include "ISR.h"
#include "Filter.h"
#include "Motion_Detect.h"
#include "ASW.h"



ApiType ASW_ApiTable[DeviceTypeNum] =
{
  ASW_CHEST,
  ASW_LEFT_ARM,
  ASW_RIGHT_ARM,
  ASW_LEFT_LEG,
  ASW_TOOL,
  ASW_RIGHT_LEG,
  ASW_WEAPON,
  ASW_TEST
};


void ASW_Loop(void){
  ASW_ApiTable[DeviceId]();
}

void ASW_Send_Command(void){
  if(ISR_Count % 20 >= 0)
  {
      Serial.println("********************************");
      NRF_Send(&T_Cmd);
      ISR_Count = 0;
  }
}

void ASW_CHEST(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is CHEST ASW");
  #endif
  if(ISR_Count_Send == 1){
    Chest_Motion_Detect();
    if(Action_Found == true){
      T_Cmd.Operation_Index += 1;
      for(uint8_t send_count = 0;send_count < RE_SEND_COUNT;send_count++)
        NRF_Send(&T_Cmd);
      Action_Found = false;
      Silent_Count = 2 * SILENT_VALVE;
    }
    ISR_Count_Send = 0;
  }
}

void ASW_LEFT_ARM(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is LEFT ARM ASW");
  #endif
  if(ISR_Count_Send == 1){
    Left_Arm_Motion_Detect();
    if(Action_Found == true){
      T_Cmd.Operation_Index += 1;
      for(uint8_t send_count = 0;send_count < RE_SEND_COUNT;send_count++)
        NRF_Send(&T_Cmd);
      Action_Found = false;
      Silent_Count = 5*SILENT_VALVE;
    }
    ISR_Count_Send = 0;
  }
}

void ASW_RIGHT_ARM(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is RIGHT ARM ASW");
  #endif
  if(ISR_Count_Send == 1){
    Right_Arm_Motion_Detect();
    if(Action_Found == true){
      T_Cmd.Operation_Index += 1;
      for(uint8_t send_count = 0;send_count < RE_SEND_COUNT;send_count++)
        NRF_Send(&T_Cmd);
      Action_Found = false;
      Silent_Count = SILENT_VALVE;
    }
    ISR_Count_Send = 0;
  }
}

void ASW_LEFT_LEG(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is LEFT LEG ASW");
  #endif
  if(ISR_Count_Send == 1){
    Motion_Check_Left_Leg();
     if(Action_Found == true){
      T_Cmd.Operation_Index += 1;
      for(uint8_t send_count = 0;send_count < 2*RE_SEND_COUNT;send_count++)
        NRF_Send(&T_Cmd);
      Action_Found = false;
    }
  ISR_Count_Send = 0;
  }
}


void ASW_RIGHT_LEG(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is RIGHT LEG ASW");
  #endif
}

void ASW_WEAPON(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is WEAPON ASW");
  #endif
}

void ASW_TOOL(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is TOOL ASW");
  #endif

  if(ISR_Count_Send == 1){
    Motion_Check_Tool();
     if(Action_Found == true){
      T_Cmd.Operation_Index += 1;
      for(uint8_t send_count = 0;send_count < RE_SEND_COUNT;send_count++)
        NRF_Send(&T_Cmd);
      Action_Found = false;
    }
  ISR_Count_Send = 0;
  }
}

void ASW_TEST(void){
  #if defined(ASW_API_SERIAL_PRINT)
    Serial.println("This is TEST ASW");
  #endif


  
  if(ISR_Count_Send == 1){
    ISR_Count_Send = 0;

    #if defined(PRINT_ISR_COUNT)
    Serial.print("ISR_Count:");
    Serial.print(ISR_Count);
    Serial.print("  ");
    #endif

    Serial.print("AcX:");  
    Serial.print(Current_AcData.AcX);
    Serial.print("  AcY:");  
    Serial.print(Current_AcData.AcY);
    Serial.print("  AcZ:");  
    Serial.print(Current_AcData.AcZ);
    Serial.print("  GyX:");  
    Serial.print(Current_GyData.GyX);
    Serial.print("  GyY:");  
    Serial.print(Current_GyData.GyY);
    Serial.print("  GyZ:");  
    Serial.println(Current_GyData.GyZ);

    
    /*
    Serial.print("AcX:");  
    Serial.print(Current_AcData.AcX);
    Serial.print("  Filter_output_AcX:");  
    Serial.println(IIR_Filter(&IIR_Filter_State_AcX,float(Current_AcData.AcX)));
    */
  }
  
}

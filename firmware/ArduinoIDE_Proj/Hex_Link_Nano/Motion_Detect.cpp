#include "Universal_Headers.h"
#include "IMU.h"
#include "COM.h"
#include "ISR.h"
#include "Motion_Detect.h"

uint8_t AcX_Max_Index_In_Array = 0;
uint8_t AcY_Max_Index_In_Array = 0;
uint8_t AcZ_Max_Index_In_Array = 0;
bool    Action_Found = false;
uint8_t Silent_Count = SILENT_VALVE;

#define LEFT_VALVE_ACX  131
#define RIGHT_VALVE_ACX 124
#define JUMP_VALVE_ACZ  130

const Ellipse_Para_Type Ctrl_Ellipse = {185,130,30,40,150};
const Ellipse_Para_Type Hook_Ellipse = {120,176,25,20,0};
const Ellipse_Para_Type Shift_Ellipse = {90,195,20,25,50};
const Ellipse_Para_Type Switch_Ellipse = {145,140,17,25,60};

const Ellipse_Para_Type Attack_Ellipse = {115,69,16,30,118};//x_0\y_0\a\b\Theta
const Ellipse_Para_Type Defend_Ellipse = {81,141,12,20,109};
const Ellipse_Para_Type Aim_Ellipse = {80,120,15,10,10};

#define WALK_VALVE  25
#define RUN_VALVE   14

#define TOOL_USE_VALVE    132
#define TOOL_UNUSE_VALVE  123
#define TOOL_CONTINUS_VALVE 5

#define ATTACK_SP_VALVE    139
#define ATTACK_SP_DEV_VALVE    2
#define ATTACK_SP_CONTINUS_VALVE 25


Std_ReturnType Fit_Ellipse(float x, float y, Ellipse_Para_Type * Elli_Para){
    Std_ReturnType Ret_Val = false;
    float Fit_Result = 0;
    Fit_Result = (pow(((x-Elli_Para->x_0)*cos(Elli_Para->Theta/180*3.14159) + (y-Elli_Para->y_0)*sin(Elli_Para->Theta/180*3.14159)),2)/pow((Elli_Para->a),2)
      +
      pow((-1*(x-Elli_Para->x_0)*sin(Elli_Para->Theta/180*3.14159) + (y-Elli_Para->y_0)*cos(Elli_Para->Theta/180*3.14159)),2)/pow((Elli_Para->a),2));
    
    if(Fit_Result<=1){
        Ret_Val = true; 
        Serial.println(Fit_Result);
      }
     return Ret_Val;
}

void Prv_Max_Data(IMU_AcData_Type * OutAcData,IMU_GyData_Type * OutGyData){
  uint8_t temVal_x;
  uint8_t temVal_y;
  uint8_t temVal_z;
  temVal_x = 0;temVal_y = 0;temVal_y = 0;
  for(uint8_t index = 0; index < IMU_DATA_BATCH;index ++){
    if(Prv_AcData[index].AcX > temVal_x)temVal_x = Prv_AcData[index].AcX;
    if(Prv_AcData[index].AcY > temVal_y)temVal_y = Prv_AcData[index].AcY;
    if(Prv_AcData[index].AcZ > temVal_z)temVal_z = Prv_AcData[index].AcZ;
  }
  OutAcData->AcX = temVal_x;OutAcData->AcY = temVal_y;OutAcData->AcZ = temVal_z;
  temVal_x = 0;temVal_y = 0;temVal_y = 0;
  for(uint8_t index = 0; index < IMU_DATA_BATCH;index ++){
    if(Prv_GyData[index].GyX > temVal_x)temVal_x = Prv_GyData[index].GyX;
    if(Prv_GyData[index].GyY > temVal_y)temVal_y = Prv_GyData[index].GyY;
    if(Prv_GyData[index].GyZ > temVal_z)temVal_z = Prv_GyData[index].GyZ;
  }
  OutGyData->GyX = temVal_x;OutGyData->GyY = temVal_y;OutGyData->GyZ = temVal_z;
}

void Prv_Min_Data(IMU_AcData_Type * OutAcData,IMU_GyData_Type * OutGyData){
  uint8_t temVal_x;
  uint8_t temVal_y;
  uint8_t temVal_z;
  temVal_x = 255;temVal_y = 255;temVal_y = 255;
  for(uint8_t index = 0; index < IMU_DATA_BATCH;index ++){
    if(Prv_AcData[index].AcX < temVal_x)temVal_x = Prv_AcData[index].AcX;
    if(Prv_AcData[index].AcY < temVal_y)temVal_y = Prv_AcData[index].AcY;
    if(Prv_AcData[index].AcZ < temVal_z)temVal_z = Prv_AcData[index].AcZ;
  }
  OutAcData->AcX = temVal_x;OutAcData->AcY = temVal_y;OutAcData->AcZ = temVal_z;
  temVal_x = 0;temVal_y = 0;temVal_y = 0;
  for(uint8_t index = 0; index < IMU_DATA_BATCH;index ++){
    if(Prv_GyData[index].GyX < temVal_x)temVal_x = Prv_GyData[index].GyX;
    if(Prv_GyData[index].GyY < temVal_y)temVal_y = Prv_GyData[index].GyY;
    if(Prv_GyData[index].GyZ < temVal_z)temVal_z = Prv_GyData[index].GyZ;
  }
  OutGyData->GyX = temVal_x;OutGyData->GyY = temVal_y;OutGyData->GyZ = temVal_z;
}

void Prv_Ave_Data(IMU_AcData_Type * OutAcData,IMU_GyData_Type * OutGyData){
  int16_t AcX_Sum = 0;
  int16_t AcY_Sum = 0;
  int16_t AcZ_Sum = 0;
  int16_t GyX_Sum = 0;
  int16_t GyY_Sum = 0;
  int16_t GyZ_Sum = 0; 
  for(uint8_t index = 0; index < IMU_DATA_BATCH;index ++){
    AcX_Sum += Prv_AcData[index].AcX;
    AcY_Sum += Prv_AcData[index].AcY;
    AcZ_Sum += Prv_AcData[index].AcZ;
    GyX_Sum += Prv_GyData[index].GyX;
    GyY_Sum += Prv_GyData[index].GyY;
    GyZ_Sum += Prv_GyData[index].GyZ;
  }
  OutAcData->AcX = AcX_Sum/IMU_DATA_BATCH;
  OutAcData->AcY = AcY_Sum/IMU_DATA_BATCH;
  OutAcData->AcZ = AcZ_Sum/IMU_DATA_BATCH;
  OutGyData->GyX = GyX_Sum/IMU_DATA_BATCH;
  OutGyData->GyY = GyY_Sum/IMU_DATA_BATCH;
  OutGyData->GyZ = GyZ_Sum/IMU_DATA_BATCH;
}

Std_ReturnType Chest_Motion_Detect(void){
  Std_ReturnType RetVal = CHEST_MID;
  static uint8_t Last_Highest_Point_X = 128;
  static uint8_t Last_Lowest_Point_X = 127;
  static double Last_Mid_Point_X = 128;
  double K_Prv_AcData_X = (Prv_AcData[IMU_DATA_BATCH - 1].AcX - Prv_AcData[0].AcX)/(IMU_DATA_BATCH - 1);
  double K_Prv_AcData_Z = (Prv_AcData[IMU_DATA_BATCH - 1].AcZ - Prv_AcData[0].AcZ)/(IMU_DATA_BATCH - 1);
  IMU_AcData_Type Tmp_Max_AcData;
  IMU_AcData_Type Tmp_Min_AcData;
  IMU_GyData_Type Tmp_Max_GyData;
  IMU_GyData_Type Tmp_Min_GyData;
  IMU_AcData_Type Tmp_Ave_AcData;
  IMU_GyData_Type Tmp_Ave_GyData;
  Prv_Max_Data(&Tmp_Max_AcData, &Tmp_Max_GyData);
  Prv_Min_Data(&Tmp_Min_AcData, &Tmp_Min_GyData);
  Prv_Ave_Data(&Tmp_Ave_AcData, &Tmp_Ave_GyData);
  /*
  if(K_Prv_AcData_Z < (Prv_AcData[1].AcZ - Prv_AcData[0].AcZ) && K_Prv_AcData_Z > (Prv_AcData[IMU_DATA_BATCH - 1].AcZ - Prv_AcData[IMU_DATA_BATCH - 2].AcZ)){
      if(Tmp_Max_AcData.AcZ >= JUMP_VALVE_ACZ && T_Cmd.Command == CHEST_MID){
      T_Cmd.Command = CHEST_JUMP;
      Action_Found = true;
      Serial.println("CHEST_JUMP");
      return RetVal;
      }
    } */
  if(abs(Tmp_Ave_AcData.AcX - 128)<=1){
    Last_Highest_Point_X = 128;
    Last_Lowest_Point_X = 128;
    
    if(T_Cmd.Command != CHEST_MID){
      T_Cmd.Command = CHEST_MID;
      Action_Found = true;
      Serial.println("CHEST_MID");
    }
    return RetVal;
  }
  if(K_Prv_AcData_X < (Prv_AcData[1].AcX - Prv_AcData[0].AcX) && K_Prv_AcData_X > (Prv_AcData[IMU_DATA_BATCH - 1].AcX - Prv_AcData[IMU_DATA_BATCH - 2].AcX)){
      Last_Highest_Point_X = Tmp_Max_AcData.AcX;
    } 
  else if(K_Prv_AcData_X > (Prv_AcData[1].AcX - Prv_AcData[0].AcX) && K_Prv_AcData_X < (Prv_AcData[IMU_DATA_BATCH - 1].AcX - Prv_AcData[IMU_DATA_BATCH - 2].AcX)){
      Last_Lowest_Point_X = Tmp_Min_AcData.AcX;
    }
  Last_Mid_Point_X = (Last_Highest_Point_X + Last_Lowest_Point_X)/2;
  //Serial.print("Last_Highest_Point_X");
  //Serial.print(Last_Highest_Point_X);
  //Serial.print("Last_Lowest_Point_X");
  //Serial.print(Last_Lowest_Point_X);
  if(Last_Mid_Point_X > 0){
    if(Last_Mid_Point_X > LEFT_VALVE_ACX){
      RetVal = CHEST_LEFT;
      
      if(T_Cmd.Command != CHEST_LEFT){
        T_Cmd.Command = CHEST_LEFT;
        Action_Found = true;
        Serial.println("CHEST_LEFT");
      }
      return RetVal;
    }
    else if(Last_Mid_Point_X < RIGHT_VALVE_ACX){
      RetVal = CHEST_RIGHT;
      
      if(T_Cmd.Command != CHEST_RIGHT){
        T_Cmd.Command = CHEST_RIGHT;
        Action_Found = true;
        Serial.println("CHEST_RIGHT");
      }
      return RetVal;      
    }
  }
  //
  if(T_Cmd.Command != CHEST_MID){
    T_Cmd.Command = CHEST_MID;
    Action_Found = true;
    Serial.println("CHEST_MID");
  }
  Last_Highest_Point_X = 128;
  Last_Lowest_Point_X = 128;
  return RetVal;
}

Std_ReturnType Left_Arm_Motion_Detect(void){
  IMU_AcData_Type Tmp_Max_AcData;
  IMU_AcData_Type Tmp_Min_AcData;
  IMU_GyData_Type Tmp_Max_GyData;
  IMU_GyData_Type Tmp_Min_GyData;
  IMU_AcData_Type Tmp_Ave_AcData;
  IMU_GyData_Type Tmp_Ave_GyData;
  
  Prv_Max_Data(&Tmp_Max_AcData, &Tmp_Max_GyData);
  Prv_Min_Data(&Tmp_Min_AcData, &Tmp_Min_GyData);
  Prv_Ave_Data(&Tmp_Ave_AcData, &Tmp_Ave_GyData);
  /*
  if(Fit_Ellipse(Tmp_Min_AcData.AcZ,Tmp_Max_GyData.GyX,&Switch_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = LEFT_ARM_SEE_THROUGH;
    Serial.println("SEE_THROUGH+++++++++++++++");
    return LEFT_ARM_SEE_THROUGH;
  }
  
  if(Fit_Ellipse(Tmp_Max_AcData.AcY,Tmp_Min_GyData.GyZ,&Switch_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = LEFT_ARM_SWITCH;
    Serial.println("SWITCH********************");
    return LEFT_ARM_SWITCH;
  }*/
  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Ctrl_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = LEFT_ARM_CTRL;
    Serial.println("CTRL!!!!!!!!!!!!!");
    return LEFT_ARM_CTRL;
  }
  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Hook_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = LEFT_ARM_HOOK;
    Serial.println("HOOK--------------");
    return LEFT_ARM_HOOK;
  }
 return 0;
}

Std_ReturnType Right_Arm_Motion_Detect(void){
  IMU_AcData_Type Tmp_Ave_AcData;
  IMU_GyData_Type Tmp_Ave_GyData;
  Prv_Ave_Data(&Tmp_Ave_AcData, &Tmp_Ave_GyData);
  static uint8_t Continus_Attack_SP = 0;

  //Serial.print(Tmp_Ave_GyData.GyY);
  //Serial.print("    ");
  //Serial.println(Tmp_Ave_GyData.GyZ);
  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Attack_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = RIGHT_ARM_ATTACK;
    Serial.println("Attack!!!!!!!!!!!!!");
    Continus_Attack_SP = 0;
    return RIGHT_ARM_ATTACK;
  }
  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Defend_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = RIGHT_ARM_DEFEND;
    Serial.println("Defend--------------");
    Continus_Attack_SP = 0;
    return RIGHT_ARM_DEFEND;
  }
  if(Fit_Ellipse(Tmp_Ave_GyData.GyX,Tmp_Ave_GyData.GyZ,&Aim_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = RIGHT_ARM_TARGET;
    Serial.println("Target+++++++++++++++");
    Continus_Attack_SP = 0;
    return RIGHT_ARM_TARGET;
  }
  if(abs(Tmp_Ave_AcData.AcY - ATTACK_SP_VALVE) <= ATTACK_SP_DEV_VALVE){
    Continus_Attack_SP += 1;
    if(Continus_Attack_SP >= ATTACK_SP_CONTINUS_VALVE){
      Continus_Attack_SP = 0;
      if(T_Cmd.Command != RIGHT_ARM_ATTACK_SP){
        Serial.println("ATTACK_SP");
        T_Cmd.Command = RIGHT_ARM_ATTACK_SP;
        Action_Found = true;
      }
    }
    return RIGHT_ARM_ATTACK_SP;
  }
  Continus_Attack_SP = 0;
 return 0;
}




void Motion_Check_Left_Leg(void){
  int16_t AcX_Sum = 0;
  int16_t AcY_Sum = 0;
  int16_t AcZ_Sum = 0;
  int16_t GyX_Sum = 0;
  int16_t GyY_Sum = 0;
  int16_t GyZ_Sum = 0;

  static uint16_t Last_Record_ISR_Count = 0;
  static Motion_Point_Type Motion_Point_Status = MOTION_STAND_POINT;
  static uint8_t Continus_High_Speed = 0;
  static uint8_t Continus_Low_Speed = 0;
    
  for(uint8_t index = 0; index < IMU_DATA_BATCH;index ++){
    AcX_Sum += Prv_AcData[index].AcX;
    AcY_Sum += Prv_AcData[index].AcY;
    AcZ_Sum += Prv_AcData[index].AcZ;
    GyX_Sum += Prv_GyData[index].GyX;
    GyY_Sum += Prv_GyData[index].GyY;
    GyZ_Sum += Prv_GyData[index].GyZ;
  }
  AcX_Sum = AcX_Sum/IMU_DATA_BATCH;
  AcY_Sum = AcY_Sum/IMU_DATA_BATCH;
  AcZ_Sum = AcZ_Sum/IMU_DATA_BATCH;
  GyX_Sum = GyX_Sum/IMU_DATA_BATCH;
  GyY_Sum = GyY_Sum/IMU_DATA_BATCH;
  GyZ_Sum = GyZ_Sum/IMU_DATA_BATCH;

  switch(Motion_Point_Status){
    case MOTION_STAND_POINT:
        if(GyZ_Sum <= 115){
          Motion_Point_Status = MOTION_LOW_POINT;
          Last_Record_ISR_Count = ISR_Count;
          if(T_Cmd.Command != LEFT_LEG_WALK){
            Serial.println("WALK");
            T_Cmd.Command = LEFT_LEG_WALK;
            Action_Found = true;
          }
        }
        else if(GyZ_Sum >= 135){
          Motion_Point_Status = MOTION_HIGH_POINT;
          Last_Record_ISR_Count = ISR_Count;
          if(T_Cmd.Command != LEFT_LEG_WALK){
            Serial.println("WALK");
            T_Cmd.Command = LEFT_LEG_WALK;
            Action_Found = true;
          }
        }
        break;
    case MOTION_LOW_POINT:
        if(GyZ_Sum >= 135){
          Motion_Point_Status = MOTION_HIGH_POINT;
          Serial.println(ISR_Count - Last_Record_ISR_Count);
          if(ISR_Count - Last_Record_ISR_Count <= RUN_VALVE){
            Continus_Low_Speed = 0;
            Continus_High_Speed += 1;
            if(Continus_High_Speed == 3){
              Continus_High_Speed = 0;
              if(T_Cmd.Command != LEFT_LEG_RUN){
                Serial.println("RUN");
                T_Cmd.Command = LEFT_LEG_RUN;
                Action_Found = true;
              }
            }
          }
          else{
            Continus_High_Speed = 0;
            Continus_Low_Speed += 1;
            if(Continus_Low_Speed == 3){
              Continus_Low_Speed = 0;
              if(T_Cmd.Command != LEFT_LEG_WALK){
                Serial.println("WALK");
                T_Cmd.Command = LEFT_LEG_WALK;
                Action_Found = true;
              }
            }
          }
          Last_Record_ISR_Count = ISR_Count;
        }
        if(ISR_Count - Last_Record_ISR_Count >= WALK_VALVE){
          Motion_Point_Status = MOTION_STAND_POINT;
          if(T_Cmd.Command != LEFT_LEG_STOP){
            Serial.println("Stopped");
            T_Cmd.Command = LEFT_LEG_STOP;
            Action_Found = true;
          }
        }
        break;
    case MOTION_HIGH_POINT:
        if(GyZ_Sum <= 115){
          Motion_Point_Status = MOTION_LOW_POINT;
          Serial.println(ISR_Count - Last_Record_ISR_Count);
          if(ISR_Count - Last_Record_ISR_Count <= RUN_VALVE){
            Continus_Low_Speed = 0;
            Continus_High_Speed += 1;
            if(Continus_High_Speed == 3){
              Continus_High_Speed = 0;
              if(T_Cmd.Command != LEFT_LEG_RUN){
                Serial.println("RUN");
                T_Cmd.Command = LEFT_LEG_RUN;
                Action_Found = true;
              }
            }
          }
          else{
            Continus_High_Speed = 0;
            Continus_Low_Speed += 1;
            if(Continus_Low_Speed == 3){
              Continus_Low_Speed = 0;
              if(T_Cmd.Command != LEFT_LEG_WALK){
                Serial.println("WALK");
                T_Cmd.Command = LEFT_LEG_WALK;
                Action_Found = true;
              }
            }
          }
          Last_Record_ISR_Count = ISR_Count;
        }
        if(ISR_Count - Last_Record_ISR_Count >= WALK_VALVE){
          Motion_Point_Status = MOTION_STAND_POINT;
          if(T_Cmd.Command != LEFT_LEG_STOP){
            Serial.println("Stopped");
            T_Cmd.Command = LEFT_LEG_STOP;
            Action_Found = true;
          }
        }
        break;
  }
}

void Motion_Check_Tool(void){
  IMU_AcData_Type Tmp_Ave_AcData;
  IMU_GyData_Type Tmp_Ave_GyData;
  static uint8_t Continus_Tool_Use = 0;
  static uint8_t Continus_Tool_Unuse = 0;
  Prv_Ave_Data(&Tmp_Ave_AcData, &Tmp_Ave_GyData);
  if(Tmp_Ave_AcData.AcY > TOOL_USE_VALVE){
    Continus_Tool_Use += 1;
    Continus_Tool_Unuse = 0;
    if(Continus_Tool_Use >= TOOL_CONTINUS_VALVE){
      Continus_Tool_Use = 0;
      if(T_Cmd.Command != TOOL_USE){
        Serial.println("Tool_Use");
        T_Cmd.Command = TOOL_USE;
        Action_Found = true;
      }
    }
  }
  if(Tmp_Ave_AcData.AcY < TOOL_UNUSE_VALVE){
    Continus_Tool_Unuse += 1;
    Continus_Tool_Use = 0;
    if(Continus_Tool_Unuse >= TOOL_CONTINUS_VALVE){
      Continus_Tool_Unuse = 0;
      if(T_Cmd.Command != TOOL_UNUSE){
        Serial.println("Tool_not_use");
        T_Cmd.Command = TOOL_UNUSE;
        Action_Found = true;
      }
    }
  }
}

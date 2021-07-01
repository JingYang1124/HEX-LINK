#include "Universal_Headers.h"
#include "IMU.h"
#include "COM.h"
#include "ISR.h"
#include "Motion_Detect.h"


bool    Action_Found = false;
uint8_t Silent_Count = SILENT_VALVE;

#define LEFT_VALVE_ACX  131
#define RIGHT_VALVE_ACX 124
#define JUMP_VALVE_ACZ  130

const Ellipse_Para_Type Ctrl_Ellipse = {185,130,25,35,150};/* x_0,y_0,a,b,Theta */
const Ellipse_Para_Type Hook_Ellipse = {120,176,25,20,0};

const Ellipse_Para_Type Attack_Ellipse = {115,69,16,30,118};
const Ellipse_Para_Type Defend_Ellipse = {81,141,12,20,109};
const Ellipse_Para_Type Aim_Ellipse = {80,120,15,10,10};

#define WALK_VALVE  25
#define RUN_VALVE   14

#define TOOL_USE_VALVE    132
#define TOOL_UNUSE_VALVE  123
#define TOOL_CONTINUS_VALVE 5

Std_ReturnType Fit_Ellipse(float x, float y, const Ellipse_Para_Type * Elli_Para){
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

void Prv_Init_Data(void){
    for(uint8_t index = 0; index < IMU_DATA_BATCH;index ++){
    Prv_AcData[index].AcX = IMU_DATA_INIT_VALUE;
    Prv_AcData[index].AcY = IMU_DATA_INIT_VALUE;
    Prv_AcData[index].AcZ = IMU_DATA_INIT_VALUE;
    Prv_GyData[index].GyX = IMU_DATA_INIT_VALUE;
    Prv_GyData[index].GyY = IMU_DATA_INIT_VALUE;
    Prv_GyData[index].GyZ = IMU_DATA_INIT_VALUE;
  }
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
  static uint8_t Last_Highest_Point_X = 128;
  static uint8_t Last_Lowest_Point_X = 127;
  static double Last_Mid_Point_X = 128;
  double K_Prv_AcData_X = (Prv_AcData[IMU_DATA_BATCH - 1].AcX - Prv_AcData[0].AcX)/(IMU_DATA_BATCH - 1);
  IMU_AcData_Type Tmp_Max_AcData;
  IMU_AcData_Type Tmp_Min_AcData;
  IMU_GyData_Type Tmp_Max_GyData;
  IMU_GyData_Type Tmp_Min_GyData;
  IMU_AcData_Type Tmp_Ave_AcData;
  IMU_GyData_Type Tmp_Ave_GyData;
  Prv_Max_Data(&Tmp_Max_AcData, &Tmp_Max_GyData);
  Prv_Min_Data(&Tmp_Min_AcData, &Tmp_Min_GyData);
  Prv_Ave_Data(&Tmp_Ave_AcData, &Tmp_Ave_GyData);

  if(abs(Tmp_Ave_AcData.AcX - 128)<=1){
    Last_Highest_Point_X = 128;
    Last_Lowest_Point_X = 128;
    
    if(T_Cmd.Command != CHEST_MID){
      T_Cmd.Command = CHEST_MID;
      Action_Found = true;
      Serial.println("CHEST_MID");
    }
    return CHEST_MID;
  }
  if(K_Prv_AcData_X < (Prv_AcData[1].AcX - Prv_AcData[0].AcX) && K_Prv_AcData_X > (Prv_AcData[IMU_DATA_BATCH - 1].AcX - Prv_AcData[IMU_DATA_BATCH - 2].AcX)){
      Last_Highest_Point_X = Tmp_Max_AcData.AcX;
    } 
  else if(K_Prv_AcData_X > (Prv_AcData[1].AcX - Prv_AcData[0].AcX) && K_Prv_AcData_X < (Prv_AcData[IMU_DATA_BATCH - 1].AcX - Prv_AcData[IMU_DATA_BATCH - 2].AcX)){
      Last_Lowest_Point_X = Tmp_Min_AcData.AcX;
    }
  Last_Mid_Point_X = (Last_Highest_Point_X + Last_Lowest_Point_X)/2;
  if(Last_Mid_Point_X > 0){
    if(Last_Mid_Point_X > LEFT_VALVE_ACX){ 
      if(T_Cmd.Command != CHEST_LEFT){
        T_Cmd.Command = CHEST_LEFT;
        Action_Found = true;
        Serial.println("CHEST_LEFT");
      }
      return CHEST_LEFT;
    }
    else if(Last_Mid_Point_X < RIGHT_VALVE_ACX){
      if(T_Cmd.Command != CHEST_RIGHT){
        T_Cmd.Command = CHEST_RIGHT;
        Action_Found = true;
        Serial.println("CHEST_RIGHT");
      }
      return CHEST_RIGHT;      
    }
  }
  if(T_Cmd.Command != CHEST_MID){
    T_Cmd.Command = CHEST_MID;
    Action_Found = true;
    Serial.println("CHEST_MID");
  }
  Last_Highest_Point_X = 128;
  Last_Lowest_Point_X = 128;
  return CHEST_MID;
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

  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Ctrl_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = LEFT_ARM_CTRL;
    Serial.println("Ctrl");
    return LEFT_ARM_CTRL;
  }
  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Hook_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = LEFT_ARM_HOOK;
    Serial.println("Hook");
    return LEFT_ARM_HOOK;
  }
 return DEFAULT_COMMAND;
}

Std_ReturnType Right_Arm_Motion_Detect(void){
  IMU_AcData_Type Tmp_Ave_AcData;
  IMU_GyData_Type Tmp_Ave_GyData;
  Prv_Ave_Data(&Tmp_Ave_AcData, &Tmp_Ave_GyData);
  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Attack_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = RIGHT_ARM_ATTACK;
    Serial.println("Attack");
    return RIGHT_ARM_ATTACK;
  }
  if(Fit_Ellipse(Tmp_Ave_GyData.GyY,Tmp_Ave_GyData.GyZ,&Defend_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = RIGHT_ARM_DEFEND;
    Serial.println("Defend");
    return RIGHT_ARM_DEFEND;
  }
  if(Fit_Ellipse(Tmp_Ave_GyData.GyX,Tmp_Ave_GyData.GyZ,&Aim_Ellipse)){
    Action_Found = true;
    T_Cmd.Command = RIGHT_ARM_TARGET;
    Serial.println("Target");
    return RIGHT_ARM_TARGET;
  }
 return DEFAULT_COMMAND;
}




Std_ReturnType Motion_Check_Left_Leg(void){
  static uint16_t Last_Record_ISR_Count = 0;
  static Motion_Point_Type Motion_Point_Status = MOTION_STAND_POINT;
  static uint8_t Continus_High_Speed = 0;
  static uint8_t Continus_Low_Speed = 0;
  IMU_AcData_Type Tmp_Ave_AcData;
  IMU_GyData_Type Tmp_Ave_GyData;
  Prv_Ave_Data(&Tmp_Ave_AcData, &Tmp_Ave_GyData);

  switch(Motion_Point_Status){
    case MOTION_STAND_POINT:
        if(Tmp_Ave_GyData.GyZ <= 115){
          Motion_Point_Status = MOTION_LOW_POINT;
          Last_Record_ISR_Count = ISR_Count;
          if(T_Cmd.Command != LEFT_LEG_WALK){
            Serial.println("Walk");
            T_Cmd.Command = LEFT_LEG_WALK;
            Action_Found = true;
          }
        }
        else if(Tmp_Ave_GyData.GyZ >= 135){
          Motion_Point_Status = MOTION_HIGH_POINT;
          Last_Record_ISR_Count = ISR_Count;
          if(T_Cmd.Command != LEFT_LEG_WALK){
            Serial.println("Walk");
            T_Cmd.Command = LEFT_LEG_WALK;
            Action_Found = true;
          }
        }
        break;
    case MOTION_LOW_POINT:
        if(Tmp_Ave_GyData.GyZ >= 135){
          Motion_Point_Status = MOTION_HIGH_POINT;
          Serial.println(ISR_Count - Last_Record_ISR_Count);
          if(ISR_Count - Last_Record_ISR_Count <= RUN_VALVE){
            Continus_Low_Speed = 0;
            Continus_High_Speed += 1;
            if(Continus_High_Speed == 3){
              Continus_High_Speed = 0;
              if(T_Cmd.Command != LEFT_LEG_RUN){
                Serial.println("Run");
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
                Serial.println("Walk");
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
        if(Tmp_Ave_GyData.GyZ <= 115){
          Motion_Point_Status = MOTION_LOW_POINT;
          Serial.println(ISR_Count - Last_Record_ISR_Count);
          if(ISR_Count - Last_Record_ISR_Count <= RUN_VALVE){
            Continus_Low_Speed = 0;
            Continus_High_Speed += 1;
            if(Continus_High_Speed == 3){
              Continus_High_Speed = 0;
              if(T_Cmd.Command != LEFT_LEG_RUN){
                Serial.println("Run");
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
                Serial.println("Walk");
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
  return T_Cmd.Command;
}

Std_ReturnType Motion_Check_Tool(void){
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
      return TOOL_USE;
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
      return TOOL_UNUSE;
    }
  }
  return TOOL_UNUSE;
}



#include "Universal_Headers.h"
#include "IMU.h"
#include "Wire.h"
#include "Motion_Detect.h"

const int MPU_addr=0x68; // I2C address of the MPU-6050


IMU_AcData_Type Current_AcData;
IMU_AcData_Type Prv_AcData[IMU_DATA_BATCH];

IMU_GyData_Type Current_GyData;
IMU_GyData_Type Prv_GyData[IMU_DATA_BATCH];

bool Obtain_New_IMU_Data = 0;


void Update_Prv_AcData(void){
  for(uint8_t index = 0; index < IMU_DATA_BATCH - 1; index ++)
  {
    Prv_AcData[index].AcX = Prv_AcData[index + 1].AcX;
    Prv_AcData[index].AcY = Prv_AcData[index + 1].AcY;
    Prv_AcData[index].AcZ = Prv_AcData[index + 1].AcZ;
  }
  Prv_AcData[IMU_DATA_BATCH - 1].AcX = Current_AcData.AcX;
  Prv_AcData[IMU_DATA_BATCH - 1].AcY = Current_AcData.AcY;
  Prv_AcData[IMU_DATA_BATCH - 1].AcZ = Current_AcData.AcZ; 
}

void Update_Prv_GyData(void){
  for(uint8_t index = 0; index < IMU_DATA_BATCH - 1; index ++)
  {
    Prv_GyData[index].GyX = Prv_GyData[index + 1].GyX;
    Prv_GyData[index].GyY = Prv_GyData[index + 1].GyY;
    Prv_GyData[index].GyZ = Prv_GyData[index + 1].GyZ;
  }
  Prv_GyData[IMU_DATA_BATCH - 1].GyX = Current_GyData.GyX;
  Prv_GyData[IMU_DATA_BATCH - 1].GyY = Current_GyData.GyY;
  Prv_GyData[IMU_DATA_BATCH - 1].GyZ = Current_GyData.GyZ;
}

void IMU_Measure_Ac(IMU_Raw_AcData_Type * AcData){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,true); // request a total of 6 registers

  AcData->AcX = Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcData->AcY = Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcData->AcZ = Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
}

void IMU_Measure_Gy(IMU_Raw_GyData_Type * GyData){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43); // starting with register 0x43 (GYRO_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,true); // request a total of 6 registers

  GyData->GyX = Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyData->GyY = Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyData->GyZ = Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) 
}

void IMU_Read_Average_Ac(uint16_t READ_NUM, IMU_AcData_Type * OutAcData){
  IMU_Raw_AcData_Type TempAcData;
  IMU_Raw_AcData_Type SumAcData;
  TempAcData.AcX = 0;
  TempAcData.AcY = 0;
  TempAcData.AcZ = 0;
  SumAcData.AcX = 0;
  SumAcData.AcY = 0;
  SumAcData.AcZ = 0;
  OutAcData->AcX = 0;
  OutAcData->AcY = 0;
  OutAcData->AcZ = 0;
  for(uint16_t count = 0; count <=(READ_NUM - 1); count++)
  {
    IMU_Measure_Ac(&TempAcData);
    SumAcData.AcX += (TempAcData.AcX/400 + IMU_DATA_INIT_VALUE);  
    SumAcData.AcY += (TempAcData.AcY/400 + IMU_DATA_INIT_VALUE);
    SumAcData.AcZ += (TempAcData.AcZ/400 + IMU_DATA_INIT_VALUE);
  }
  OutAcData->AcX = (uint8_t)(SumAcData.AcX/READ_NUM);
  OutAcData->AcY = (uint8_t)(SumAcData.AcY/READ_NUM);
  OutAcData->AcZ = (uint8_t)(SumAcData.AcZ/READ_NUM);
  
  #if defined(IMU_READ_SERIAL_PRINT)
    Serial.print("AcX:");  
    Serial.print(OutAcData->AcX);
    Serial.print("  AcY:");  
    Serial.print(OutAcData->AcY);
    Serial.print("  AcZ:");  
    Serial.println(OutAcData->AcZ);
  #endif
  
}

void IMU_Read_Average_Gy(uint16_t READ_NUM, IMU_GyData_Type * OutGyData){
  IMU_Raw_GyData_Type TempGyData;
  IMU_Raw_GyData_Type SumGyData;
  TempGyData.GyX = 0;
  TempGyData.GyY = 0;
  TempGyData.GyZ = 0;
  SumGyData.GyX = 0;
  SumGyData.GyY = 0;
  SumGyData.GyZ = 0;
  OutGyData->GyX = 0;
  OutGyData->GyY = 0;
  OutGyData->GyZ = 0;
  for(uint16_t count = 0; count <=(READ_NUM - 1); count++)
  {
    IMU_Measure_Gy(&TempGyData);
    SumGyData.GyX += (TempGyData.GyX/400 + IMU_DATA_INIT_VALUE);  
    SumGyData.GyY += (TempGyData.GyY/400 + IMU_DATA_INIT_VALUE);
    SumGyData.GyZ += (TempGyData.GyZ/400 + IMU_DATA_INIT_VALUE);
  }
  OutGyData->GyX = (uint8_t)(SumGyData.GyX/READ_NUM);
  OutGyData->GyY = (uint8_t)(SumGyData.GyY/READ_NUM);
  OutGyData->GyZ = (uint8_t)(SumGyData.GyZ/READ_NUM);
  
  #if defined(IMU_READ_SERIAL_PRINT)
    Serial.print("GyX:");  
    Serial.print(OutGyData->GyX);
    Serial.print("  GyY:");  
    Serial.print(OutGyData->GyY);
    Serial.print("  GyZ:");  
    Serial.println(OutGyData->GyZ);
  #endif
  
}

void IMU_Init(void){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(100);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1C); // 
  Wire.write(0x10); // Full range is +-8g, and 4000 is 1g
  Wire.endTransmission(true);
  delay(100);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B); //
  Wire.write(0x10); // Full range is +-1000Degree/s, and 32.4 is 1Degree/s
  Wire.endTransmission(true);  

  Prv_Init_Data();
}

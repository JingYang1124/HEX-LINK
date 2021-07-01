#ifndef TYPES_DEFINE_H
#define TYPES_DEFINE_H

#include <inttypes.h>

#define P2FUNC(rettype, fctname) rettype (* fctname)
typedef int8_t   Std_ReturnType;
typedef P2FUNC(void, ApiType) (void);


#define E_OK       0
#define E_NOT_OK   1

typedef enum{
	DEVICE_CHEST,
	DEVICE_LEFT_ARM,
	DEVICE_RIGHT_ARM,
	DEVICE_LEFT_LEG,
  DEVICE_TOOL,
	DEVICE_RIGHT_LEG,
	DEVICE_WEAPON,
  DEVICE_TEST
}DeviceIdType;



typedef struct{
  int16_t AcX;
  int16_t AcY;
  int16_t AcZ;
}IMU_Raw_AcData_Type;

typedef struct{
  uint8_t AcX;
  uint8_t AcY;
  uint8_t AcZ;
}IMU_AcData_Type;

typedef struct{
  int16_t GyX;
  int16_t GyY;
  int16_t GyZ;
}IMU_Raw_GyData_Type;

typedef struct{
  uint8_t GyX;
  uint8_t GyY;
  uint8_t GyZ;
}IMU_GyData_Type;

typedef struct{
  byte ThisDeviceId;
  byte Command;
  byte Operation_Index;
}CommandType;


typedef enum{
  MOTION_STAND_POINT,
  MOTION_LOW_POINT,
  MOTION_HIGH_POINT
}Motion_Point_Type;

typedef struct{
  float x_0;
  float y_0;
  float a;
  float b;
  float Theta;
}Ellipse_Para_Type;


#endif

#ifndef TYPES_DEFINE_H
#define TYPES_DEFINE_H

#include <inttypes.h>

#define P2FUNC(rettype, fctname) rettype (* fctname)
typedef int8_t   Std_ReturnType;
typedef P2FUNC(Std_ReturnType, ApiType) (void);



typedef enum{
	DEVICE_CHEST,
	DEVICE_LEFT_ARM,
	DEVICE_RIGHT_ARM,
	DEVICE_LEFT_LEG,
  	DEVICE_TOOL,
	DEVICE_RIGHT_LEG,
	DEVICE_WEAPON,
}DeviceIdType;


typedef struct{
  byte ThisDeviceId;
  byte Command;
  byte Operation_Index;
}CommandType;

#endif

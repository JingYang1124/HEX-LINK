#ifndef IMU_H
#define IMU_H


#define IMU_DEVICE MPU6050

#define IMU_AVERAGE_READ_NUM  8
#define IMU_DATA_BATCH   5
#define ACDATA_ARRAY_NUM 50
#define GREAT_VALVE_X      190
#define GREAT_VALVE_Y      180
#define GREAT_VALVE_Z      175
#define SMALL_VALVE_Z      65

extern IMU_Measure_StateMachine_Type Current_IMU_Measure_State;


extern uint8_t Continus_Count;

extern void Update_Prv_AcData(void);
extern void Update_Prv_GyData(void);
extern IMU_AcData_Type Current_AcData;
extern IMU_AcData_Type Prv_AcData[IMU_DATA_BATCH];
extern IMU_AcData_Type AcData_Array[ACDATA_ARRAY_NUM];

extern IMU_GyData_Type Current_GyData;
extern IMU_GyData_Type Prv_GyData[IMU_DATA_BATCH];
extern IMU_GyData_Type GyData_Array[ACDATA_ARRAY_NUM];

extern bool IMU_Start_Point_Check(void);
extern void IMU_Measure_Ac(IMU_Raw_AcData_Type * AcData);
extern void IMU_Read_Average_Ac(uint16_t READ_NUM, IMU_AcData_Type * OutAcData);
extern void IMU_Measure_Gy(IMU_Raw_GyData_Type * GyData);
extern void IMU_Read_Average_Gy(uint16_t READ_NUM, IMU_GyData_Type * OutGyData);
extern void IMU_Init(void);

#endif

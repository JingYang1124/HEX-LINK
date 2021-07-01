#ifndef MOTION_DETECT_H
#define MOTION_DETECT_H


#define BAD_POINT_VALVE  10
#define CONCAVE_CONVEX_CHECK_NUM  8
#define SILENT_VALVE  10

#define TARGETTING_X_AVERAGE    128
#define TARGETTING_Y_AVERAGE    128
#define TARGETTING_Z_AVERAGE    122

extern uint8_t Silent_Count;
extern bool Action_Found;

extern Std_ReturnType Left_Arm_Motion_Detect(void);
extern Std_ReturnType Right_Arm_Motion_Detect(void);
extern Std_ReturnType Chest_Motion_Detect(void);

extern bool Smooth_Check_Left_Arm(void);
extern void Concave_Convex_Check_Left_Arm(void);
extern void Motion_Check_Left_Leg(void);
extern void Motion_Check_Tool(void);
#endif

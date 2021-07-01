#ifndef ISR_H
#define ISR_H

#define Period 10
#define Duration_Period_Num 6
#define ISR_IMU_Measure_Period 20 //unit:ms

extern uint16_t ISR_Count;
extern uint16_t ISR_Count_Send;
extern void flash(void);
extern void ISR_IMU_Measure(void);
extern void ISR_Init(void);

#endif

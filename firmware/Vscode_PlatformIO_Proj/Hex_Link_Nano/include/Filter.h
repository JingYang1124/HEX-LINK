#ifndef FILTER_H
#define FILTER_H

#define FILTER_ORDER 4

typedef struct{
    float x[FILTER_ORDER+1];
    float y[FILTER_ORDER+1];
}IIR_Filter_State_Type;

extern IIR_Filter_State_Type IIR_Filter_State_AcX;
extern IIR_Filter_State_Type IIR_Filter_State_AcY;
extern IIR_Filter_State_Type IIR_Filter_State_AcZ;
extern IIR_Filter_State_Type IIR_Filter_State_GyX;
extern IIR_Filter_State_Type IIR_Filter_State_GyY;
extern IIR_Filter_State_Type IIR_Filter_State_GyZ;

extern float IIR_Filter(IIR_Filter_State_Type* filter_state,  float x_0);
extern void IIR_Filter_state_Init(IIR_Filter_State_Type* filter_state);

#endif

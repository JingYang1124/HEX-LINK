#include "Universal_Headers.h"
#include "Filter.h"

/*
   Filter type: Butterworth
   Order of filter: 4
   cut-off frequency: 10Hz
   sample frequency: 50Hz
*/

float IIR_B[FILTER_ORDER + 1] = {
  0.04658290663644364,
  0.18633162654577456,
  0.2794974398186618,
  0.18633162654577456,
  0.04658290663644364
};
float IIR_A[FILTER_ORDER + 1] = {
  1.0,
  -0.7820951980233378,
  0.6799785269162995,
  -0.1826756977530324,
  0.03011887504316925
};


IIR_Filter_State_Type IIR_Filter_State_AcX;
IIR_Filter_State_Type IIR_Filter_State_AcY;
IIR_Filter_State_Type IIR_Filter_State_AcZ;
IIR_Filter_State_Type IIR_Filter_State_GyX;
IIR_Filter_State_Type IIR_Filter_State_GyY;
IIR_Filter_State_Type IIR_Filter_State_GyZ;

/*
  filter_out[i] = IIR_Filter(&IIR_Filter,in_data[i]);
*/
float IIR_Filter(IIR_Filter_State_Type* filter_state,  float x_0){
	int i;
	float tmp = 0;
  /*
    Update the state of the filter
  */
	for(i = FILTER_ORDER; i > 0 ;i--)
    {
        filter_state->x[i] = filter_state->x[i-1];
        filter_state->y[i] = filter_state->y[i-1];
    }
	filter_state->x[0] = x_0;
	/*
    Calculate the output of filter
  */
	for(i = 0; i <= FILTER_ORDER;i++)
    {
        tmp += (IIR_B[i]* filter_state->x[i]);
    }
    for(i = 1; i <= FILTER_ORDER;i++)
    {
        tmp -= (IIR_A[i]* filter_state->y[i]);
    }
    tmp /= IIR_A[0];
	filter_state->y[0] = tmp;
	return tmp;
}

void IIR_Filter_state_Init(IIR_Filter_State_Type* filter_state){
  for(int i = 0; i < FILTER_ORDER+1;i++)
    {
        filter_state->x[i] = 0;
        filter_state->y[i] = 0;
    }
}

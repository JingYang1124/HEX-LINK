#ifndef COM_H
#define COM_H


#define MY_ADDRESS       "Nano"
#define TARGER_ADDRESS   "Leo"

#define NRF_CHANNEL      3

#define DEFAULT_COMMAND  1
#define DEFAULT_INDEX    1

/* CHEST COMMAND*/
#define CHEST_MID        2
#define CHEST_LEFT       3
#define CHEST_RIGHT      4
#define CHEST_JUMP       5

/* LEFT ARM COMMAND*/
#define LEFT_ARM_HOOK    2
#define LEFT_ARM_SEE_THROUGH     3
#define LEFT_ARM_CTRL    4
#define LEFT_ARM_SWITCH  5

/* RIGHT ARM COMMAND*/
#define RIGHT_ARM_ATTACK    2
#define RIGHT_ARM_DEFEND    3
#define RIGHT_ARM_TARGET    4
#define RIGHT_ARM_ATTACK_SP    5

/* LEFT LEG COMMAND*/
#define LEFT_LEG_STOP    2
#define LEFT_LEG_RUN     3
#define LEFT_LEG_WALK    4

/* TOOL COMMAND*/
#define TOOL_UNUSE       2
#define TOOL_USE         3


#define RE_SEND_COUNT    4

extern CommandType R_Cmd,T_Cmd;
extern void NRF_Init(void);
extern void NRF_Send(CommandType * Tdata);
extern void NRF_Read(void);
#endif

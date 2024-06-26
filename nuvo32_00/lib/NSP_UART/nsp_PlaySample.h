#ifndef _NSP_PLAY_SAMPLE_H_
#define _NSP_PLAY_SAMPLE_H_

#define ISP_BUF_SIZE   (25000)	

/* exact-width unsigned integer types */
typedef unsigned          char UINT8;
typedef unsigned short     int UINT16;
typedef unsigned           int UINT32;
typedef unsigned char *		PUINT8;
typedef unsigned short int  *	PUINT16;

#include <Arduino.h> //Arduinoライブラリをインクルード cpp形式
extern UINT8 ISP_Buffer[];

/*----- Waiting for playback to finish -----*/
void UART_WaitPlayEND(void);
/*----- Waiting for playback to finish -----*/
UINT8 UART_WaitExecutionEND(void);
/*----- Ask for status -----*/
UINT8 UART_AskStatus(void);

/*----- Index Play Sample -----*/
void UART_IndexPlaySample(void);
/*----- MultiPlay Sample -----*/
void UART_MultiPlaySample(void);
/*----- Sleep Wake Up Sample -----*/
void UART_SleepWakeUpSample(void);
/*----- Low Power Detection -----*/
void UART_LowPowerDetectionSample(void);
/*----- NSP IO Control Sample -----*/
void UART_NSP_IO_CtrlSample(void);
/*----- CheckSum Sample -----*/
void UART_CheckSumSample(void);
/*----- Busy Pin Set Sample -----*/
void UART_BusyPinSetSample(void);
/*----- Volume Control Sample -----*/
void UART_VolumeCtrlSample(void);
/*----- Read ID & FW Version Sample -----*/
void UART_ReadIdAndFwVerSample(void);
/*----- Mixed Sample 1 -----*/
void UART_MixedSample1(void);
/*----- Mixed Sample 2 -----*/
void UART_MixedSample2(void);
/*----- Mixed Sample 3 -----*/
void UART_MixedSample3(void);
/*----- NSP ISP Update all resource Sample -----*/
void UART_ISPUpdateAllResourceSample(void);
/*----- NSP ISP Update one resource Sample -----*/
void UART_ISPUpdateOneResourceSample(void);
/*----- Continue Play Sample -----*/
void UART_ContinuePlaySample(void);

/*----- NSP ISP Update user data Sample -----*/
UINT8 UART_UserDataWrite(UINT32 UserWrite_ADDR,PUINT8 ISP_BUFFER,UINT16 WriteSize);
/*----- NSP ISP read user data Sample -----*/
UINT8 UART_UserDataRead(UINT32 UserRead_ADDR,PUINT8 ISP_BUFFER,UINT16 ReadSize);
/*----- NSP ISP Update user space Sample -----*/
void UART_ISPUserSpaceWriteAndRead(void);

/*----- NSP Updata touch config -----*/
UINT8 UART_TouchUpdataSample(UINT16 u16ConfigSize);
/*----- NSP Load touch config -----*/
UINT8 UART_TouchLoadSample(UINT16 u16ConfigSize);

#endif





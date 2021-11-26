 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the TIMER AVR driver
 *
 * Author: Peter Maged
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_


#include "std_types.h"
#include<avr/io.h>

/* Timer configurations for timer number(0,1,2) and  channel selection (A,B)*/
#define Timer_number 0
#define Channel_number 0

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
#if (Timer_number ==0)
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	OVERFLOW=0,COMPARE=1
	/* the compare =2 to fit the insertion equation of the TIMSK register */
}Timer_Mode;
typedef enum
{
	DISABLE,ENABLE
}Timer_Interrupt_Mode;

typedef struct
{
	Timer_Clock clock;
	Timer_Mode mode;
	Timer_Interrupt_Mode interruptMode;
	uint8 initialValue ;
	uint8 compareValue ;
}Timer_ConfigType;
#elif(Timer_number ==1)
#if(Timer_channel ==0)
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	OVERFLOW=0,COMPARE=2
	/* the compare =2 to fit the insertion equation of the TIMSK register */
}Timer_Mode;
typedef enum
{
	DISABLE,ENABLE
}Timer_Interrupt_Mode;

typedef struct
{
	Timer_Clock clock;
	Timer_Mode mode;
	Timer_Interrupt_Mode interruptMode;
	uint16 initialValue ;
	uint16 compareValue ;
}Timer_ConfigType;
#elif(Timer_channel ==1)
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	OVERFLOW=0,COMPARE=1
	/* the compare =2 to fit the insertion equation of the TIMSK register */
}Timer_Mode;
typedef enum
{
	DISABLE,ENABLE
}Timer_Interrupt_Mode;

typedef struct
{
	Timer_Clock clock;
	Timer_Mode mode;
	Timer_Interrupt_Mode interruptMode;
	uint16 initialValue ;
	uint16 compareValue ;
}Timer_ConfigType;
#endif
#elif (Timer_number ==2)
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	OVERFLOW=0,COMPARE=1
	/* the compare =2 to fit the insertion equation of the TIMSK register */
}Timer_Mode;
typedef enum
{
	DISABLE,ENABLE
}Timer_Interrupt_Mode;

typedef struct
{
	Timer_Clock clock;
	Timer_Mode mode;
	Timer_Interrupt_Mode interruptMode;
	uint8 initialValue ;
	uint8 compareValue ;
}Timer_ConfigType;
#endif

void TIMER_init(const Timer_ConfigType * Config_Ptr);
void TIMER_setCallBack(void(*a_ptr)(void));
void TIMER_stop();
void TIMER_deinit(void);
void Timer_clearTimerValue();
uint16 Timer_getTimerValue();


#endif /* TIMER_H_ */





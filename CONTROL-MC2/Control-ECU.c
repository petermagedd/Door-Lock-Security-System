/******************************************************************************
 *
 * Module: Main Application for Control-ECU
 *
 * File Name: Control-ECU.c
 *
 * Description: Source file for the main application of Control ECU
 *
 * Author: Peter Maged
 *
 *******************************************************************************/

#include "UART_messages.h"
#include "buzzer.h"
#include "DcMotor.h"
#include "external_eeprom.h"
#include "twi.h"
#include "timer.h"
#include "uart.h"
#include <avr/delay.h>

#define TIMER_3_SECS  92
#define TIMER_15_SECS 457
#define TIMER_60_SECS 1831

/************************************************************************
 *                           Global variables                           *
 ************************************************************************/
/* variable that holds number of wrong trials for entering password */
uint8 g_trialNumber =0 ;
/* holds the UDR of UART whenever there's data in it */
volatile uint8 g_recievedValue=0;
/*Flag that is set to one whenever there's data in UDR */
volatile uint8 g_recievedFlag=0 ;
/* variable that holds number of buzzer ticks  */
uint16 tick_buzzer = 0;
/* variable that holds number of motor ticks  */
uint16 tick_motor = 0;
/* variable that holds number of motor stop ticks  */
uint16 tick_motor_stop = 0;
/*Flag that is set to one whenever Timer counts 1min */
uint8 g_flag_buzzer=0;
/*Flag that is set to one whenever Timer counts 15sec */
uint8 g_flag_motor=0;
/*Flag that is set to one whenever Timer counts 3sec */
uint8 g_flag_motor_stop=0;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * [function Name] : storeNewPassword
 * [Description] : A function to store new password in eeprom
 * [Args] :
 * [in] uint8* a_storedPassword_Ptr:
 * This argument shall indicate an array that is passed
 * by reference which store the password in it
 */
void storeNewPassword (uint8* a_storedPassword_Ptr);
/*
 * [function Name] : checkPassword
 * [Description] : A function to check entered password
 * [Args] :
 * [in] uint8* a_checkPassword_Ptr:
 * This argument shall indicate an array that is passed
 * by reference which store the password in it
 */
void checkPassword (uint8* a_checkPassword_Ptr) ;
/*
 * [function Name] : compareTwoPasswords
 * [Description] : A function to compare 2 passwords
 * [Args] :
 * [in] uint8* a_checkPassword_Ptr:
 * This argument shall indicate an array that is passed
 * by reference which store the first password in it
 * [in] uint8* a_checkPassword_Ptr:
 * This argument shall indicate an array that is passed
 * by reference which store the secoend password in it
 * [Returns] :
 */
uint8 compareTwoPasswords(uint8* a_passwordOne_Ptr, uint8* a_passwordTwo_Ptr) ;
/*
 * [function Name] : Timer0_Callbackfunc
 * [Description] : A function to count seconds needed for motor and buzzer
 */
void Timer0_Callbackfunc(void);
/*
 * [function Name] : buzzer_on
 * [Description] : A function to start the buzzer
 */
void buzzer_on(void);
/*
 * [function Name] : motor_on
 * [Description] : A function to start the motor
 */
void motor_on(void);

/* initialize TIMER */
Timer_ConfigType Timer_Config = {F_CPU_1024,OVERFLOW,ENABLE,0,0};

int main(void)
{
	/* 3 arrays used to store data (password) received from MC1*/
	uint8 arr_storedData1 [12]  ;
	uint8 arr_storedData2 [12] ;
	uint8 arr_storedData3 [12] ;
	/* Enable interrupts by setting I-bit */
	SREG  |= (1<<7);
	/* initialize UART */
	UART_ConfigType UART_Config ={EIGHT,DISABLED,ONE,9600};
	UART_init(&UART_Config);
	/* initialize BUZZER */
	BUZZER_init();
	/* initialize DC-MOTOR */
	DcMotor_init();
	/* Initialize EEPROM */
	EEPROM_init();

	TIMER_setCallBack(Timer0_Callbackfunc);


	while(1)
	{
		/* 3 variables used to represent arrays indexes while accessing them*/
		uint8 countArray = 0;
		uint8 countArray2 =0 ;
		uint8 countArray3 =0 ;
		switch(g_recievedValue) {
		case newPasswordFirst :
			/* every time after entering the case you should clear
			 * 1- the received value so the switch case is performed
			 * one time only
			 * 2- the flag because if you want to store the received
			 * value in this case ,clearing the flag will make
			 * sure you want store the initialize communication
			 * message */
			g_recievedValue=0;
			g_recievedFlag=0 ;
			while(g_recievedValue != finishedEntry ) {
				/* &&(g_recievedValue != finishedEntery : This statement
				 * is important to avoid storing (finishedEntery) if the
				 * g_recievedValue changed after checking the while
				 * condition  */
				if((g_recievedFlag ==1) &&(g_recievedValue != finishedEntry )) {
					arr_storedData1[countArray] =g_recievedValue ;
					countArray++ ;
					g_recievedFlag=0 ;
				}
			}
			arr_storedData1[countArray]=endOfPass ;
			break ;
		case newPasswordSecond :
			/* every time after entering the case you should clear
			 * 1- the received value so the switch case is performed
			 * one time only
			 * 2- the flag because if you want to store the received
			 * value in this case ,clearing the flag will make
			 * sure you want store the initialize communication
			 * message */
			g_recievedValue=0;
			g_recievedFlag=0 ;
			while(g_recievedValue != finishedEntry ) {
				/* &&(g_recievedValue != finishedEntery : This statement
				 * is important to avoid storing (finishedEntery) if the
				 * g_recievedValue changed after checking the while
				 * condition  */
				if((g_recievedFlag ==1) &&(g_recievedValue != finishedEntry )) {
					arr_storedData2[countArray2] =g_recievedValue ;
					countArray2++ ;
					g_recievedFlag=0 ;
				}
			}
			arr_storedData2[countArray2]=endOfPass ;
			if((compareTwoPasswords(arr_storedData1,arr_storedData2)) ==0) {
				storeNewPassword(arr_storedData1) ;
			}
			else {
				UART_sendByte(passwordsUnmatched) ;
			}
			break ;
		case uncheckedPassword :
			g_recievedFlag=0 ;
			g_recievedValue=0;
			while(g_recievedValue != finishedEntry ) {
				if((g_recievedFlag ==1) &&(g_recievedValue != finishedEntry )) {
					arr_storedData3[countArray3] =g_recievedValue ;
					countArray3++ ;
					g_recievedFlag=0 ;
				}
			}
			arr_storedData3[countArray3]=endOfPass ;
			checkPassword(arr_storedData3) ;
			break ;
		case openDoor :
			g_recievedValue=0;
			motor_on();
			break ;
		default:
			break;
		}

	}
	return 0;
}

void storeNewPassword (uint8* a_storedPassword_Ptr) {
	uint8 index=0 ;
	/* address of the eeprom */
	uint16 add=0x0302;
	while(a_storedPassword_Ptr[index] != endOfPass) {
		/* Write in the external EEPROM */
		EEPROM_writeByte(add, a_storedPassword_Ptr[index]);
		/* delay time as the eeprom take 10ms to make a write*/
		_delay_ms(100) ;
		add++ ;
		index++ ;
	}
	EEPROM_writeByte(add, endOfPass);
	UART_sendByte(returnMainOptions) ;
}
void checkPassword (uint8* a_checkPassword_Ptr) {
	uint8 index=0 ;
	uint16 add=0x0302 ;
	uint8 c[12] ;
	do{
		/* Read the pass from external EEPROM */
		EEPROM_readByte(add , c+index);
		/* delay time as the eeprom take 10ms to make a write*/
		_delay_ms(100) ;
		add++ ;
		index++;
	}
	while(c[index-1] != endOfPass) ;
	if(compareTwoPasswords(a_checkPassword_Ptr,c) == 0)
	{
		g_trialNumber=0 ;
		UART_sendByte(rightPassword) ;
	}
	else
	{
		g_trialNumber++ ;
		if (g_trialNumber ==3){
			UART_sendByte(errorScreen) ;
			buzzer_on();
		}
		else
		{
			UART_sendByte(wrongPassword) ;
		}
	}
}

uint8 compareTwoPasswords(uint8* a_passwordOne_Ptr, uint8* a_passwordTwo_Ptr) {
	uint8 numberOfDifference = 0;
	uint8 index=0;
	while(a_passwordOne_Ptr[index] != endOfPass)  {
		if (a_passwordOne_Ptr[index] == a_passwordTwo_Ptr[index]) {
		}
		else{
			numberOfDifference ++;
		}
		index++;
	}
	/*this statment check if they have the same length or not
	 * because if it wasn't there and the first password is 123
	 * and the seconend is 1234 the compare function would
	 * think they are identical while they are not*/
	if(a_passwordTwo_Ptr[index] == endOfPass) {

	}
	else{
		numberOfDifference ++;
	}
	return numberOfDifference ;
}

void buzzer_on(void)
{
	g_flag_buzzer = 0; /* Clear flag for next loop */
	BUZZER_start();
	TIMER_init(&Timer_Config);
	while(g_flag_buzzer != 1); /* wait till flag is set */
	g_flag_buzzer = 0; /* Clear flag for next loop */
	BUZZER_stop();
	UART_sendByte(returnMainOptions);
}


void motor_on(void)
{
	g_flag_motor = 0; /* Clear flag */
	UART_sendByte(doorOpening);
	DcMotor_Rotate(CW);
	TIMER_init(&Timer_Config);
	while(g_flag_motor != 1){}; /* wait till flag is set */
	g_flag_motor = 0; /* Clear flag for next loop */
	TIMER_deinit();


	DcMotor_Rotate(STOP);
	_delay_ms(3000);
	TIMER_init(&Timer_Config);
	while(g_flag_motor_stop != 1){} /* wait till flag is set */
	g_flag_motor_stop = 0; /* Clear flag for next loop */
	TIMER_deinit();

	UART_sendByte(doorClosing);
	DcMotor_Rotate(A_CW);
	TIMER_init(&Timer_Config);
	while(g_flag_motor != 1){}; /* wait till flag is set */
	g_flag_motor = 0; /* Clear flag for next loop */
	TIMER_deinit();
	DcMotor_Rotate(STOP);
	UART_sendByte(returnMainOptions);

}
void Timer0_Callbackfunc(void)
{
	/* We are using F_CPU = 8MHz and prescaler = 1024 so one second is 30 ticks */
	tick_buzzer ++ ;
	tick_motor ++ ;
	tick_motor_stop ++;
	if(tick_motor == TIMER_15_SECS) /* for counting 15 seconds */
	{
		g_flag_motor = 1;
		tick_motor = 0;
	}
	if(tick_motor_stop == TIMER_3_SECS) /* for counting 3 seconds */
	{
		g_flag_motor_stop = 1;
		tick_motor_stop = 0;
	}

	if(tick_buzzer == TIMER_60_SECS) /* for counting 60 seconds */
	{
		g_flag_buzzer = 1;
		tick_buzzer = 0;
	}
}

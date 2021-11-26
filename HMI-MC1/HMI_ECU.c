/******************************************************************************
 *
 * Module: Main Application for HMI-ECU
 *
 * File Name: HMI-ECU.c
 *
 * Description: Source file for the main application of HMI ECU
 *
 * Author: Peter Maged
 *
 *******************************************************************************/

#include "UART_messages.h"
#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include <util/delay.h>
#include "timer.h"

#define TIMER_3_SECS  92
#define TIMER_15_SECS 457
#define TIMER_60_SECS 1828

/************************************************************************
 *                           Global variables                           *
 ************************************************************************/
/* holds the UDR of UART whenever there's data in it */
volatile uint8 g_recievedValue=0;
/*Flag that is set to one whenever there's data in UDR */
volatile uint8 g_recievedFlag=0 ;
/*this variable holds the value of the number of entering password
 * weather it is the first entry or the seconend one (re -entering) */
uint8 g_entryCount = 0;
/*this is a flag that help the function askForPassword works until
 * the = sign is written */
uint8 g_passwordEntry=0 ;
/*holds the value of pressed key in the keypad */
uint8 g_key;
/*holds the value of pressed key - or * in the keypad to take action*/
uint8 g_keyOption;
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
 * [function Name] : App_askForPassword
 * [Description] : A function to ask the user about the previously-made
 * password
 */
void App_askForPassword(void) ;
/*
 * [function Name] : App_mainOptions
 * [Description] : A function to display the main options to user
 */
void App_mainOptions (void);
/*
 * [function Name] : App_newPassword
 * [Description] : A function to display the steps that is made to create
 * a new password
 */
void App_newPassword (void) ;
/*
 * [function Name] : Timer0_Callbackfunc
 * [Description] : A function to count seconds needed for motor and buzzer
 */
void Timer0_Callbackfunc(void);

/* initialize TIMER */
Timer_ConfigType Timer_Config = {F_CPU_1024,OVERFLOW,ENABLE,0,0};

int main(void)
{
	/* Enable interrupts by setting I-bit */
	SREG  |= (1<<7);
	/* initialize UART */
	UART_ConfigType UART_Config ={EIGHT,DISABLED,ONE,9600};
	UART_init(&UART_Config);
	/* initialize LCD */
	LCD_init();

	App_newPassword() ;
	TIMER_setCallBack(Timer0_Callbackfunc);

	while(1)
	{
		g_entryCount=0;
		g_passwordEntry=0;
		/* The program begins by switching the received data
		 * in the UDR to take action according to it */
		switch(g_recievedValue)
		{
		case passwordsUnmatched :
			/* must clear the g_recievedValue after every
			 * switch case executed so it will not execute again */
			g_recievedValue=0;
			LCD_clearScreen();
			LCD_displayString("Unmatched");
			_delay_ms(1000) ;
			App_newPassword() ;
			break ;

		case wrongPassword :
			g_recievedValue=0;
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			_delay_ms(1000) ;
			App_askForPassword() ;
			break ;

		case rightPassword :
			g_recievedValue=0;
			if(g_keyOption== '-'){
				/*Tell the other MC2 to open the door*/
				UART_sendByte(openDoor) ;
			}
			else if(g_keyOption=='+'){
				App_askForPassword() ;
				App_newPassword() ;
			}
			break;

		case errorScreen :
			g_recievedValue=0;
			LCD_clearScreen();
			LCD_displayString("ERROR!!!!!");
			TIMER_init(&Timer_Config);
			while(g_flag_buzzer != 1); /* wait till flag is set */
			g_flag_buzzer = 0; /* Clear flag for next loop */
			break ;

		case doorOpening :
			g_recievedValue=0;
			LCD_clearScreen();
			LCD_displayString("DOOR IS OPENING");
			TIMER_init(&Timer_Config);
			while(g_flag_motor != 1){}; /* wait till flag is set */
			g_flag_motor = 0; /* Clear flag for next loop */
			TIMER_deinit();

			LCD_clearScreen();
			LCD_displayString("DOOR IS OPEN");
			TIMER_init(&Timer_Config);
			_delay_ms(3000);
			while(g_flag_motor_stop != 1){} /* wait till flag is set */
			g_flag_motor_stop = 0; /* Clear flag for next loop */
			TIMER_deinit();
			break ;

		case doorClosing :
			g_recievedValue=0;
			LCD_clearScreen();
			LCD_displayString("DOOR IS Closing");
			break ;

		case returnMainOptions :
			g_recievedValue=0;
			App_mainOptions() ;
			break ;

		default:
			break;
		}
	}
	return 0;
}

void App_newPassword (void) {
	LCD_clearScreen();
	LCD_displayString("Enter New Password:");
	LCD_moveCursor(1,0);
	/*initiate communication with MC2 to save the first entry of
	 * the new password */
	UART_sendByte(newPasswordFirst) ;
	while(g_entryCount ==0) {
		g_key = KEYPAD_getPressedKey(); /* get the pressed key number */
		if ((g_key >=0 && g_key <=9 ))
		{
			UART_sendByte(g_key) ;
			LCD_displayString("*"); /* display the pressed keypad switch as * */
		}

		else if(g_key == '=')
		{
			/*Tell the other MC that u finished entry*/
			UART_sendByte(finishedEntry) ;
			/*tells MC1 that i finished first entry */
			g_entryCount++;
		}
		_delay_ms(1000);
	}
	LCD_clearScreen();
	LCD_displayString("Re-Enter Password:");
	LCD_moveCursor(1,0);
	/*initiate communication with MC2 to save the second enter of
	 * the new password */
	UART_sendByte(newPasswordSecond) ;
	while (g_entryCount ==1) {

		g_key = KEYPAD_getPressedKey();
		if ((g_key >=0 && g_key <=9 ))
		{
			UART_sendByte(g_key) ;
			/* display the pressed keypad switch as * */
			LCD_displayString("*");
		}

		else if(g_key == '=')
		{/*Tell the other MC that u finished entry*/
			UART_sendByte(finishedEntry) ;
			/*tells MC1 that i finished second entry */
			g_entryCount++;
		}
		_delay_ms(1000); /* Press time */
	}
}

void App_mainOptions (void){
	LCD_clearScreen();
	LCD_displayString("+ :change password");
	LCD_displayStringRowColumn(1,0,"- :open door") ;
	/* get the pressed key number */
	g_key = KEYPAD_getPressedKey();
	if (g_key =='+')
	{
		App_askForPassword() ;
		g_keyOption='+' ;
	}
	else if(g_key == '-')
	{
		App_askForPassword() ;
		g_keyOption='-' ;
	}
	/* Press time */
	_delay_ms(1000);
}

void App_askForPassword(void) {
	LCD_clearScreen();
	LCD_displayString("Enter Your Password:");
	LCD_moveCursor(1,0);
	/*initiate communication with MC2 to save the entered password and
	 * check if it's right or not */
	UART_sendByte(uncheckedPassword) ;
	while(g_passwordEntry ==0) {
		/* get the pressed key number */
		g_key = KEYPAD_getPressedKey();
		if ((g_key >=0 && g_key <=9 ))
		{
			UART_sendByte(g_key) ;
			/* display the pressed keypad switch as * */
			LCD_displayString("*");
		}

		else if(g_key == '=')
		{
			/*Tell the other MC that u finished entering
			 * password and the MC2 have to check if it's right
			 * */
			UART_sendByte(finishedEntry) ;
			/*tells MC1 that i finished writing password and
			 * should terminate the while loop */
			g_passwordEntry++;
		}
		/* Press time */
		/* if any switch pressed for more than 1000 ms it
		 * counts more than one press */
		_delay_ms(1000);
	}

}
void Timer0_Callbackfunc(void)
{
	/* We are using F_CPU = 8MHz and prescaler = 1024 so one second is 30 ticks */
	tick_buzzer ++ ;
	tick_motor ++ ;
	tick_motor_stop ++ ;
	if(tick_motor == TIMER_15_SECS) /* for counting 15 seconds */
	{
		g_flag_motor = 1;
		tick_motor = 0;
	}
	if(tick_motor_stop == TIMER_3_SECS)
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

 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Peter Maged
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


/************************************************************************
 *                           Global variables                           *
 ************************************************************************/


extern volatile uint8 g_recievedValue;
extern volatile uint8 g_recievedFlag ;

/*******************************************************************************
 *                      Types Declaration                                     *
 *******************************************************************************/
typedef enum
{
	FIVE,SIX,SEVEN,EIGHT
}Data_Bits;
typedef enum
{
	ONE,TWO
}Stop_Bits;
typedef enum
{
	DISABLED,EVEN=2,ODD
}Parity_Mode;

/******************************************************************************
Configuration structure for dynamic configuration of uart
like bit number,parity,stop bit and baud rate
 *******************************************************************************/
typedef struct
{
	Data_Bits data;
	Parity_Mode parity;
	Stop_Bits stop;
	uint32 baud_rate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */

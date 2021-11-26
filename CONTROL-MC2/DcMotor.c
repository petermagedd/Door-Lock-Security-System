/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: DcMotor.c
 *
 * Description: Source file for the DC Motor driver
 *
 * Author: Peter Maged
 *
 *******************************************************************************/

#include <avr/io.h>
#include "gpio.h"
#include "DcMotor.h"



void DcMotor_init(void){
	GPIO_setupPinDirection(MOTOR_INPUT_PORT_ID,MOTOR_INPUT1_PIN_ID	,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_INPUT_PORT_ID,MOTOR_INPUT2_PIN_ID	,PIN_OUTPUT);
	GPIO_writePin(MOTOR_INPUT_PORT_ID,MOTOR_INPUT1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_INPUT_PORT_ID,MOTOR_INPUT2_PIN_ID,LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state)
{
	switch (state)
	{
	case STOP:
		GPIO_writePin(MOTOR_INPUT_PORT_ID, MOTOR_INPUT1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_INPUT_PORT_ID, MOTOR_INPUT2_PIN_ID, LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(MOTOR_INPUT_PORT_ID, MOTOR_INPUT1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_INPUT_PORT_ID, MOTOR_INPUT2_PIN_ID, LOGIC_HIGH);
		break;
	case A_CW:
		GPIO_writePin(MOTOR_INPUT_PORT_ID, MOTOR_INPUT1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_INPUT_PORT_ID, MOTOR_INPUT2_PIN_ID, LOGIC_LOW);
		break;
	}
}

/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: DcMotor.h
 *
 * Description: Header file for the DC Motor driver
 *
 * Author: Peter Maged
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


#define MOTOR_INPUT_PORT_ID					PORTB_ID
#define MOTOR_INPUT1_PIN_ID					PIN6_ID
#define MOTOR_INPUT2_PIN_ID					PIN7_ID


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state);


#endif /* DCMOTOR_H_ */

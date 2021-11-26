/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the Buzzer driver
 *
 * Author: Peter Maged
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_PORT_ID          PORTA_ID
#define BUZZER_PIN_ID           PIN1_ID

void BUZZER_start();
void BUZZER_stop();
void BUZZER_init(void);

#endif /* BUZZER_H_ */

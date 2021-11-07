/*! @file : sensor_ultrasonico_dp1.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    6/11/2021
 * @brief   Driver para 
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "irq_lptmr0.h"
#include "pin_mux.h"
#include "board.h"
#include "sensor_ultrasonico_dp1.h"
#include "leds.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/
float sensor_1_ultrasonico;
float cont;
float distancia;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


void Sensorultrasonico_1_init(void){
	tiemposensorultrasonico = 0;
	GPIO_PinWrite(GPIOB,0,0);
	tiempocapturadato_echo = 0;
	cont = 0;

}

 float Sensorultrasonico_1_obtenerdato(void){
	 if(tiemposensorultrasonico==0){
		 GPIO_PinWrite(GPIOB,0,0);
	 }
	 if(tiemposensorultrasonico==90){
		 //encender_led_rojo();
		 cont=0;
		 GPIO_PinWrite(GPIOB,0,1);	//////pulso del trid
	 }
	 if(tiemposensorultrasonico==91){
		//apagar_led_rojo();
	 	GPIO_PinWrite(GPIOB,0,0);
	 	tiemposensorultrasonico=0;
	 }
	 if(GPIO_PinRead(GPIOB,1)!=0){
	 		 cont++;
	 }
	 if(tiempocapturadato_echo>150){
		 distancia = ((cont+1)*5)/4;
		 if(tiempocapturadato_echo==151){
			 tiempocapturadato_echo=0;
		 }
	 }
	return(distancia);
}

void Sensorultrasonico_1_Task_Run(void){
	  sensor_1_ultrasonico=Sensorultrasonico_1_obtenerdato();
}



/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 

/*! @file : sensor_ultrasonico_dp2.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    7/11/2021
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
#include "sensor_ultrasonico_dp2.h"
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
float sensor_2_ultrasonico;
float cont2;
float distancia2;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


void Sensorultrasonico_2_init(void){
	tiemposensorultrasonico2 = 0;
	GPIO_PinWrite(GPIOB,2,0);
	tiempocapturadato_echo2 = 0;
	cont2 = 0;

}

 float Sensorultrasonico_2_obtenerdato(void){
	 if(tiemposensorultrasonico2==0){
		 GPIO_PinWrite(GPIOB,2,0);
	 }
	 if(tiemposensorultrasonico2==90){
		 //encender_led_rojo();
		 cont2=0;
		 GPIO_PinWrite(GPIOB,2,1);	//////pulso del trid
	 }
	 if(tiemposensorultrasonico2==91){
		//apagar_led_rojo();
	 	GPIO_PinWrite(GPIOB,2,0);
	 	tiemposensorultrasonico2=0;
	 }
	 if(GPIO_PinRead(GPIOB,3)!=0){
	 		 cont2++;
	 }
	 if(tiempocapturadato_echo2>150){
		 distancia2 = ((cont2+1)*5)/4;
		 if(tiempocapturadato_echo2==151){
			 tiempocapturadato_echo2=0;
		 }
	 }
	return(distancia2);
}




void Sensorultrasonico_2_Task_Run(void){
	  sensor_2_ultrasonico=Sensorultrasonico_2_obtenerdato();
}

/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 

/*! @file : cronometro.c
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
#include "cronometro.h"
#include "irq_lptmr0.h"
#include "pin_mux.h"
#include "board.h"
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
uint32_t segundos;
uint32_t minutos=0;
uint32_t horas=0;
uint32_t segundos_destilacion;
uint32_t minutos_destilacion=0;
uint32_t horas_destilacion=0;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/

void tiempo_fermentacion(void){
	  if(GPIO_PinRead(GPIOC,1)==0){
		   		tiempofermentacion=0;
		   		minutos=0;
		   		horas=0;
	  	}else{
		    	segundos=tiempofermentacion*0.001;
		   		if(segundos==60){
		   			minutos++;
		   			if(minutos==60){
		   				horas++;
		   				minutos=0;
		   				segundos=0;
		   			}
		   			segundos=0;
		   			tiempofermentacion=0;
		   		}
		   	}
}


void tiempo_destilacion(void){
	if(GPIO_PinRead(GPIOE,1)==0){
		tiempodestilacion=0;
	   	minutos_destilacion=0;
	   	horas_destilacion=0;
	   	encender_led_verde();

	}else{
	  	segundos_destilacion=tiempodestilacion*0.001;
		if(segundos_destilacion==60){
		   minutos_destilacion++;
		   if(minutos_destilacion==60){
		   		horas_destilacion++;
		   		minutos_destilacion=0;
		   		segundos_destilacion=0;
		   	}
		   	segundos_destilacion=0;
		   	tiempodestilacion=0;
		 }
		 if(minutos_destilacion==1){
		   	apagar_led_verde();
		 }
	 }
}
/*******************************************************************************
 * Public Source Code
 ******************************************************************************/

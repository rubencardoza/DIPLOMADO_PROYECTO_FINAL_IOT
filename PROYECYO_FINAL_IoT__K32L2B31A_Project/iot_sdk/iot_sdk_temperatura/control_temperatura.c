/*! @file : control_temperatura.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    28/11/2021
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
#include "control_temperatura.h"
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
volatile float sensor_temperatura;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
int j = 0 ;
int i = 0 ;

void Sensor_temperatura_init(void)
{
   GPIO_PinWrite(GPIOD,7,0); // SCK
   GPIO_PinWrite(GPIOE,0,1); // CS
}

void delay_ms_block(){
	uint32_t i;
	for(i=0;i<0x1063;i++);
}

void delay_us_block(){
	uint32_t j;
	for(j=0;j<0x5;j++);
}

void delay_200ms_block(){
	uint32_t l;
	for(l=0;l<0xCCCCD;l++);
}

 


uint16_t read_sensor_temperatura(void){
	int k = 0 ;
	uint16_t buffer[16];
	float temperatura_decimal ;
	uint16_t temperatura_entero ;
	float temperatura_max6675 ;
	uint16_t dato_12, dato_11, dato_10, dato_9, dato_8, dato_7, dato_6, dato_5, dato_4, dato_3, dato_decima_2, dato_decima_1;
	//uint16_t datos_temperatura ;
	GPIO_PinWrite(GPIOE,0,0); // CS
	delay_ms_block();
	for (k=0;k<16;k++){
		//shift_left(&data,2,GPIO_PinRead(GPIOE,1));      //reads in 2 bytes to data from the pin TC_DATA
		buffer[k] = GPIO_PinRead(GPIOE,1);
		//datos_temperatura = buffer[k];
		GPIO_PinWrite(GPIOD,7,1); // SCK;
		delay_us_block();
		//delay_us(1); //Descomentar si usa crystal mayor a 10MHz
		GPIO_PinWrite(GPIOD,7,0); // SCK;
		delay_us_block();
		//delay_us(1);
		apagar_led_verde();
		//printf("The %d is wrong! data is %d\n\r", k, buffer[k]);
		if(k==15){
			encender_led_verde();
		}
	}
	delay_ms_block();
	//printf("%d%d%d%d%d%d%d%d%d%d%d%d\n\r",buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11],buffer[12]);
	GPIO_PinWrite(GPIOE,0,1); // CS
	delay_ms_block();
	dato_12 = 512 * buffer[1] ;
	dato_11 = 256 * buffer[2] ;
	dato_10 = 128 * buffer[3] ;
	dato_9 = 64 * buffer[4] ;
	dato_8 = 32 * buffer[5] ;
	dato_7 = 16 * buffer[6] ;
	dato_6 = 8 * buffer[7] ;
	dato_5 = 4 * buffer[8] ;
	dato_4 = 2 * buffer[9] ;
	dato_3 = 1 * buffer[10] ;
	dato_decima_2 = 2 * buffer[11] ;
	dato_decima_1 = 1 * buffer[12] ;
	temperatura_entero = dato_12 + dato_11 + dato_10 + dato_9 + dato_8 + dato_7 + dato_6 + dato_5 + dato_4 + dato_3;
	temperatura_decimal = (dato_decima_2 + dato_decima_1)*0.1 ;
	temperatura_max6675 = temperatura_entero + temperatura_decimal ;
	//printf("%d , %0.2f , %0.2f\n\r",temperatura_entero, temperatura_decimal, temperatura_max6675);
  return(temperatura_max6675);
}




float do_everything(void){
	Sensor_temperatura_init();
    delay_200ms_block(); //200ms is a long time to be doing nothing. use a timer interrupt to avoid wasting time here
   return(read_sensor_temperatura());
}


void Sensor_temperatura_Task_Run(void){
	  sensor_temperatura = do_everything();
	  //printf("%0.2f\r\n",sensor_temperatura);
}

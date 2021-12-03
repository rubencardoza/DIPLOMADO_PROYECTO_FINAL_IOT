/*! @file : sensor_hk1100c.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    10/11/2021
 * @brief   Driver para 
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sensor_hk1100c.h"
#include <stdio.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
 /*!
  * @brief Inicia captura por ADC de voltaje generado por sensor de luz
  *
  */
 void sensorDePresionIniciarCaptura(void);
 /*!
  * @brief Espera a que finalice el trabajo del ADC
  *
  */
 void sensorDePresionEsperarResultado(void);

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/
 uint32_t adc_sensor_de_presion;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
 void sensorDePresionIniciarCaptura(void){
 	ADC16_SetChannelConfig(SENSOR_DE_PRESION_ADC16_BASE,SENSOR_DE_PRESION_ADC16_CHANNEL_GROUP,&ADC0_channelsConfig[0]);
 }
 /*---------------------------------------------------------------*/
 void sensorDePresionEsperarResultado(void){
 	while (0U == (kADC16_ChannelConversionDoneFlag
 					& ADC16_GetChannelStatusFlags(
 							SENSOR_DE_PRESION_ADC16_BASE,
 							SENSOR_DE_PRESION_ADC16_CHANNEL_GROUP)))
 	{
 	}
 }
/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
uint32_t sensorDePresionObtenerdatoADC(void){
 uint32_t resultadoADC;

 sensorDePresionIniciarCaptura();
 sensorDePresionEsperarResultado();

 resultadoADC=ADC16_GetChannelConversionValue(
		 	 	 SENSOR_DE_PRESION_ADC16_BASE,
				 SENSOR_DE_PRESION_ADC16_CHANNEL_GROUP);

 return(resultadoADC);
}
/*---------------------------------------------------------------*/

void SensorPresion_Init(void){

}

void SensorPresion_Task_Run(void){
	  adc_sensor_de_presion=sensorDePresionObtenerdatoADC();
	  printf("%d\r\n",adc_sensor_de_presion);
}
/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 

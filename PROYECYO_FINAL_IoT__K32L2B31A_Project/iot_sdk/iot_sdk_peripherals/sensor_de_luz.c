/*! @file : sensor_de_luz.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    10/09/2021
 * @brief   Driver para 
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sensor_de_luz.h"

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
 void sensorDeLuzIniciarCaptura(void);
 /*!
  * @brief Espera a que finalice el trabajo del ADC
  *
  */
 void sensorDeLuzEsperarResultado(void);

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/
 uint32_t adc_sensor_de_luz;


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
 void sensorDeLuzIniciarCaptura(void){
 	ADC16_SetChannelConfig(
 			SENSOR_DE_LUZ_ADC16_BASE,
 			SENSOR_DE_LUZ_ADC16_CHANNEL_GROUP,
 			&ADC0_channelsConfig[0]);
 }
 /*---------------------------------------------------------------*/
 void sensorDeLuzEsperarResultado(void){
 	while (0U == (kADC16_ChannelConversionDoneFlag
 					& ADC16_GetChannelStatusFlags(
 							SENSOR_DE_LUZ_ADC16_BASE,
 							SENSOR_DE_LUZ_ADC16_CHANNEL_GROUP)))
 	{
 	}
 }
/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
uint32_t sensorDeLuzObtenerdatoADC(void){
 uint32_t resultadoADC;

 sensorDeLuzIniciarCaptura();
 sensorDeLuzEsperarResultado();

 resultadoADC=ADC16_GetChannelConversionValue(
		 	 	 SENSOR_DE_LUZ_ADC16_BASE,
				 SENSOR_DE_LUZ_ADC16_CHANNEL_GROUP);

 return(resultadoADC);
}
/*---------------------------------------------------------------*/

void SensorLuz_Init(void){

}

void SensorLuz_Task_Run(void){
	  adc_sensor_de_luz=sensorDeLuzObtenerdatoADC();
}

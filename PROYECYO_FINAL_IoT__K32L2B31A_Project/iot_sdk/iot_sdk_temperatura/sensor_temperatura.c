/*! @file : sensor_tem.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    7/09/2021
 * @brief   Driver para sensor de temperatura
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include"sensor_temperatura.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
void sensorDetemperaturaIniciarCaptura(void);
void sensorDetemperaturaEsperarResultado(void);

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void sensorDetemperaturaIniciarCaptura(){

 	 ADC16_SetChannelConfig(SENSOR_DE_temperatura_ADC16_BASE, SENSOR_DE_temperatura_ADC16_CHANNEL_GROUP, &ADC0_channelsConfig[1]);

  }
  void sensorDetemperaturaEsperarResultado(){
 	 while (0U == (kADC16_ChannelConversionDoneFlag &
 	                       ADC16_GetChannelStatusFlags(SENSOR_DE_temperatura_ADC16_BASE, SENSOR_DE_temperatura_ADC16_CHANNEL_GROUP)))
 	         {
 	         }
  }

/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 float SensorDetemperaturaObtenerDatosADC(){
	 float resultadoADC;
	 float temp25=0.716;


		 sensorDetemperaturaIniciarCaptura();
		         sensorDetemperaturaEsperarResultado();
		 resultadoADC=ADC16_GetChannelConversionValue(SENSOR_DE_temperatura_ADC16_BASE, SENSOR_DE_temperatura_ADC16_CHANNEL_GROUP);



				resultadoADC=resultadoADC*0.000732421;

				 if(resultadoADC>temp25){
					 resultadoADC=25-((resultadoADC-temp25)/1.646);
				  }

				 if(resultadoADC<0.7012){
							 resultadoADC=25-((resultadoADC-temp25)/1.769);
				 	}


		 return(resultadoADC);

 }


/*! @file : sensor_tem.h
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    7/09/2021
 * @brief   Driver para 
 * @details
 *
 */
#ifndef IOT_SDK_TEMP_SENSOR_TEM_H_
#define IOT_SDK_TEMP_SENSOR_TEM_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include"peripherals.h"

/*!
 * @addtogroup X
 * @{
 */
/*!
 * @addtogroup X
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/
#define SENSOR_DE_temperatura_ADC16_BASE          ADC0
#define SENSOR_DE_temperatura_ADC16_CHANNEL_GROUP 0U
#define SENSOR_DE_temperatura_ADC16_USER_CHANNEL  26U /* PTE22, ADC0_SE3 */
/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
void sensorDetemperaturaIniciarCaptura(void);
void sensorDetemperaturaEsperarResultado(void);
 float SensorDetemperaturaObtenerDatosADC(void);
/** @} */ // end of X group
/** @} */ // end of X group

#endif /* IOT_SDK_TEMP_SENSOR_TEM_H_ */

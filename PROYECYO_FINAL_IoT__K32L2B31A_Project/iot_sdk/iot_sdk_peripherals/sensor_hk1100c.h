/*! @file : sensor_hk1100c.h
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    10/11/2021
 * @brief   Driver para 
 * @details
 *
 */
#ifndef IOT_SDK_PERIPHERALS_SENSOR_HK1100C_H_
#define IOT_SDK_PERIPHERALS_SENSOR_HK1100C_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "peripherals.h"

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

#define SENSOR_DE_PRESION_ADC16_BASE          ADC0
#define SENSOR_DE_PRESION_ADC16_CHANNEL_GROUP 0U
#define SENSOR_DE_PRESION_ADC16_USER_CHANNEL  14U /* PTE22, ADC0_SE3 */

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/

/*!
  * @brief Retorna del resultado de la conversión ADC para sensor de luz
  *
  */
 uint32_t sensorDeLuzObtenerdatoADC(void);

// inicializa vables del modulo
 void SensorPresion_Init(void);
 void SensorPresion_Task_Run(void);

/** @} */ // end of X group
/** @} */ // end of X group

#endif /* IOT_SDK_PERIPHERALS_SENSOR_HK1100C_H_ */

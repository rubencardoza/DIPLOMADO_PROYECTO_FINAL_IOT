/*! @file : botones.h
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    11/09/2021
 * @brief   Driver para botones de la tarjeta FRDM-K32L2B3
 * @details
 *
 */
#ifndef IOT_SDK_PERIPHERALS_BOTONES_H_
#define IOT_SDK_PERIPHERALS_BOTONES_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "peripherals.h"
#include "fsl_gpio.h"

/*!
 * @addtogroup PERIPHERALS
 * @{
 */
/*!
 * @addtogroup BOTONES
 * @{
 */
/*******************************************************************************
 * Public Definitions
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/

bool boton1leerestado();
bool boton2leerestado();
/** @} */ // end of BOTONES group
/** @} */ // end of PERIPHERALS group

#endif /* IOT_SDK_PERIPHERALS_BOTONES_H_ */

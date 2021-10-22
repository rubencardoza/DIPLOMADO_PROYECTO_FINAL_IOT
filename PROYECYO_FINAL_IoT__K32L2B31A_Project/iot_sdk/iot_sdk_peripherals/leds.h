/*! @file : leds.h
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    9/09/2021
 * @brief   Driver para controlar LEDs FRDM-K32L2B3
 * @details
 *
 */
#ifndef IOT_SDK_PERIPHERALS_LEDS_H_
#define IOT_SDK_PERIPHERALS_LEDS_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/


/*!
 * @addtogroup PERIPHERALS
 * @{
 */
/*!
 * @addtogroup LEDS
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
/*
 * @brief enciende led verde y rojo de tarjeta
 */

void led_on_green();
void led_on_red();

/*
 * @brief apaga led verdey rojo  de  tarjeta
 */

void led_off_green();
void led_off_red();

/** @} */ // end of LEDS
/** @} */ // end of PERIPHERALS

#endif /* IOT_SDK_PERIPHERALS_LEDS_H_ */

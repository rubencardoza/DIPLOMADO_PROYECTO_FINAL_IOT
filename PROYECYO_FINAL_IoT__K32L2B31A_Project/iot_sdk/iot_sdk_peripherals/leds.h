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
/*!
 * @brief enciende led verde de tarjeta FRDM-K32L2B3
 *
 */
 void encender_led_verde();
 void encender_led_rojo();
 /*---------------------------------------------------------------*/
 /*!
  * @brief apaga led verde de tarjeta FRDM-K32L2B3
  *
  */
 void apagar_led_verde();
 void apagar_led_rojo();
 /*!
  * @brief invierte estado de led rojo de tarjeta FRDM-K32L2B3
  *
  */
 void toggle_led_rojo();

 void toggle_led_verde();

/** @} */ // end of X LEDS
/** @} */ // end of X PERIPHERALS

#endif /* IOT_SDK_PERIPHERALS_LEDS_H_ */

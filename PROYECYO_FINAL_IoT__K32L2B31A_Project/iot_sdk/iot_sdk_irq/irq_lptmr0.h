/*! @file : irq_lptmr0.h
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    11/09/2021
 * @brief   Driver para 
 * @details
 *
 */
#ifndef IOT_SDK_IRQ_IRQ_LPTMR0_H_
#define IOT_SDK_IRQ_IRQ_LPTMR0_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include"peripherals.h"

/*!
 * @addtogroup IRQ
 * @{
 */
/*!
 * @addtogroup LPRMR0
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
extern volatile uint32_t lptmr0_irq_counter;
extern volatile uint32_t led_rojo_irq_counter;
extern volatile uint32_t boton1_presionado;
extern volatile uint32_t boton2_presionado;
/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/

/** @} */ // end of X LPTMR0
/** @} */ // end of X IRQ

#endif /* IOT_SDK_IRQ_IRQ_LPTMR0_H_ */

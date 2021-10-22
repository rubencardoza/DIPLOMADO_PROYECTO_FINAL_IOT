/*! @file : irq_lptmr0.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    11/09/2021
 * @brief   Driver para 
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "irq_lptmr0.h"

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
 volatile uint32_t lptmr0_irq_counter=0;

 volatile uint32_t boton1_presionado=0;
 volatile uint32_t boton2_presionado=0;
/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
/* LPTMR0_IRQn interrupt handler */
void LPTMR0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = LPTMR_GetStatusFlags(LPTMR0_PERIPHERAL);
  LPTMR_ClearStatusFlags(LPTMR0_PERIPHERAL, intStatus);

  /* Place your code here */

   lptmr0_irq_counter++;

   /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/


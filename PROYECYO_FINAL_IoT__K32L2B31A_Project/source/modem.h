/*! @file : modem.h
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    21/10/2021
 * @brief   Driver para 
 * @details
 *
 */
#ifndef MODEM_H_
#define MODEM_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/


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

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Public vars
 ******************************************************************************/

/*******************************************************************************
 * Public Prototypes
 ******************************************************************************/
#define TIME_WAIT_RTA_CMD 2000 //[=] Segundos que espera una Respuesta del Modem
//despues de enviar un comando

#define Modem_Send_Cmd(comandoStr) printf(comandoStr)

// Incicializa variables y estado inicial del Modem
// configura y inicia proceso de enviar un primer mensaje
void Modem_Init(void);



// Funcion de estado estable
void Modem_Task_Run(void);

void Key_Task_Run(void);

/** @} */ // end of X group
/** @} */ // end of X group

#endif /* MODEM_H_ */

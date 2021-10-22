/*! @file : irq_lpuart0.c
 * @author  Luis Carlos Nigrinis Alvarez
 * @version 1.0.0
 * @date    12/09/2021
 * @brief   Driver para 
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "irq_lpuart0.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
void pushNuevoByteEnBufferCircular(uint8_t nuevo_byte);

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/
volatile uint8_t 	rx_ring_buffer[RX_RING_BUFFER_SIZE];
volatile uint32_t	rx_index_productor=0;
volatile uint32_t	rx_index_consumidor=0;
/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/* LPUART0_IRQn interrupt handler */
void LPUART0_SERIAL_RX_TX_IRQHANDLER(void) {
 uint32_t intStatus;
 uint8_t nuevo_dato_uart;

 /* Obtiene status de banderas IRQ por LPUART0,
  * esto implica borrar banderas IRQ */
 intStatus = LPUART_GetStatusFlags(LPUART0_PERIPHERAL);

 if ((kLPUART_RxDataRegFullFlag)&intStatus)
 {
	  nuevo_dato_uart = LPUART_ReadByte(LPUART0);		//Captura dato recibido
	  pushNuevoByteEnBufferCircular(nuevo_dato_uart);	//Intenta almacenar nuevo byte en buffer circular
 }

 /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
    Store immediate overlapping exception return operation might vector to incorrect interrupt. */
 #if defined __CORTEX_M && (__CORTEX_M == 4U)
   __DSB();
 #endif
}

void pushNuevoByteEnBufferCircular(uint8_t nuevo_byte){
    //Verifica que exista espacio de memoria disponible para almacenar nueva informacion
	  //incluye corrección del buffer circular
    if (((rx_index_productor + 1) % RX_RING_BUFFER_SIZE) != rx_index_consumidor)
    {
  	  //Si tiene espacio en buffer disponible, almacena nuevo byte
  	  rx_ring_buffer[rx_index_productor] = nuevo_byte;
        //incrementa apuntador de memoria del productor de información
        rx_index_productor++;
        //realiza ajuste de buffer circular para el apuntador
        rx_index_productor %= RX_RING_BUFFER_SIZE;
    }
}
/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
uint8_t pullByteDesdeBufferCircular(void){
	uint8_t byte_a_retornar;
	//Pregunta si existe byte disponible en buffer
    if (rx_index_productor != rx_index_consumidor)
    {	//Saca byte desde buffer
    	byte_a_retornar=rx_ring_buffer[rx_index_consumidor];
    	//Incrementa apuntador de memoria del consumidor de información
    	rx_index_consumidor++;
    	//realiza ajuste de buffer circular para el apuntador
    	rx_index_consumidor %= RX_RING_BUFFER_SIZE;
    }
    return(byte_a_retornar);
}

int32_t numeroDeBytesDisponiblesEnBufferRx(void){
	return((int32_t)(rx_index_productor-rx_index_consumidor));
}




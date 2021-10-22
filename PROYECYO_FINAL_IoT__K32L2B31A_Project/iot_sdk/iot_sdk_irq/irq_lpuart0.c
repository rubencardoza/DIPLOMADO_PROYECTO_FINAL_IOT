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
void pushNuevoByteEnBuefferCircular(uint8_t nuevo_byte);

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/

volatile uint8_t rx_ring_buffer[RX_RING_BUFFER_SIZE];

volatile uint32_t rx_index_productor=0;
volatile uint32_t rx_index_consumidor=0;

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
void pushNuevoByteEnBuefferCircular(uint8_t nuevo_byte){
	  //verifica que exista espacio de memoria disponible para almacenar nueva informacion
	  //incluye correccion del buffer circular
	  if (((rx_index_productor + 1) % RX_RING_BUFFER_SIZE) != rx_index_consumidor)
	  {

		  //si tiene espacio disponible almacena nuevo byte
		  rx_ring_buffer[rx_index_productor] = nuevo_byte;
		  //incrementa apuntador de memoria del productor de informacion
		  rx_index_productor++;
		  //realiza ajuste de buffer circular para el apuntador
		  rx_index_productor %= RX_RING_BUFFER_SIZE;
	  }

}




/* LPUART0_IRQn interrupt handler */
void LPUART0_SERIAL_RX_TX_IRQHANDLER(void) {
	uint32_t intStatus;
	uint8_t nuevo_dato_uart;

  /* obtiene estado de bandera IRQ y borra banderas IRQ*/
  intStatus = LPUART_GetStatusFlags(LPUART0_PERIPHERAL);

    /* If new data arrived. */
  if ((kLPUART_RxDataRegFullFlag)&intStatus)
  {

	  nuevo_dato_uart= LPUART_ReadByte(LPUART0);        //captura dato recibido
	  pushNuevoByteEnBuefferCircular(nuevo_dato_uart);  //intenta almacenar nuevo byte en buffer circular




  }

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
uint8_t pullBytedesdeBuefferCircular(){
	uint8_t byte_a_retornar=0;
	//pregunta si existe byte disponible
		if (rx_index_productor != rx_index_consumidor)
		 {	//saca byte desde buffer
			byte_a_retornar=rx_ring_buffer[rx_index_consumidor];
			//incrementa apuntador de memoria del consumidor de informacion
			rx_index_consumidor++;
			//realiza ajuste de buffer circular para el apuntador
			rx_index_consumidor %= RX_RING_BUFFER_SIZE;
		 }
	return(byte_a_retornar);
}

int32_t NumeroDeByteDisponiblesEnBufferRx(){
	return((int32_t)(rx_index_productor-rx_index_consumidor));
}



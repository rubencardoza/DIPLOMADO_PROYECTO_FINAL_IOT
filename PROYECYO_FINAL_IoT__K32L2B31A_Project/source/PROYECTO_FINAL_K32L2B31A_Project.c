/*! @file : K32L2B31A_Project.c
 * @author  Luis Carlos Niginis Alvarez
 * @version 0.0.000
 * @date    23/08/2021
 * @brief   Funcion principal main
 * @details
 *			v0.0.000	Proyecto base creado usando MCUXpresso
 *
 *
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
#include "fsl_debug_console.h"
#include "stdbool.h"

#include "leds.h"

#include "botones.h"
#include "modem.h"
#include "irq_lptmr0.h"
#include "irq_lpuart0.h"
#include "sensor_ultrasonico_dp1.h"

#include "cronometro.h"
#include "control_temperatura.h"

#include "sensor_hk1100c.h"

#include "fsl_spi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#define EXAMPLE_SPI_MASTER              SPI0
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ     CLOCK_GetFreq(kCLOCK_BusClk)

#define BUFFER_SIZE (2)
static uint8_t srcBuff[BUFFER_SIZE];
static uint16_t destBuff[BUFFER_SIZE];

// Para modulo Alarma
volatile uint32_t segAct=0;
volatile uint32_t tiempoAct=0;

// Inicializa Alarma
void Alarma_Init(void){
	segAct = lptmr0_irq_counter;
}

// retorna el tiempo futuro de la alarma
uint32_t Alarma_Set(uint32_t time2Wait){
	segAct = lptmr0_irq_counter;
	return (segAct + time2Wait);
}

// retorna si la alarma se cumplio
char Alarma_Elapsed(uint32_t time2Test){
	segAct = lptmr0_irq_counter;
	if(segAct >= time2Test) return true;
	else return false;
}

#define Timer_Init() LPTMR_StartTimer(LPTMR0)

/* Init board hardware. */
void BOARD_Init(void){
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
}



// Scratch

	// 1. crear codigo nuevo
	// 2. Se compilar
	// 3. Se prueba en el board
	// 4. Se depura
	// 5.Funciona??? NO --> va al punto 1.
	// Funciona??? SI ---> punto 6.
	// 6. Backup Version
	// 7.Modularizar el modulo que Funciono
	// 8. Ensayar el Funcionamiento ya modularizado
	// 9. Comentar y organizar
	// 10.Optimizar codigo si es necesario
	// 11. Clean y Build
	// 12. Liberar version V2.0

//end Scratf

//  main
int main(void) {
	BOARD_Init();
    Timer_Init();
    Modem_Init();
    Key_Task_Init();
    Alarma_Init();

    Sensorultrasonico_1_init();
	SensorPresion_Init();

	///////////////////////////////////////////////////////////////////////////////////////////
    spi_master_config_t userConfig = {0};
    uint32_t srcFreq               = 0;
    //uint32_t i                     = 0;
    //uint32_t err                   = 0;
    spi_transfer_t xfer            = {0};

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    PRINTF("\n\rMaster Start...\n\r");
    /*
     * userConfig.enableStopInWaitMode = false;
     * userConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig.phase = kSPI_ClockPhaseFirstEdge;
     * userConfig.direction = kSPI_MsbFirst;
     * userConfig.dataMode = kSPI_8BitMode;
     * userConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * userConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     * userConfig.pinMode = kSPI_PinModeNormal;
     * userConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
     * userConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&userConfig);
    userConfig.dataMode = kSPI_16BitMode;
    userConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
    userConfig.direction = kSPI_MsbFirst;
    userConfig.pinMode = kSPI_PinModeNormal;

    srcFreq = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &userConfig, srcFreq);

	//////////////////////////////////////////////////////////////////////////////////////////
    uint16_t dato_original;
    uint16_t dato_entero;
    uint16_t dato_final_entero;
    uint16_t dato_final_decimal;
    uint16_t dato_decimal ;
    float temperatura_grados ;


    while(1) { // multiTaks de tareas por Polling
    	//Sensorultrasonico_1_Task_Run();
    	//Modem_Task_Run();
    	//tiempo_fermentacion();
    	//Sensor_temperatura_Task_Run();
    	//Sensortemperatura_Task_Run();
    	//SensorPresion_Task_Run();



    	if(tiempocapturadato_echo==1000){
    		 /*Start Transfer*/
    		xfer.txData   = srcBuff;
    		xfer.rxData   = destBuff;
    		xfer.dataSize = BUFFER_SIZE;
    		SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);
    		encender_led_rojo();
    	}
    	if(tiempocapturadato_echo > 2000){
    		apagar_led_rojo();
    		tiempocapturadato_echo = 0;
    		dato_original = destBuff[0] ;
    		dato_decimal = dato_original & 0x32;
    		dato_final_decimal  = dato_decimal>>3;
    		dato_entero = dato_original & 0b0000111111100000;
    		dato_final_entero = dato_entero>>5;
    		temperatura_grados = dato_final_entero + dato_final_decimal*0.1;
    		//printf("16_bits---%d\r\n",dato_original);
    		printf("%0.2f\r\n",temperatura_grados);
    	}


    }
    return 0 ;
}


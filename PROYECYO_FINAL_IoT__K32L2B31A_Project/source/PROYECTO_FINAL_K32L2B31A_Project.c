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
#include "sensor_de_luz.h"
#include "botones.h"
#include "modem.h"
#include "irq_lptmr0.h"
#include "irq_lpuart0.h"
#include "sensor_ultrasonico_dp1.h"
#include "sensor_ultrasonico_dp2.h"
#include "cronometro.h"
#include "sensor_temperatura.h"
#include "sensor_hk1100c.h"



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
    SensorLuz_Init();
    Alarma_Init();
    Sensorultrasonico_1_init();
    Sensorultrasonico_2_init();
    sensorDetemperaturaIniciarCaptura();
	SensorPresion_Init();


    while(1) { // multiTaks de tareas por Polling
    	Modem_Task_Run();
    	SensorLuz_Task_Run();
    	Sensorultrasonico_1_Task_Run();
    	Sensorultrasonico_2_Task_Run();
    	tiempo_fermentacion();
    	Sensortemperatura_Task_Run();
    	SensorPresion_Task_Run();

    }
    return 0 ;
}


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

#include "leds.h"
#include "sensor_de_luz.h"
#include "botones.h"
#include "sensor_temperatura.h"

#include "irq_lptmr0.h"
#include "irq_lpuart0.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define SIZE_BUFFER_COMANDOS  50

#define COMANDOS_DISPONIBLES 6
#define COMANDOS_DISPONIBLES_EC25 24
#define COMANDOS_RESPUESTA_OK 1



/*******************************************************************************
 * Local vars
 ******************************************************************************/

uint32_t index_buffer_nuevo_comando_recibido=0;
uint8_t nuevo_byte_lpuart0;
char buffer_nuevo_comando_recibido[50];
uint8_t i=0;

/******************************/
enum _lista_comandos_controll{
	GREEN_ON = 0,
	GREEN_OFF,
	RED_ON,
	RED_OFF,
	SENSOR_TEMPERATURA,
	SENSOR_LUZ,

};

const char *comandos_control[] ={
		"GREEN_ON",  //encender led VERDE
		"GREEN_OFF",  //apagar led VERDE
		"RED_ON",     //encender led ROJO
		"RED_OFF",    //apagar led ROJO
		"SENSOR_TEMPERATURA",    //Imprimir datos del sensor de temperatura
		"SENSOR_LUZ",    //No imprime datos del sensor

};

/*******************************/
enum lista_ec25_comandos_at{
	informacion_del_producto=0,
	identificacion_del_producto,
	identificacion_del_modelo_de_TA,
	identificacion_de_revision_versiondesoftware,
	Solicitar_identificacion_del_fabricante,
	Solicitar_identificacion_de_modelo,
	Solicitar_TA_de_la_version_d_esoftware,
	Solicitar_identidad_del_equipo_movil_IMEI,
	Identificacion_numero_de_serie_del_producto,
	Establecer_valores_predeter_del_fabricante,
	Mostrar_configuracion_actual,
	Almacenar_parametros_actuales,
	Establecer_todos_los_parametros_actuales,
	Establecer_el_modo_de_presentacion,
	Establecer_modo_de_eco_de_comando,
	Repetir_linea_de_comando_anterior,
	Establecer_determinacion_de_la_linea_de_comandos,
	Establecer_caracter_de_formato_de_respuesta,
	Establecer_caracter_de_edicion_delineade_comando,
	Resultado_de_CONNECT_monitore0_progreso_llamada,
	Configurar_la_funcionalidad_del_telefono,
	Formato_del_mensaje_de_error,
	Seleccione_el_juego_de_caracteres_TE,
	Configurar_la_opcion_de_indicacion_URC,

};


const char *ec25_comandos_at[]={
		"informacion_del_producto",  //CONSULTA INFORMACION EL MODEN
		"identificacion_del_producto",
		"identificacion_del_modelo_de_TA",
		"identificacion_de_revision_versiondesoftware",
		"Solicitar_identificacion_del_fabricante",
		"Solicitar_identificacion_de_modelo",
		"Solicitar_TA_de_la_version_d_esoftware",
		"Solicitar_identidad_del_equipo_movil_IMEI",
		"Identificacion_numero_de_serie_del_producto",
		"Establecer_valores_predeter_del_fabricante",
		"Mostrar_configuracion_actual",
		"Almacenar_parametros_actuales",
		"Establecer_todos_los_parametros_actuales",
		"Establecer_el_modo_de_presentacion",

		"Establecer_modo_de_eco_de_comando",
		"Repetir_linea_de_comando_anterior",
		"Establecer_determinacion_de_la_linea_de_comandos",
		"Establecer_caracter_de_formato_de_respuesta",
		"Establecer_caracter_de_edicion_delineade_comando",
		"Resultado_de_CONNECT_monitore0_progreso_llamada",
		"Configurar_la_funcionalidad_del_telefono",
		"Formato_del_mensaje_de_error",
		"Seleccione_el_juego_de_caracteres_TE",
		"Configurar_la_opcion_de_indicacion_URC",


};

/************************************/
enum lista_respuesta_ec25_OK{
	OK=0,

};


const char *respuesta_ec25_OK[]={
		"OK",


};

/************************************/
void LIMPIAR_BUFFER(){
	for(i=0;i<50;i++){
		buffer_nuevo_comando_recibido[i]=0x00;
	}
	index_buffer_nuevo_comando_recibido=0;
}

/*******************************************************************************
 * Public Source Code
 ******************************************************************************/



int main(void) {


    /* Init board hardware. *//***********************/
    BOARD_InitBootPins();     /***********************/
    BOARD_InitBootClocks();   /***********************/
    BOARD_InitBootPeripherals();/*********************/
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL/**********/
    /* Init FSL debug console. */ /*******************/
    BOARD_InitDebugConsole();   /*********************/
#endif                         /**********************/


    printf("FRDM-K32L2B3 iniciando......\r\n");

    LPTMR_StartTimer(LPTMR0); //Inicializar LPTMR0
    bool boton1,boton2;
   float adc_sensor_de_luz;
   float adc_sensor_de_temperatura;


   /************************************************************************************************/
    while(1) {


    	if(NumeroDeByteDisponiblesEnBufferRx()!=0){

    		nuevo_byte_lpuart0=pullBytedesdeBuefferCircular();
    		buffer_nuevo_comando_recibido[index_buffer_nuevo_comando_recibido]=nuevo_byte_lpuart0;
    		index_buffer_nuevo_comando_recibido++;

    		char *puntero_ok;
    		if(nuevo_byte_lpuart0=='\n'){
    			for(int i=0;i<COMANDOS_DISPONIBLES;i++){
    				puntero_ok=(char*)(strstr((char*)(&buffer_nuevo_comando_recibido[0]),(char*)(comandos_control[i])));
    				if(puntero_ok!=0){
    					switch(i){

    					case GREEN_ON:
    					led_on_green();
    					break;

    					case GREEN_OFF:
    					led_off_green();
    				    break;

    					case RED_ON:
    					led_on_red();
    					break;

    					case RED_OFF:
    					led_off_red();
    					break;

    					case SENSOR_TEMPERATURA:
    					adc_sensor_de_temperatura=SensorDetemperaturaObtenerDatosADC();
    					printf("Temperatura en grados centigrados  : %.4f\r\n",adc_sensor_de_temperatura);
    					break;

    					case SENSOR_LUZ:
    					adc_sensor_de_luz=sensorDeLuzObtenerDatosADC();
    					printf("sensor de luz en unidades  lux : %.4f\r\n",adc_sensor_de_luz);
    					break;

    					}
    				}
    			}


      			for(int i=0;i<COMANDOS_DISPONIBLES_EC25;i++){
        			puntero_ok=(char*)(strstr((char*)(&buffer_nuevo_comando_recibido[0]),(char*)(ec25_comandos_at[i])));
        			if(puntero_ok!=0){
        				switch(i){

        				case informacion_del_producto:
        				printf("ATI\r\n");
        				break;

        				case identificacion_del_producto:
        				printf("AT+GMI\r\n");
        				break;

        				case identificacion_del_modelo_de_TA:
        				printf("AT+GMM\r\n");
        				break;

        				case identificacion_de_revision_versiondesoftware:
        				printf("AT+GMR\r\n");
        				break;

        				case Solicitar_identificacion_del_fabricante:
        				printf("AT+CGMI\r\n");
        				break;

        				case Solicitar_identificacion_de_modelo:
        				printf("AT+CGMM\r\n");
        				break;

        				case Solicitar_TA_de_la_version_d_esoftware:
        				printf("AT+CGMR\r\n");
        				break;

        				case Solicitar_identidad_del_equipo_movil_IMEI:
        				printf("AT+GSN\r\n");
        				break;

        				case Identificacion_numero_de_serie_del_producto:
        				printf("AT+CGSN\r\n");
        				break;

        				case Establecer_valores_predeter_del_fabricante:
        				printf("AT&F\r\n");
        				break;

        				case Mostrar_configuracion_actual:
        				printf("AT&V\r\n");
        				break;

        				case Almacenar_parametros_actuales:
        				printf("AT&W\r\n");
        				break;

        				case Establecer_todos_los_parametros_actuales:
        				printf("ATZ\r\n");
        				break;

        				case Establecer_el_modo_de_presentacion:
        				printf("ATQ\r\n");
        				break;

        				case Establecer_modo_de_eco_de_comando:
        				printf("ATE\r\n");
        				break;

        				case Repetir_linea_de_comando_anterior:
        				printf("A/\r\n");
        				break;

        				case Establecer_determinacion_de_la_linea_de_comandos:
        				printf("ATS3\r\n");
        				break;

        				case Establecer_caracter_de_formato_de_respuesta:
        				printf("ATS4\r\n");
        				break;

        				case Establecer_caracter_de_edicion_delineade_comando:
        				printf("ATS5\r\n");
        				break;

        				case Resultado_de_CONNECT_monitore0_progreso_llamada:
        				printf("ATX\r\n");
        				break;

        				case Configurar_la_funcionalidad_del_telefono:
        				printf("AT+CFUN\r\n");
        				break;

        				case Formato_del_mensaje_de_error:
        				printf("AT+CMEE\r\n");
        				break;

        				case Seleccione_el_juego_de_caracteres_TE:
        				printf("AT+CSCS\r\n");
        				break;

        				case Configurar_la_opcion_de_indicacion_URC:
        				printf("AT+QURCCFG\r\n");
        				break;
        				}
        			}
        		}

      			for(int i=0;i<COMANDOS_RESPUESTA_OK;i++){
      			     puntero_ok=(char*)(strstr((char*)(&buffer_nuevo_comando_recibido[0]),(char*)(respuesta_ec25_OK[i])));
      			     if(puntero_ok!=0){
      			        switch(i){

      			        case OK:
      					printf("   COMANDO  OK.\r\n");
      					break;


      			        }
      			      }
      			}

      			LIMPIAR_BUFFER();
    		}
    	} // fin del if  NumeroDeByteDisponiblesEnBufferRx


     if (lptmr0_irq_counter !=0){

    	 boton1=boton1leerestado();
    	 boton2=boton2leerestado();

    	 if (boton1 && !boton1_presionado){
    		 boton1_presionado=1;
    		 led_on_green();
    		 led_off_red();
    		 printf("BOTON 1 PRESIONADO...\r\n");
    	 }
    	 if (!boton1){
    		 boton1_presionado=0;
    	 }



    	 if (boton2 && !boton2_presionado){
    		 boton2_presionado=1;
    		 led_off_green();
    		 led_on_red();
    		 printf("BOTON 2 PRESIONADO...\r\n");
    	 }
    	 if (!boton2){
    		 boton2_presionado=0;
    	 }

     } //fin if lptmr0_irq_counter


    }//fin while  /*****************************************/

  return 0;
}

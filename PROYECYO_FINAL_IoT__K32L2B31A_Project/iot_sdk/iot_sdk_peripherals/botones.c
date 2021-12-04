/*! @file : botones.c
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
#include "botones.h"
#include "irq_lptmr0.h"
#include "stdio.h"
#include "modem.h"




/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/
extern uint32_t adc_sensor_de_luz;

/*******************************************************************************
 * Local vars
 ******************************************************************************/
bool boton1,boton2;


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
 bool boton1LeerEstado(void){
	 uint32_t valor_pin;
	 bool resultado;

	 valor_pin=GPIO_PinRead(GPIOC,3);
	 if(valor_pin!=0)
		 resultado=true;
	 else
		 resultado=false;

	 return(resultado);
 }

 bool boton2LeerEstado(void){
	 uint32_t valor_pin;
	 bool resultado;

	 valor_pin=GPIO_PinRead(GPIOA,4);
	 if(valor_pin!=0)
		 resultado=true;
	 else
		 resultado=false;

	 return(resultado);
  }

 char Boton1_Presionado(void){
	// temp=0;
  return !boton1;
 }

void Key_Task_Init(void){
 	boton1 = 0;
 	boton2 = 0;
 }

/*void Key_Task_Run(void){
 	boton1=boton1LeerEstado();
 	boton2=boton2LeerEstado();
 	if (!boton1 && !boton1_presionado){
 	    	boton1_presionado=1;
 	    		//adc_sensor_de_luz=sensorDeLuzObtenerDatosADC();
 	    	tiempopresionado=0;
 	 }
 	if(tiempopresionado==5000){
 	 	  if(!boton1 && !estado){
 	 		  printf("presionado\r\n");
 	 		  printf("%u\r\n",adc_sensor_de_luz);
 	 		  putchar(CNTL_Z);
 	 		  Modem_Rta_Cmd(3000,"OK",ST_MOD_PUBLIC_DAT,ST_MOD_CONN_PUB);
 	 		  estado=1;
 	 	  }
 	}

 	  if (boton1){
 		  boton1_presionado=0;
 		  estado=0;
 	 }


}*/






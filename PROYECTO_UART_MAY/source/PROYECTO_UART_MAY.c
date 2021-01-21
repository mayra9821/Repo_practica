#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h" //reciente

#include "sdk_hal_uart0.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
/*
* @brief Application entry point.
*/

#define BOARD_LED_GPIO BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

//nuevo desde aca
void delay(void)
{
volatile uint32_t i = 0;
for (i = 0; i < 800000; ++i)
{
__asm("NOP"); /* delay */
}
}//hasta aca

int Funcion_secuencia(unsigned char valor){
unsigned char a = 0;
switch (valor){
case '1':
GPIO_PortSet(GPIOB, 1u << 6U);
GPIO_PortSet(GPIOB, 1u << 10U);
printf("Luz verde parpadeante\r\n");
for (a=1;a<10;a++){
GPIO_PortClear(GPIOB, 1u << 7U);
delay();
GPIO_PortSet(GPIOB, 1u << 7U);
delay();
}

 printf("Luz verde parpadeante terminado\r\n");
break;
case '2':
GPIO_PortSet(GPIOB, 1u << 7U);
GPIO_PortSet(GPIOB, 1u << 10U);
printf("Luz roja parpadeante\r\n");
for (a=1;a<10;a++){
GPIO_PortClear(GPIOB, 1u << 6U);
delay();
GPIO_PortSet(GPIOB, 1u << 6U);
delay();
}

 printf("Luz roja parpadeante terminado\r\n");
break;
case '3':
GPIO_PortSet(GPIOB, 1u << 7U);
GPIO_PortSet(GPIOB, 1u <<6);
printf("Luz azul parpadeante\r\n");
for (a=1;a<10;a++){
GPIO_PortClear(GPIOB, 1u << 10U);
delay();
GPIO_PortSet(GPIOB, 1u << 10U);
delay();
}

 printf("Luz azul parpadeante terminado\r\n");
break;

 }
}


int main(void) {
/* Init board hardware. */
BOARD_InitBootPins();
BOARD_InitBootClocks();
BOARD_InitBootPeripherals();


 /* Define the init structure for the output LED pin*/
gpio_pin_config_t led_config = {
kGPIO_DigitalOutput, 0,
};

 /* Board pin, clock, debug console init */
BOARD_InitPins();
BOARD_BootClockRUN();
BOARD_InitDebugConsole();

 /* Print a note to terminal. */
PRINTF("\r\n actividad\r\n");
PRINTF("\r\n los leds estan apagados\r\n");

 /* Init output LED GPIO. */ // led rojo PTB6
GPIO_PinInit(GPIOB, 6U, &led_config);
//GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_RED_GPIO_PIN, &led_config);

 /* Init output LED GPIO. */ // led verde PTB7
//GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GREEN_GPIO_PIN, &led_config);
GPIO_PinInit(GPIOB, 7U, &led_config);

/* Init output LED GPIO. */ // led azul PTB10
//GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_BLUE_GPIO_PIN, &led_config);
GPIO_PinInit(GPIOB, 10U, &led_config);


#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
/* Init FSL debug console. */
BOARD_InitDebugConsole();
#endif

 (void)uart0Inicializar(115200);
GPIO_PortSet(GPIOB, 1u << 6U);
GPIO_PortSet(GPIOB, 1u << 7U);
GPIO_PortSet(GPIOB, 1u << 10U);

 while(1) {
status_t status;
uint8_t nuevo_byte;

 if(uart0NuevosDatosEnBuffer()>0){
status=uart0LeerByteDesdeBufferCircular(&nuevo_byte);
if(status==kStatus_Success){
printf("dato:%c\r\n",nuevo_byte);
}else{
printf("error\r\n");
}
}

 switch (nuevo_byte)

 {
case 'R':
GPIO_PortSet(GPIOB, 1u << 7U);
GPIO_PortSet(GPIOB, 1u << 10U);
GPIO_PortClear(GPIOB, 1u << 6U);

 printf("Luz roja encendida\r\n");

 nuevo_byte = 0;

 break;
case 'r':
GPIO_PortSet(GPIOB, 1u << 6U);
printf("Luz roja apagada\r\n");

 nuevo_byte = 0;

 break;
case 'V':
GPIO_PortSet(GPIOB, 1u << 6U);
GPIO_PortSet(GPIOB, 1u << 10U);
GPIO_PortClear(GPIOB, 1u << 7U);

 printf("Luz verde encendida\r\n");

 nuevo_byte = 0;

 break;
case 'v':
GPIO_PortSet(GPIOB, 1u << 7U);
printf("Luz verde apagada\r\n");

nuevo_byte = 0;
break;
case 'A':
GPIO_PortSet(GPIOB, 1u << 6U);
GPIO_PortSet(GPIOB, 1u << 7U);
GPIO_PortClear(GPIOB, 1u << 10U);

 printf("Luz azul encendida\r\n");

 nuevo_byte = 0;

 break;
case 'a':
GPIO_PortSet(GPIOB, 1u << 10U);
printf("Luz azul apagada\r\n");

 nuevo_byte = 0;
break;
default:
if (nuevo_byte > 48 && nuevo_byte < 58){
Funcion_secuencia(nuevo_byte);
}
nuevo_byte = 0;
}

 }
return 0 ;
}

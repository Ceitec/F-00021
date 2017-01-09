/*
 * Firmware.c
 *
 * Created: 22.8.2016 11:11:59
 * Author : atom2
 */ 

//Nastavení Fuse bits
/*

Extended = 0xFD
High = 0xD4
Low = 0xCF
	
*/

//#include <avr/iom32.h>
#include <avr/io.h>
#include "inc/AllInit.h"
# define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "inc/common_defs.h"
#include "inc/defines.h"
#include "inc/timer.h"
#include "inc/uart_types.h"
#include "inc/uart_tri_0.h"
#include "inc/Tribus_types.h"
#include "inc/Tribus.h"
#include <stdlib.h>
// Standard Input/Output functions
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>

//Define port
//Nepoužitelné
/*
PB0 - Xtal1
PB1 - Xtal2
PB2 - RS485 - RXD
PB3 - Reset
PA0 - RS485 - EN
PA1 - Led0
PA2 - Led1
PA3 - 
PA4 - 
PA5 - 
PA6 - 
PA7 - RS485 - TXD
*/

 	#define LED0_PORT	PORTA
 	#define LED0_DDR	DDRA
	#define LED1_PORT	PORTA
	#define LED1_DDR	DDRA
	
 	#define LED0_PIN	PORTA1
 	#define LED1_PIN	PORTA2

 	#define LED0_ON		sbi(LED0_PORT, LED0_PIN)
 	#define LED1_ON		sbi(LED1_PORT, LED1_PIN)

	#define LED0_OFF	cbi(LED0_PORT, LED0_PIN)
	#define LED1_OFF	cbi(LED1_PORT, LED1_PIN)

	#define LED0_TOG	nbi(LED0_PORT, LED0_PIN)
	#define LED1_TOG	nbi(LED1_PORT, LED1_PIN)

	#define	EEPROM_ADDRESS	0x16

// Init Servo
volatile uint16_t Servo_LEFT=2000, Servo_CENTER=3000, Servo_RIGHT=4000;


// Init pro Tribus
volatile byte timer0_flag = 0, Servo_Flag=0; // T = 10ms

byte led_timer = 0;




void send_data(void)
{
	uart0_put_data((byte *) &TB_bufOut);
}


ISR(TIMER0_COMPA_vect)
{
	// T = 10ms
	timer0_flag = TRUE;
}

uint16_t Citac=0;
//----------------------------------------------------------
void process_timer_100Hz(void)
{
	if (timer0_flag)
	{	
		// T = 10ms
		timer0_flag = FALSE;
		uart0_ISR_timer();
		if (Citac > 5)
		{
			//TB_SendAck(100,0);
			//LED0_TOG;
			//LED1_TOG;
			Citac=0;
		}
		Citac++;
	}
	
}


void try_receive_data(void)
{
	byte i;
	byte *ptr;
	//ORANGE_TOG;
	if (uart0_flags.data_received)
	{
		ptr = uart0_get_data_begin();
		for (i=0; i<9; i++)
		{
			TB_bufIn[i] = *ptr;
			ptr++;
		}
		uart0_get_data_end();
		uart0_flags.data_received = FALSE;
		if (TB_Read() == 0)
		{
			switch (TB_Decode())
			{
				//Nastavení
				case TB_CMD_SIO:
					switch(TB_bufIn[TB_BUF_TYPE])
					{
						case 0:
							switch(TB_bufIn[TB_BUF_MOTOR])
							{
								case 3:
									if (TB_Value == 0)
									{
										// Levá pozice
										OCR1B = Servo_LEFT;
										TB_SendAck(TB_ERR_OK, Servo_LEFT);	
										eeprom_update_word(( uint16_t *) EEPROM_ADDRESS + 18, Servo_LEFT);
									}
									else if (TB_Value == 1)
									{
										// Støední pozice
										OCR1B = Servo_CENTER;
										TB_SendAck(TB_ERR_OK, Servo_CENTER);
										eeprom_update_word(( uint16_t *) EEPROM_ADDRESS + 20, Servo_CENTER);
									}
									else if (TB_Value == 2)
									{
										// Pravá pozice
										OCR1B = Servo_RIGHT;
										TB_SendAck(TB_ERR_OK, Servo_RIGHT);
										eeprom_update_word(( uint16_t *) EEPROM_ADDRESS + 22, Servo_RIGHT);
									}
									else
									{
										TB_SendAck(TB_ERR_VALUE, 0);	
									}
									break;
								default:
									TB_SendAck(TB_ERR_NOK, 0);
									break;
							}
							break;
						case 20:
							switch(TB_bufIn[TB_BUF_MOTOR])
							{
								case 1:
									//Servo_LEFT
									eeprom_write_word(( uint16_t *) EEPROM_ADDRESS + 18, TB_Value);
									TB_SendAck(TB_ERR_OK, TB_Value);
									break;
								case 2:
									//Servo_CENTER
									eeprom_write_word(( uint16_t *) EEPROM_ADDRESS + 20, TB_Value);
									TB_SendAck(TB_ERR_OK, TB_Value);
									break;
								case 3:
									//Servo_RIGHT
									eeprom_write_word(( uint16_t *) EEPROM_ADDRESS + 22, TB_Value);
									TB_SendAck(TB_ERR_OK, TB_Value);
									break;
								default:
									TB_SendAck(TB_ERR_NOK, 0);
									break;
								
							}
							break;
						default:
							TB_SendAck(TB_ERR_TYPE, 0);
							break;
					}
					break;
				case 0:
				// špatný pøíkaz
					//TB_SendAck(TB_ERR_COMMAND, 0);
					break;
			}
		}
	}
}
uint16_t Perioda=0, Strida=0;



int main(void)
{
	//Nastavení Systemového enable pro RS485 pro UART0	
	sbi(DDRA, DDRA0);
	// Nastavení UART na piny PB2 a PA7 místo PA1 a PA2
	sbi(REMAP, U0MAP);
	
/* / *Rozložení pinù* / */
// 	sbi(DDRA, DDRA1);
// 	sbi(DDRA, DDRA2);
	sbi(LED0_DDR, LED0_PIN);
	sbi(LED1_DDR, LED1_PIN);
	//Nastavení výstupních PWM
	sbi(DDRA, DDRA1);	//TOCC0
	sbi(DDRA, DDRA2);	//TOCC1
	sbi(DDRA, DDRA3);	//TOCC2
	sbi(DDRA, DDRA4);	//TOCC3
	sbi(DDRA, DDRA5);	//TOCC4
	sbi(DDRA, DDRA6);	//TOCC5

	
	timer_init();
	
	uart0_init();
	
	TB_Callback_setBaud = &uart0_set_baud;
	TB_Callback_TX = &send_data;
	TB_Init((void*) 0x10); // addr in eeprom with setting
	
	
	
	Servo_LEFT = eeprom_read_word(( uint16_t *) EEPROM_ADDRESS + 18);
	Servo_CENTER = eeprom_read_word(( uint16_t *) EEPROM_ADDRESS + 20);
	Servo_RIGHT = eeprom_read_word(( uint16_t *) EEPROM_ADDRESS + 22);
	

 	sei();

    while(1)
    {
 		process_timer_100Hz();
 		uart0_process();
 		try_receive_data();
    }
}




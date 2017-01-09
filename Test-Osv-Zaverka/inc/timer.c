/*
 *
 * timer initialization and operation
 *
 */

#include <avr/io.h>
#include "common_defs.h"
#include "timer.h"

//----------------------------------------------------
void timer_init(void)
{
	
	uint16_t PWM00=1;
	uint16_t PWM01=100;
	uint16_t PWM02=250;
	uint16_t PWM03=1000;
  // Xtal = 14 745 600 Hz

  // * Timer 0 - unused
  //TCCR0 = 0; // stopped

	OCR0A = 144; // = TOP
	OCR0B = 100; // dummy
	TCCR0A = 2; // CTC mode, hardware outputs off
	TCCR0B = 5; // CTC mode, presc = 1024
	TIMSK0 |= BV(OCIE0A); // compare A int enabled

  // * Timer 1 - system timer and 2× PWM
  // 14745600 / 256 / 576 = 100 Hz
  //   Xtal  /presca/ TOP
  
//   OCR1A = 0;
//   OCR1B = 0;
//   ICR1 = 575; 
//   TCCR1A = BV(WGM11) | BV(COM1A1) | BV(COM1B1); // Fast PWM
//   TCCR1B = BV(WGM12) | BV(WGM13) | 4; // Fast PWM + presca = 1024
//   TIMSK1 |= BV(OCIE1A); // capt int enabled
	
	//Povolení výstupních C/C
	//TOCPMCOE |= (BV(TOCC0OE) | BV(TOCC1OE) | BV(TOCC2OE) | BV(TOCC3OE) | BV(TOCC4OE) | BV(TOCC5OE))
	//TCCR2A
	// Pin PA1 - TOCC0 - OC1B
	TOCPMSA0 = (BV(TOCC0S0));
	TOCPMCOE = (BV(TOCC0OE));
	
	TCCR1A = (BV(COM1A1) | BV(COM1B1) | BV(WGM11));
	TCCR1B = (BV(WGM12) | BV(WGM13) | BV(CS11));
	ICR1 = 40000 - 1;
	OCR1B = 2000;
	//1 ms = 2000
	//1,5 ms = 3000
	//2 ms = 4000
	
/* Tento kus kódu je pro regulaci osvìtlovaèe
	    // TOCC[0:2] = OC1A, OC1B, OC2B (01, 01, 10)
	    TOCPMSA0 = (1 << TOCC0S0) | (1 << TOCC1S0) | (1 << TOCC2S1) | (1 << TOCC3S1);
	    TOCPMCOE = (1 << TOCC0OE) | (1 << TOCC1OE) | (1 << TOCC2OE) | (1 << TOCC3OE) | (1 << TOCC4OE) | (1 << TOCC5OE);
	    
	    // Fast PWM, mode 14, non inverting, presc 1:1
	    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10);
	    TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21);
	    TCCR2B = (1 << WGM22) | (1 << WGM23) | (1 << CS20);
	    
		// Set TOP for 0,1ms period
	    ICR1 = 1600 - 1;
	    ICR2 = 1600 - 1;

	    // Set servos to 20, 50 and 80%
	    OCR1B = PWM00;	//TOCC0
		OCR1A = PWM01;	//TOCC1
	    OCR2B = PWM02;	//TOCC2
		OCR2A = PWM03;	//TOCC3
*/



	
	
  // * Timer 2 - unused
  //TCCR2 = 0; // stopped
}

//----------------------------------------------------

//----------------------------------------------------

/*
Autor:	Lukas
Soubor:	Hlavicka s daty
*/


//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*				Nutn� definovat #define MicroControler dle uC           */
/*																		*/
/*#define MicroController Atmega8										*/
/*#define MicroController Atmega16										*/
/*#define MicroController Atmega32										*/
/*#define MicroController Atmega64										*/
/*#define MicroController Atmega128										*/
/*#define MicroController Atmega644p									*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/
//////////////////////////////////////////////////////////////////////////


#ifndef AllInit_H_
#define AllInit_H_

//#define F_CPU 10000000UL
#define F_CPU 16000000UL
//#define F_CPU 14745600UL

// Definov�n� desky
#define Libs_Tescan	TEST_All
#define MCU	Attiny841
//Verze
#ifndef	Libs_Tescan
	#error "Pozor neni definovana deska (Seriove cislo)"
#else
	#warning "Je definovana spravna deska? - Kontrola v AllInit.h"
#endif
#ifndef MCU
	#error "Pozor neni definovan mikrokontroler"
#else
	#warning "Je definovan spravny mikrokontorler? - Kontrola v AllInit.h"
#endif

#define SC_MODUL	Libs_Tescan		// Modul Black Box
#define SC_VERZE	0			//Verze firmwaru modulu Black Box
#define SC_ADRESS	21			//Adresa aktu�ln�ho modulu


///////////////////////////////////////////////////////////////////////////
// Definov�n� rychlosti s�riov� linky pro rychlou editaci #CPU 14745600UL.
///////////////////////////////////////////////////////////////////////////
// #if F_CPU==14745600UL
// 
// 	#define	RS232_2400		383
// 	#define RS232_4800		191
// 	#define RS232_9600		95
// 	#define RS232_14400		63
// 	#define RS232_19200		47
// 	#define RS232_28800		31
// 	#define RS232_38400		23
// 	#define RS232_57600		15
// 	#define RS232_76800		11
// 	#define RS232_115200	7
// 	#define RS232_234000	3
// 	
// #elif F_CPU==16000000UL

// 	#define	RS232_2400		416
// 	#define RS232_4800		207
// 	#define RS232_9600		103
// 	#define RS232_14400		68
// 	#define RS232_19200		51
// 	#define RS232_28800		34
// 	#define RS232_38400		25
// 	#define RS232_57600		16
// 	#define RS232_76800		12
// 	#define RS232_115200	7
// 	#define RS232_234000	3

/*#endif*/

//Definovani TRUE a FALSE
#define TRUE	1
#define	FALSE	0

//Definov�n� uC
#define		Atmega8		0
#define		Atmega16	1
#define		Atmega32	2
#define		Atmega64	3
#define		Atmega128	4
#define		Atmega48	5
#define		Atmega88	6
#define		Atmega168	7
#define		Atmega328	8
#define		Atmega8A	9
#define		Atmega644P	10
#define		Attiny841	11

// Definov�n� z�kladn�ch karet
#define ZakladniKarta	0
#define OvladaniMotoru	1
#define OvladaniOsvetleni	3	// 3 a 4 adresa


#define InterLock		8
#define OvladaniOfuku	9	//Ovl�d�n� Ofuku
#define CteckaVzorku	10	//�te�ka vzork�
#define Motherboard		11	//Motherboard
#define DelayGenerator1	12	//DDG 1
#define Odtah			13	//Ovl�d�n� Odtahu
#define DelayGenerator2	14	//DDG 2
#define BlackBox		15	// Black Box
#define TEST_All			50	// Testovac� Firmware

//Deklarace procedur
void Text_to_Buffer(char Buffer); /*Uklada do Bufferu chary */
void Null_Buffer(); /* Vynuluje buffer */


#endif /* AllInit_H_ */
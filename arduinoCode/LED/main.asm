; This example program read the analog value of a LDR sensor from the AO pin (PC0) of the Arduino UNO
; Then based on the value of the sensor it turn off and on an LED.
; The analog value we read will be from range 0 to 255 

.include "m328pdef.inc"
.include "delay_Macro.inc"
.include "UART_Macros.inc"
.def A  = r16			; just rename or attach a label to the register
.def AH = r17			; just rename or attach a label to the register



.org 0x0000
; I/O Pins Configuration
SBI DDRB, PB5			; Set PB5 pin for Output to LED ldr
CBI PORTB, PB5			; LED OFF lde
SBI DDRB, PB4	         ;led for fire
cBI PORTB, PB4
CBI DDRB, PB3 ; PB3 set as INPUT pin
SBI PORTB, PB3
SBI DDRB, PB2			; Set PB5 pin for Output to LED water
CBI PORTB, PB2			; LED OFF lde
SBI DDRB, PB1			; Set PB5 pin for Output to LED water
CBI PORTB, PB1			; LED OFF lde
		; Set PB5 pin for Output to LED
LDI   A,0b11000111		; [ADEN ADSC ADATE ADIF ADIE ADIE ADPS2 ADPS1 ADPS0]
STS   ADCSRA,A			
LDI   A,0b01100000	; [REFS1 REFS0 ADLAR – MUX3 MUX2 MUX1 MUX0]
STS   ADMUX,A			; Select ADC0 (PC0) pin
SBI   PORTC,PC0	
Serial_begin

loop:
	delay 1000
    CBI 	PORTB, PB5	
	cBI 	PORTB, PB4
	cbi     PORTB, PB2
	cbi     PORTB,PB1

	LDI   A,0b11000111		; [ADEN ADSC ADATE ADIF ADIE ADIE ADPS2 ADPS1 ADPS0]
	STS   ADCSRA,A			
	LDI   A,0b01100000	; [REFS1 REFS0 ADLAR – MUX3 MUX2 MUX1 MUX0]
	STS   ADMUX,A			; Select ADC0 (PC0) pin
	SBI   PORTC,PC0	


	analogread
	cpi 	AH,150		; compare LDR reading with our desired threshold value e.g. 200
	brsh 	ledforldr	;jump if AH >= 200
	Serial_writeChar 'D'
	ldi ah,0
	CBI 	PORTB, PB5	
	cBI 	PORTB, PB4

	cbi     PORTB, PB2
	rjmp sensor2

rjmp loop
ledforldr:
	SBI 	PORTB, PB5		; LED ON
	Serial_writeChar 'L'
rjmp sensor2
sensor:
rjmp sensor2
sensor2:
	SBIC PINB,PB3 ; if not pressed, skip next line if the PINB reg. bit# 3 is 1
	rjmp Ledforfire
	Serial_writeChar 'W'
	rjmp sensor3
sensor3:
LDI   A,0b11000111		; [ADEN ADSC ADATE ADIF ADIE ADIE ADPS2 ADPS1 ADPS0]
	STS   ADCSRA,A			
	LDI   A,0b01100001	; [REFS1 REFS0 ADLAR – MUX3 MUX2 MUX1 MUX0]
	STS   ADMUX,A			; Select ADC0 (PC0) pin
	SBI   PORTC,PC1	

	ldi AH,0
	analogread
	cpi 	AH,100		; compare LDR reading with our desired threshold value e.g. 200
	brsh 	ledforwater	;jump if AH >= 100
	Serial_writeChar 'S'
	rjmp sensor4

sensor4:

LDI   A,0b11000111		; [ADEN ADSC ADATE ADIF ADIE ADIE ADPS2 ADPS1 ADPS0]
	STS   ADCSRA,A			
	LDI   A,0b01100010	; [REFS1 REFS0 ADLAR – MUX3 MUX2 MUX1 MUX0]
	STS   ADMUX,A			; Select ADC0 (PC0) pin
	SBI   PORTC,PC2	


	analogread

	cpi 	AH,100		; compare LDR reading with our desired threshold value e.g. 200
	brsh 	ledforgas	;jump if BH >= 100
	Serial_writeChar 'N'
	rjmp loop

Ledforfire:
	sBI PORTB, PB4 ; LED ON
	Serial_writeChar 'F'
rjmp sensor3
ledforwater:
	SBI 	PORTB, PB2		; LED ON
	Serial_writeChar 'R'
rjmp sensor4
ledforgas:
SBI 	PORTB, PB1
Serial_writeChar 'G'
rjmp loop
; ***************************************************************************
; *	Code written by:														*
; *		Syed Tehseen ul Hasan Shah											*
; *		Lecturer, University of Engineering and Technology Lahore, Pakistan	*
; *		24-December-2023													*
; ***************************************************************************
/*
 * delayMacro.inc
 *
 *  Created: 12/5/2023 9:39:56 AM
 *   Author: lenovo
 */ 
 ;***********************************************************************
;* macro: delay
;*
;* description: creates a delay for the specified number of milliseconds
;*
;* inputs: @0 - number of milliseconds to delay for
;*
;* registers modified: none
;***********************************************************************
.macro delay
push r18
push r24
push r25
ldi r18,@0/10
L1:
ldi r24,LOW(39998) ; intialize inner loop count in inner
ldi r25,HIGH(39998) ; loop high and low registers
L2:
sbiw r24,1 ; decrement inner loop registers
brne L2 ; branch to L2 if iLoop registers != 0
dec r18 ; decrement outer loop register
brne L1 ; branch to L1 if outer loop register != 0
nop ; no operation
pop r25
pop r24
pop r18
.endmacro

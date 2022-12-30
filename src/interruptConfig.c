
#include <stdlib.h>
#include <stdio.h>
#include "interruptConfig.h"

 /* *****************************************************************
 Name:		enable_interrupts()
 Inputs:	none
 Outputs:	none
 Description:enables interrupt INT0 (pin D2 and sets falling condition for firing) 
 ******************************************************************** */
 void enable_interrupts()
 {
	 cli();
	 EIMSK |= (1<<INT0);
	 EICRA |= (1<<ISC01);
	 sei();
 }
 
 /* *****************************************************************
 Name:		interruptConfig()
 Inputs:	none
 Outputs:	none
 Description:sets PIND2 as input 
 ******************************************************************** */
 void interruptConfig()
 {
	 DDRD &= ~(1<<PIND2);	// Input
	 //PORTD |= (1<<PIND2);
 }
#include "PowerManagement.h"

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>


/* *****************************************************************
Name:		enterSleep()
Inputs:		none
Outputs:	none
Description:configures the sleep mode
******************************************************************** */
void enterSleep(void)
{
	/* Set sleep mode */
	set_sleep_mode(SLEEP_MODE_IDLE);

	/* Enable sleep */
	sleep_enable();
  
	/* Now enter sleep mode. */
	sleep_mode();

	/* The program will continue from here after the WDT timeout*/
	sleep_disable(); /* First thing to do is disable sleep. */
  
	/* Re-enable the peripherals. */
	power_all_enable();
}
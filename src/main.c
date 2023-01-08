/*******************************/
/*** Created by Ovidiu Sabau ***/
/***	10th December 2022	 ***/
/*******************************/

#define F_CPU 16000000UL   

#include "driver_ds3231.h"
#include "I2C.h"
#include "PowerManagement.h"
#include "interruptConfig.h"
#include <asf.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

void setRegistersAlarm1(uint8_t);
uint8_t dec2bcd(uint8_t);

int main (void)
{
	uint8_t status_reg;
	/* Initialization of interrupt */
	interruptConfig();
	/* Set alarm */
	setRegistersAlarm1(7);
	/* Reset status register */
	write_i2c(DS3231_REG_STATUS, 0x0);
	/* Set alarm 1 and turn on the interrupt */
	write_i2c(DS3231_REG_CONTROL, 0x5);

	while(1)
	{	
		/* Read status register */
		status_reg = read_i2c(DS3231_REG_STATUS);
		if ((status_reg & 0x01) > 0)
		{
			/* Reset Alarm flag*/
			write_i2c(DS3231_REG_STATUS, 0x0);
		}

		_delay_ms(4000);
		/* Go to sleep */
		enable_interrupts();
		enterSleep();
	}
	
	return 0;
}

/* *****************************************************************
Name:		dec2bcd()
Inputs:		Value to be converted for DS3231 registers
Outputs:	Value for seconds, minutes, hour registers
Description:converts decimal values into ds3231 register format
******************************************************************** */
uint8_t dec2bcd(uint8_t val)
{
	uint8_t i, j, k;
	
	i = val / 10;            /* get tens place */
	j = val % 10;            /* get ones place */
	k = j + (i << 4);        /* set bcd */
	
	return k;                /* return bcd */
}


/* *****************************************************************
Name:		Interrupt service routine 0
Inputs:		none
Outputs:	none
Description:wakes up MCU when an external interrupt on pin PIND2 occurs
******************************************************************** */
ISR(INT0_vect)
{
	// Wake up
}


void setRegistersAlarm1(uint8_t time_value)
{
	/*------- Alarm 1 ---------- */
	write_i2c(DS3231_REG_ALARM1_SECOND, dec2bcd(time_value));	// Set alarm 1 to trigger at the time time_value
	write_i2c(DS3231_REG_ALARM1_MINUTE, 128);
	write_i2c(DS3231_REG_ALARM1_HOUR, 128);
	write_i2c(DS3231_REG_ALARM1_WEEK, 128);
}

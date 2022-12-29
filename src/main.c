/*******************************/
/*** Created by Ovidiu Sabau ***/
/***	10th December 2022	 ***/
/*******************************/

#define F_CPU 16000000UL   

#include "driver_ds3231_interface.h"
#include "driver_ds3231.h"
#include "driver_ds3231_basic.h"
#include "I2C.h"
#include "LCD.h"
#include "PowerManagement.h"
#include "interruptConfig.h"
#include <asf.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

void setRegistersAlarm1(void);
void setAlarm1(void);
uint8_t readStatusAlarm1(void);
uint8_t dec2bcd(uint8_t);

int main (void)
{
	uint8_t st_reg = 0;
	//initLCD();
	//ds3231_basic_init();
	interruptConfig();


	
	
	/* Main loop */
	while(1)
	{
		/* Set alarm 1*/
		setRegistersAlarm1();
		setAlarm1();
			
		i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
		i2c_write(DS3231_REG_STATUS);
		i2c_write(0x0);
		
// 		st_reg = readStatusAlarm1();
// 		if ((st_reg & 0x01) > 0)
// 		{
// 			
// 		}
		_delay_ms(2000);
		
		/* Power management */
		//enable_interrupts();
		//enterSleep();
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
	//cli();
	_delay_ms(10);
}

void setRegistersAlarm1(void)
{
	/*------- Alarm 1 ---------- */
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_SECOND);
	i2c_write(dec2bcd(7));	// Seconds in dec
	
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_MINUTE);
	i2c_write(128);
	
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_HOUR);
	i2c_write(128);
	
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_WEEK);
	i2c_write(128);
}

void setAlarm1(void)
{
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_CONTROL);
	i2c_write(0x5);		// Set bit 0 and 2 (alarm 1 and interrupt)
}

uint8_t readStatusAlarm1(void)
{
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_STATUS);
	/* Send START condition with SLA+R */
	i2c_rep_start((DS3231_ADDRESS<<1)+I2C_READ);
	/* Receive data */
	uint8_t st_reg = i2c_readNak();
	
	return st_reg;
}
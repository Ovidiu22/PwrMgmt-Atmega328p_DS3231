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
void displayRegistersAlarm1(void);
uint8_t dec2bcd(uint8_t);
void blink_LED_PORTB(uint8_t, uint8_t);
//void TURNOFF_PORTB(uint8_t);
//void TURNON_PORTB(uint8_t);

volatile int toggle = 1;

int main (void)
{
	ds3231_time_t t;
	
	initLCD();
	ds3231_basic_init();
	interruptConfig();
	
	/* Set alarm */
	setRegistersAlarm1();
	
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_CONTROL);
	i2c_write(0x5);		// Set bit 0 and 2 (alarm 1 and interrupt)

	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_STATUS);
	i2c_write(0x0);

	
	//displayRegistersAlarm1();
	/* Read and display current time */
		while(1)
		{
			
			//ds3231_basic_init();
			updateLCDScreen(1, "Basic init ", NONE, "NONE");
			
			setRegistersAlarm1();
			updateLCDScreen(2, "RegAlarm Set ", NONE, "NONE");
			
			i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
			i2c_write(DS3231_REG_CONTROL);
			i2c_write(0x5);		// Set bit 0 and 2 (alarm 1 and interrupt)

			updateLCDScreen(3, "Reg control set", NONE, "NONE");
			
			i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
			i2c_write(DS3231_REG_STATUS);
			i2c_write(0x0);
			
			updateLCDScreen(4, "Reg status set", NONE, "NONE");
			
			toggle = 0;
			
			i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
			i2c_write(DS3231_REG_STATUS);
			/* Send START condition with SLA+R */
			i2c_rep_start((DS3231_ADDRESS<<1)+I2C_READ);
			/* Receive data */
			uint8_t st_reg = i2c_readNak();
		
			if ((st_reg & 0x01) > 0)
			{
				i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
				i2c_write(DS3231_REG_STATUS);
				i2c_write(0x0);
			}
			//blink_LED_PORTB(PINB0, 10);
		
			//ds3231_basic_get_time(&t);
 			//updateLCDScreen(1, "ST: ", (st_reg & 0x01), "NONE");
//   			updateLCDScreen(2, "Hour: ", t.hour, "NONE");
//   			updateLCDScreen(3, "Minutes:", t.minute, "NONE");
//   			updateLCDScreen(4, "Second:", t.second, "NONE");
			_delay_ms(4000);
			clearScreen();
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
	toggle = 1;
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

void displayRegistersAlarm1(void)
{
	/*------- Alarm 1 ---------- */
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_SECOND);
	/* Send START condition with SLA+R */
	i2c_rep_start((DS3231_ADDRESS<<1)+I2C_READ);
	/* Receive data */
	uint8_t A1M1 = i2c_readNak();
	
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_MINUTE);
	/* Send START condition with SLA+R */
	i2c_rep_start((DS3231_ADDRESS<<1)+I2C_READ);
	/* Receive data */
	uint8_t A1M2 = i2c_readNak();
	
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_HOUR);
	/* Send START condition with SLA+R */
	i2c_rep_start((DS3231_ADDRESS<<1)+I2C_READ);
	/* Receive data */
	uint8_t A1M3 = i2c_readNak();
	
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	i2c_write(DS3231_REG_ALARM1_WEEK);
	/* Send START condition with SLA+R */
	i2c_rep_start((DS3231_ADDRESS<<1)+I2C_READ);
	/* Receive data */
	uint8_t A1M4 = i2c_readNak();
	
	updateLCDScreen(1, "A1M1: ", A1M1, "NONE");
	updateLCDScreen(2, "A1M2: ", A1M2, "NONE");
	updateLCDScreen(3, "A1M3: ", A1M3, "NONE");
	updateLCDScreen(4, "A1M4: ", A1M4, "NONE");

}

void blink_LED_PORTB(uint8_t pinNumber, uint8_t times)
{
	DDRB |= (1 << pinNumber);
	for (uint8_t i = 0; i < times; i++)
	{
		PORTB |= ( 1 << pinNumber );
		_delay_ms(100);
		PORTB &= ~( 1 << pinNumber );
		_delay_ms(100);
	}
}

#if 0
void TURNON_PORTB(uint8_t pinNumber)
{
	DDRB |= (1 << pinNumber);
	PORTB |= ( 1 << pinNumber );
}

void TURNOFF_PORTB(uint8_t pinNumber)
{
	//DDRB |= (1 << pinNumber);
	PORTB &= ~( 1 << pinNumber );
}
#endif
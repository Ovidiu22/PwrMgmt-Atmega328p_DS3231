#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <inttypes.h>
#include <compat/twi.h>
#include "driver_ds3231.h"
#include "I2C.h"

/* *****************************************************************
Name:		write_i2c()
Inputs:		Register and value to send over I2C
Outputs:	none
Description:Starts communication over I2C and send a value
******************************************************************** */
void write_i2c(unsigned char reg, unsigned char value)
{
	/* Send START condition with SLA+W */
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	/* Choose register */
	i2c_write(reg);
	/* Send data */
	i2c_write(value);
	/* Send stop condition */
	i2c_stop();
}

/* *****************************************************************
Name:		read_i2c()
Inputs:		Register to read from
Outputs:	register value
Description:Reads the value of a register over SPI
******************************************************************** */
unsigned char read_i2c(unsigned char reg)
{
	uint8_t res = 0;
	
	/* Send START condition with SLA+W */
	i2c_start((DS3231_ADDRESS<<1)+I2C_WRITE);
	/* Choose register */
	i2c_write(reg);
	/* Send START condition with SLA+R */
	i2c_rep_start((DS3231_ADDRESS<<1)+I2C_READ);
	/* Receive data */
	res = i2c_readNak();
	/* Send stop condition */
	i2c_stop();
	
	return res;
}

/* *****************************************************************
Name:		i2c_init()
Inputs:		none
Outputs:	none
Description:Initializes the I2C communication
******************************************************************** */
void i2c_init(void)
{
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */ 
  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */

}

/* *****************************************************************
Name:		i2c_start()
Inputs:		TWDR register (Address + direction / Data to send)
Outputs:	none
Description:Starts I2C communcation, sends SLA+W or SLA+R
******************************************************************** */
unsigned char i2c_start(unsigned char transmissionMode)
{
	uint8_t   twst = 0;
	/* 1. Send START condition */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// Check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;		// TWSR
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

	/* 2. Send transmission mode SLA+W (MT) or SLA+R (MR) */

	TWDR = transmissionMode;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;
}

/* *****************************************************************
Name:		i2c_rep_start()
Inputs:		TWDR register (Address + direction / Data to send)
Outputs:	0 device accessible
			1 failed to access device
Description:Sends a repeated start condition, address and transfer direction
******************************************************************** */
unsigned char i2c_rep_start(unsigned char address)
{
    return i2c_start( address );
}

/* *****************************************************************
Name:		i2c_stop()
Inputs:		none
Outputs:	none
Description:Terminates the I2C communication and releases the BUS
******************************************************************** */
void i2c_stop(void)
{
	/* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));
}

/* *****************************************************************
Name:		i2c_write()
Inputs:		Byte to the transferred
Outputs:	0 write successful
			1 write failed
Description:Sends one byte to I2C device
******************************************************************** */
uint8_t i2c_write( uint8_t data )
{	

	 uint8_t   twst;
	 // send data to the previously addressed device
	 TWDR = data;
	 TWCR = (1<<TWINT) | (1<<TWEN);

	 // wait until transmission completed
	 while(!(TWCR & (1<<TWINT)));

	 // check value of TWI Status Register. Mask prescaler bits
	 twst = TW_STATUS & 0xF8;
	 if( twst != TW_MT_DATA_ACK) return 1;

	 return 0;
}

/* *****************************************************************
Name:		i2c_readAck()
Inputs:		none
Outputs:	Data received from I2C device
Description:Receives and returns the data from I2C device.
******************************************************************** */
unsigned char i2c_readAck(void)
{
	uint8_t   twst = 0;
	while (1)	// Read data only when data acknowledged
	{
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
		while(!(TWCR & (1<<TWINT)));
		
		twst = TW_STATUS & 0xF8;
		if( twst == TW_SR_DATA_ACK)
		{
			return TWDR;
		}
	}
}

/* *****************************************************************
Name:		i2c_readNak()
Inputs:		none
Outputs:	Data received from I2C device
Description:Receives and returns the data from I2C device.
******************************************************************** */
unsigned char i2c_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
	return TWDR;
}



#ifndef I2C_H_
#define I2C_H_

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

	#include <avr/io.h>
	
	/* I2C clock in Hz */
	#define SCL_CLOCK  100000L
		
	/* defines the data direction to read */
	#define I2C_READ    1

	/* defines the data direction to write */
	#define I2C_WRITE   0

	/* Functions */
	void write_i2c(unsigned char value);
	void i2c_init(void);
	void i2c_stop(void);
	unsigned char i2c_start(unsigned char addr);
	unsigned char i2c_rep_start(unsigned char addr);
	uint8_t i2c_write(uint8_t data);
	unsigned char i2c_readAck(void);
	unsigned char i2c_readNak(void);
	
#endif /* I2C_H_ */
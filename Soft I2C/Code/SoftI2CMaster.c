// https://github.com/todbot/SoftI2CMaster/blob/master/SoftI2CMaster.cpp
//-----------------------------------------------------------
// Pin Configuration
#define sda_pin		1
#define scl_pin		2
#define pin_bank	0


//-----------------------------------------------------------

#include "SoftI2CMaster.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>
#include "gpio-mmap.h"


#define  I2C_ACK  1 
#define  I2C_NAK  0


#define i2c_scl_release()                 \
    gpio_input(pin_bank, scl_pin);
#define i2c_sda_release()                 \
    gpio_input(pin_bank, sda_pin);

// sets SCL low and drives output
#define i2c_scl_lo()                                   \
                     GPIO_WRITE(pin_bank, scl_pin, 0); \
					 gpio_output(pin_bank, scl_pin);

// sets SDA low and drives output
#define i2c_sda_lo()                                   \
                     GPIO_WRITE(pin_bank, sda_pin, 0); \
					 gpio_output(pin_bank, sda_pin);

// set SCL high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_scl_hi()                                 \
                     gpio_input(pin_bank, scl_pin);  \
					 GPIO_WRITE(pin_bank, scl_pin, 1);

// set SDA high and to input (releases pin) (i.e. change to input,turnon pullup)
#define i2c_sda_hi()                                 \
                     gpio_input(pin_bank, sda_pin);  \
					 GPIO_WRITE(pin_bank, sda_pin, 1); 



//-- Double check
struct timespec delay = { // check if it generate 10us have to 50us  ???? #define  i2cbitdelay 50
	0, 10000L
};


//
char beginTransmission(char address)
{
    i2c_start();
    char rc = i2c_write((address<<1) | 0); // clr read bit
    // The standard Wire library returns a status in endTransmission(), not beginTransmission().
    // So we will return the status here but also remember the result so we can return it in endTransmission().
    // It also allows us to disable other I2C functions until beginTransmission has been called, if we want.
    initialized = rc;
    return rc;
}

//
char requestFrom(char address)
{
    i2c_start();
    char rc = i2c_write((address<<1) | 1); // set read bit
    return rc;
}

//
char endTransmission(void)
{
    i2c_stop();
    return initialized;   // Use the result of beginTransmission()
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
void my_write(char* data, char quantity)
{
    char i =0;
    for(i = 0; i < quantity; ++i){
        return i2c_write(data);
    }
}


//--------------------------------------------------------------------
void i2c_writebit( char c )
{
    if ( c > 0 ) {
        i2c_sda_hi();
    } else {
        i2c_sda_lo();
    }

    i2c_scl_hi();
    nanosleep(&delay, NULL);

    i2c_scl_lo();
    nanosleep(&delay, NULL);

    if ( c > 0 ) {
        i2c_sda_lo();
    }
    nanosleep(&delay, NULL);
}

//
char i2c_readbit(void)
{
    char c;

    i2c_sda_hi();
    i2c_scl_hi();
    nanosleep(&delay, NULL);

    c = GPIO_READ(pin_bank, sda_pin);

    i2c_scl_lo();
    nanosleep(&delay, NULL);

    return (c) ? 1 : 0;
}

// Inits bitbanging port, must be called before using the functions below
//
void i2c_init(void)
{
    //-- Release SDA & SCL	
    initialized = 255;

	gpio_map();
    i2c_sda_hi();
    i2c_scl_hi();
    
    nanosleep(&delay, NULL);
}

// Send a START Condition
//
void i2c_start(void)
{
    // set both to high at the same time
    i2c_sda_hi();
    i2c_scl_hi();

    nanosleep(&delay, NULL);
   
    i2c_sda_lo();
    nanosleep(&delay, NULL);

    i2c_scl_lo();
    nanosleep(&delay, NULL);
}

void i2c_repstart(void)
{
    // set both to high at the same time (releases drive on both lines)
    i2c_sda_hi();
    i2c_scl_hi();

    i2c_scl_lo();                           // force SCL low
    nanosleep(&delay, NULL);

    i2c_sda_release();                      // release SDA
    nanosleep(&delay, NULL);

    i2c_scl_release();                      // release SCL
    nanosleep(&delay, NULL);

    i2c_sda_lo();                           // force SDA low
    nanosleep(&delay, NULL);
}

// Send a STOP Condition
//
void i2c_stop(void)
{
    i2c_scl_hi();
    nanosleep(&delay, NULL);

    i2c_sda_hi();
    nanosleep(&delay, NULL);
}

// write a byte to the I2C slave device
//
char i2c_write( char c )
{
    char i;
    for ( i=0;i<8;i++) {
        i2c_writebit( c & 128 );
        c<<=1;
    }

    return i2c_readbit();
}

// read a byte from the I2C slave device
//
char i2c_read( char ack )
{
    char res = 0;
    char i;
    for ( i=0;i<8;i++) {
        res <<= 1;
        res |= i2c_readbit();  
    }

    if ( ack )
        i2c_writebit( 0 );
    else
        i2c_writebit( 1 );

    nanosleep(&delay, NULL);

    return res;
}


//===================================
char my_read()
{
    return i2c_read( I2C_ACK );
}

//
char readLast()
{
    return i2c_read( I2C_NAK );
}

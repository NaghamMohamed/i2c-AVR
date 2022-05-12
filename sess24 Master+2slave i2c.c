/*
 * i2c_slave.c
 *
 * Created: 31-Jan-21 9:53:45 PM
 *  Author: DELL
 */ 

#define F_CPU 1000000
#define F_I2C 50000
#include<util/delay.h>
#include <avr/io.h>

void i2c_initslave(void)
{   TWSR=0x00;	//set prescaler bits to zero
	TWBR=(F_CPU/(2*F_I2C))-8; //serial clock frequency
	TWCR|=(1<<2);
	TWAR=0x0c;
	TWCR|=(1<<7)|(1<<6)|(1<<TWEN);
}
void i2c_listen(void)
{
	while((TWCR&(1<<TWINT))==0);

}
void start(void)
{
	TWCR|=(1<<7);//flag zero
	TWCR|=(1<<5);//Start cond
	while((TWCR&(1<<7))==0);//polling
	
}
void stop(void)
{
	TWCR|=(1<<4);//reset int flag
	TWCR|=(1<<7);//flag zero
}
void write(unsigned char data)
{
	TWDR=data;
	TWCR|=(1<<7);//flag zero
	while((TWCR&(1<<7))==0);//polling
}
unsigned char read(void)
{
	TWCR|=(1<<7);
	while((TWCR&(1<<7))==0);//polling
	return TWDR;
}

int main(void)
{   i2c_initslave();
	DDRA=0xff;
	
    while(1)
    {
       i2c_listen();
	   int rx=read();
	   PORTA=rx;
	
    }
}
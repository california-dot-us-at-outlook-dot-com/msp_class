/*
 * handle.h
 *
 *  Created on: 2018年10月31日
 *      Author: Cal
 */

#ifndef HANDLE_H_
#define HANDLE_H_

#define SDA2 BIT0   //接at24c02
#define SCL2 BIT1
#define port2Out P4OUT
#define port2In P4IN
#define port2Dir P4DIR
#define port2Din port2Dir &= ~(SDA2+SCL2)
#define port2Dout port2Dir |= SDA2+SCL2
#define portSda2Out port2Dir |= SDA2
#define portScl2In port2Dir &= ~SCL2
#define portSda2In port2Dir &= ~SDA2
#define portScl2Out port2Dir |= SCL2

unsigned char readAt(unsigned char);
unsigned char writeAt(unsigned char ,unsigned char );
void I2c_2_Write(unsigned char ,unsigned char);
unsigned char I2c_2_Read(unsigned char);



#endif /* HANDLE_H_ */

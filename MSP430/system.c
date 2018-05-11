/*
 * system.c
 *
 *  Created on: 26.10.2012
 *      Author	: graeber
 *      brief	: system specific functions
 */

/************************************************************************/
/*                		Includes                                		*/
/************************************************************************/
#include	"msp430f169.h"
#include 	"system.h"
#include	"config.h"
/************************************************************************/
/*                		Global Variables                           		*/
/************************************************************************/
uint8_t I2C_WriteBuffer[I2CWriteBufferSize];
uint8_t *I2C_WriteBufptr = I2C_WriteBuffer;

/************************************************************************
/! \fn			void write_i2c(uint8_t adr, uint8_t *buf, uint8_t count)
*  \brief		writes count byte to the adr
*  \param		uint8_t adr, uint8_t *buf, uint8_t count
*  \exception	none
*  \return		none
************************************************************************/
void write_i2c(uint8_t adr, uint8_t *buf, uint8_t count){

	I2C_WriteBufptr	=	buf;
	U0CTL |= I2C + SYNC;                      // Switch USART0 to I2C mode
	U0CTL &= ~I2CEN;                          // Recommended init procedure
	I2CTCTL = I2CSSEL_2;                      // SMCLK
	I2CNDAT = count;                          // Transmit Two bytes
	I2CSA = adr;                        	  // Slave address
	U0CTL |= I2CEN;                           // Enable I2C, 7 bit addr
	I2CIE = TXRDYIE;                          // I2C receive ready interrupt enable
	U0CTL |= MST;                             // Master
	I2CTCTL |= I2CTRX + I2CSTT + I2CSTP;      // Transmit, ST, SP (clears MST)
	_BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
	while ((I2CTCTL & I2CSTP) == 0x02);

}

/************************************************************************
/! \fn			void write_i2c(uint8_t adr, uint8_t *buf, uint8_t count)
*  \brief		writes count byte to the adr
*  \param		uint8_t adr, uint8_t *buf, uint8_t count
*  \exception	none
*  \return		none
************************************************************************/
void write_i2c2(uint8_t adr, uint8_t *buf, uint8_t count){

	I2C_WriteBufptr	=	buf;
	U0CTL |= I2C + SYNC;                      // Switch USART0 to I2C mode
	U0CTL &= ~I2CEN;                          // Recommended init procedure
	I2CTCTL = I2CSSEL_2;                      // SMCLK
	I2CNDAT = count;                          // Transmit Two bytes
	I2CSA = adr;                        	  // Slave address
	U0CTL |= I2CEN;                           // Enable I2C, 7 bit addr
	I2CIE = TXRDYIE;                          // I2C receive ready interrupt enable
	U0CTL |= MST;                             // Master
	I2CTCTL |= I2CTRX + I2CSTT + I2CSTP;      // Transmit, ST, SP (clears MST)
	//_BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
	//while ((I2CTCTL & I2CSTP) == 0x02);

}

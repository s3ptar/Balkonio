/*
 * i2c.c
 *
 *  Created on: 18.10.2012
 *      Author: graeber
 */
/************************************************************************/
/*                		Includes                                		*/
/************************************************************************/
#include	"msp430f169.h"


/************************************************************************
/! \fn			void init_i2c(void)
*  \brief		initial the i2c pins
*  \param		none
*  \exception	none
*  \return		none
************************************************************************/

void init_i2c(void){
	P3SEL |= 0x0A;                            // Select I2C pins
	U0CTL |= I2C + SYNC;                      // Recommended init procedure
	U0CTL &= ~I2CEN;                          // Recommended init procedure
}

/*
 * Config.h
 *
 *  Created on: 17.10.2012
 *      Author: graeber
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/************************************************************************/
/*                			Includes                               		*/
/************************************************************************/

/************************************************************************/
/*                			Defines                               		*/
/************************************************************************/
#define LXT1CLK  			32768UL
#define LXT2CLK 			7372800UL
#define LXT1CLK_RTC_Div		32

#define PCF8575_1			0x27
#define PCF8575_2			0x26
#define PCF8575_3			0x21
#define PCF8575_4			0x20

#define I2CWriteBufferSize	0x4

#define _debug_ 		1

#define _Volume_TimeOut_    10000


#endif /* CONFIG_H_ */

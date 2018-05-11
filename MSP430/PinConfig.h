/*
 * PinConfig.h
 *
 *  Created on: 19.10.2012
 *      Author: graeber
 */

#ifndef PINCONFIG_H_
#define PINCONFIG_H_

/************************************************************************/
/*                		Includes                                		*/
/************************************************************************/
#include	"msp430f169.h"

/************************************************************************/
/*                			Defines                               		*/
/************************************************************************/
//Segmentansteuerung
#define	Seg0_BIT				BIT2
#define Seg1_BIT				BIT0
#define Seg2_BIT				BIT5
#define Seg3_BIT				BIT4
#define Seg4_BIT				BIT6
#define Seg5_BIT				BIT3
#define Seg6_BIT				BIT1
#define Seg7_BIT				BIT7
#define	Seg_PORT				P1OUT
#define	Seg_DDR					P1DIR
#define	Seg_PIN					P1IN
#define	Seg_SEL					P1SEL

#define Dis0_BIT				BIT0
#define Dis1_BIT				BIT1
#define Dis2_BIT				BIT2
#define Dis3_BIT				BIT4
#define Dis4_BIT				BIT5
#define Dis5_BIT				BIT6
#define	Dis_PORT				P2OUT
#define Dis_DDR					P2DIR
#define Dis_PIN					P2IN
#define	Dis_SEL					P2SEL

//DFC
#define	DCF_Enable_BIT			BIT0
#define DCF_DATA_BIT			BIT1
#define CTRL_PIN_DCF			BIT2
#define	DCF_DDR					P4DIR
#define	DCF_PORT				P4OUT
#define DCF_PIN					P4IN
#define	DCF_SEL					P4SEL


//Rotary Encoder
#define Sig_A                   BIT0
#define Sig_B                   BITB
#define Encoder_PORT            P5OUT
#define Encoder_PIN             P5IN
#define Encoder_DDR             P5DIR
#define Encoder_SEL             P5SEL

//E8504CZZ

/*
 * Red = 5V
 * Blue = GND
 * Rosa = 3V3
 *
 * */




#define load_BIT                BIT3
#define load_PORT               P5OUT
#define load_PIN                P5IN
#define load_DDR                P5DIR
#define load_SEL                P5SEL

#define cp_BIT                  BIT2
#define cp_PORT                 P5OUT
#define cp_PIN                  P5IN
#define cp_DDR                  P5DIR
#define cp_SEL                  P5SEL

#define flm_BIT                 BIT1
#define flm_PORT                P5OUT
#define flm_PIN                 P5IN
#define flm_DDR                 P5DIR
#define flm_SEL                 P5SEL

#define contrast_BIT            BIT0
#define contrast_PORT           P5OUT
#define contrast_PIN            P5IN
#define contrast_DDR            P5DIR
#define contrast_SEL            P5SELR

#define DataE8504_PORT          P5OUT
#define DataE8504_PIN           P5IN
#define DataE8504_DDR           P5DIR
#define DataE8504_SEL           P5SEL
#define DataPortMask            0xF0



#endif /* PINCONFIG_H_ */

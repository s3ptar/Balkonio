/*
 * Segment_drv.h
 *
 *  Created on: 19.10.2012
 *      Author: graeber
 */

#ifndef SEGMENT_DRV_H_
#define SEGMENT_DRV_H_

/************************************************************************/
/*                		Includes                                		*/
/************************************************************************/
#include	"PinConfig.h"

/************************************************************************/
/*                		Macros                                			*/
/************************************************************************/

#define init_Seg			(Seg_DDR |= 0xFF)
#define init_Dis			(Dis_DDR |= Dis0_BIT|Dis1_BIT|Dis2_BIT|Dis3_BIT|Dis4_BIT|Dis5_BIT)

/*
 *
 *      a
 * 	   ___
 *  f |   | b
 *    |_g_|
 *    |   |
 *  e |___| c
 *      d
 *
 *  a = Seg0
 *  b = Seg1
 *  c = Seg2
 *  d = Seg3
 *  e = Seg4
 *  f = Seg5
 *  g = Seg6
 * dp = Seg7
 */



#define	Seg0_Off			(Seg_PORT &= ~(Seg0_BIT))
#define	Seg1_Off			(Seg_PORT &= ~(Seg1_BIT))
#define	Seg2_Off			(Seg_PORT &= ~(Seg2_BIT))
#define	Seg3_Off			(Seg_PORT &= ~(Seg3_BIT))
#define	Seg4_Off			(Seg_PORT &= ~(Seg4_BIT))
#define	Seg5_Off			(Seg_PORT &= ~(Seg5_BIT))
#define	Seg6_Off     		(Seg_PORT &= ~(Seg6_BIT))
#define	Seg7_Off			(Seg_PORT &= ~(Seg7_BIT))
#define	Seg0_On 			(Seg_PORT |= (Seg0_BIT))
#define	Seg1_On 			(Seg_PORT |= (Seg1_BIT))
#define	Seg2_On 			(Seg_PORT |= (Seg2_BIT))
#define	Seg3_On 			(Seg_PORT |= (Seg3_BIT))
#define	Seg4_On 			(Seg_PORT |= (Seg4_BIT))
#define	Seg5_On 			(Seg_PORT |= (Seg5_BIT))
#define	Seg6_On 			(Seg_PORT |= (Seg6_BIT))
#define	Seg7_On 			(Seg_PORT |= (Seg7_BIT))

#define Dis0_Off			(Dis_PORT |= (Dis0_BIT))
#define Dis1_Off			(Dis_PORT |= (Dis1_BIT))
#define Dis2_Off			(Dis_PORT |= (Dis2_BIT))
#define Dis3_Off			(Dis_PORT |= (Dis3_BIT))
#define Dis4_Off			(Dis_PORT |= (Dis4_BIT))
#define Dis5_Off			(Dis_PORT |= (Dis5_BIT))

#define Dis0_On 	  		(Dis_PORT &= ~(Dis0_BIT))
#define Dis1_On 	 		(Dis_PORT &= ~(Dis1_BIT))
#define Dis2_On 			(Dis_PORT &= ~(Dis2_BIT))
#define Dis3_On 			(Dis_PORT &= ~(Dis3_BIT))
#define Dis4_On 			(Dis_PORT &= ~(Dis4_BIT))
#define Dis5_On 			(Dis_PORT &= ~(Dis5_BIT))

#define SegOff				(Seg_PORT = 0x00)
#define Show0				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg2_BIT|Seg3_BIT|Seg4_BIT|Seg5_BIT)))
#define Show1				(Seg_PORT = 0x00|((Seg1_BIT|Seg2_BIT)))
#define Show2				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg6_BIT|Seg4_BIT|Seg3_BIT)))
#define Show3				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg6_BIT|Seg3_BIT|Seg2_BIT)))
#define Show4				(Seg_PORT = 0x00|((Seg1_BIT|Seg2_BIT|Seg6_BIT|Seg5_BIT)))
#define Show5				(Seg_PORT = 0x00|((Seg0_BIT|Seg2_BIT|Seg3_BIT|Seg5_BIT|Seg6_BIT)))
#define Show6				(Seg_PORT = 0x00|((Seg5_BIT|Seg4_BIT|Seg6_BIT|Seg2_BIT|Seg3_BIT)))
#define Show7				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg2_BIT)))
#define Show8				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg2_BIT|Seg3_BIT|Seg4_BIT|Seg5_BIT|Seg6_BIT)))
#define Show9				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg2_BIT|Seg3_BIT|Seg5_BIT|Seg6_BIT)))
#define ShowA				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg2_BIT|Seg4_BIT|Seg5_BIT|Seg6_BIT)))
#define Showb				(Seg_PORT = 0x00|((Seg1_BIT|Seg2_BIT|Seg3_BIT|Seg4_BIT|Seg6_BIT)))
#define ShowC				(Seg_PORT = 0x00|((Seg0_BIT|Seg3_BIT|Seg4_BIT|Seg5_BIT)))
#define Showd				(Seg_PORT = 0x00|((Seg2_BIT|Seg3_BIT|Seg4_BIT|Seg5_BIT|Seg6_BIT)))
#define ShowE				(Seg_PORT = 0x00|((Seg0_BIT|Seg3_BIT|Seg4_BIT|Seg5_BIT|Seg6_BIT)))
#define ShowF				(Seg_PORT = 0x00|((Seg0_BIT|Seg1_BIT|Seg4_BIT|Seg5_BIT|Seg6_BIT)))


/************************************************************************/
/*                		Functions                              			*/
/************************************************************************/
void init_7SegmentDisplay(void);

#endif /* SEGMENT_DRV_H_ */

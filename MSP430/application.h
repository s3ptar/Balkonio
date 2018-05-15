/*
 * application.h
 *
 *  Created on: 22.10.2012
 *      Author: graeber
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

/************************************************************************/
/*                		Includes                                		*/
/************************************************************************/
#include	"msp430f169.h"
#include 	"Segment_drv.h"
#include 	"stdint.h"
#include 	"config.h"
#include    "stdbool.h"
/************************************************************************/
/*                		Definitions                               		*/
/************************************************************************/
enum LED_Modes {single, inv_single, count_up, count_down,
				count_toggle, flash,external};
/************************************************************************/
/*                		Functions                                		*/
/************************************************************************/
void show_time_on_7SegmentDisplay(void);
void setup_Time(uint8_t Hour, uint16_t Minute, uint16_t Seconds, uint8_t circle_Mode);
void update_time(uint8_t hour, uint8_t minute,uint8_t second, bool new_time);
void update_LED_Circle(void);
void change_circle_mode(uint8_t circle_Mode);
bool check_Volume();
void show_volume_on_7SegmentDisplay(void);
void update_LED_Circle_value(uint8_t count_value, uint8_t format);
void update_LED_Circle_count_up(uint8_t LED);

#endif /* APPLICATION_H_ */

/*
 * application.c
 *
 *  Created on: 22.10.2012
 *      Author: graeber
 */

/************************************************************************/
/*                		Includes                                		*/
/************************************************************************/
#include 	"application.h"
#include 	"time.h"
#include	"system.h"
#include	"config.h"
#include    "stdbool.h"
/************************************************************************/
/*                		Definitions                               		*/
/************************************************************************/
enum timeformat {Hour1, Hour2, Minute1, Minute2, Second1, Second2};
enum volumeformat {houndred, tens, ons};
enum count_modes {toggle, up, down, on, off};
/************************************************************************/
/*                		Constants                               		*/
/************************************************************************/
// 24..17 = Adr 16..0 Value
const uint16_t clock_val[] = {	0x4000,		// 60			adr2
								0x0001,		// 1			adr1
								0x0002,		// 2			adr1
								0x0004,		// 3			adr1
								0x0008,		// 4			adr1
								0x8000,		// 5			adr1

								0x0001,		// 6			adr2
								0x0002,		// 7			adr2
								0x0004,		// 8			adr2
								0x0008,		// 9			adr2
								0x0010,		// 10			adr2
								0x0020,		// 11			adr2
								0x0040,		// 12			adr2
								0x0080,		// 13			adr2
								0x0100,		// 14			adr2
								0x0200,		// 15			adr2

								0x0020,		// 16			adr3
								0x0040,		// 17			adr3
								0x0080,		// 18			adr3
								0x0100,		// 19			adr3
								0x0200,		// 20			adr3
								0x0400,		// 21			adr3
								0x0800,		// 22			adr3
								0x1000,		// 23			adr3
								0x2000,		// 24			adr3
								0x4000,		// 25			adr3

								0x0400,		// 26			adr4
								0x0800,		// 27			adr4
								0x1000,		// 28			adr4
								0x2000,		// 29			adr4
								0x4000,		// 30			adr4

								0x0001,		// 31			adr3
								0x0002,		// 32			adr3
								0x0004,		// 33			adr3
								0x0008,		// 34			adr3
								0x0010,		// 35			adr3 * geht nicht und wird heiﬂ

								0x0001,		// 36			adr4
								0x0002,		// 37			adr4
								0x0004,		// 38			adr4
								0x0008,		// 39			adr4
								0x0010,		// 40			adr4
								0x0020,		// 41			adr4
								0x0040,		// 42			adr4
								0x0080,		// 43			adr4
								0x0100,		// 44			adr4
								0x0200,		// 45			adr4

								0x8000,		// 46			adr1	* geht nicht
								0x0040,		// 47			adr1
								0x0080,		// 48			adr1
								0x0100,		// 49			adr1
								0x0200,		// 50			adr1
								0x0400,		// 51			adr1
								0x0800,		// 52			adr1
								0x1000,		// 53			adr1
								0x2000,		// 54			adr1
								0x4000,		// 55			adr1

								0x0400,		// 56			adr2
								0x0800,		// 57			adr2
								0x1000,		// 58			adr2
								0x2000		// 59			adr2

								};

const uint8_t adr_val[] = {	PCF8575_2,

							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,

							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,

							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,

							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,

							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,
							PCF8575_3,

							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,
							PCF8575_4,

							PCF8575_4,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,
							PCF8575_1,

							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2,
							PCF8575_2
};

/************************************************************************/
/*                		Variables	                              		*/
/************************************************************************/
extern uint8_t Uart1_Rx_Buffer[16];
uint8_t I2C_WriteBuf[I2CWriteBufferSize];
uint8_t timestate = Hour1;
uint8_t volumestate = houndred;
bool valide_command;
struct tm timestamp;
struct tm received_time;
static uint8_t act_adr = PCF8575_2;
static uint8_t old_sec;
static uint16_t time_index = LXT1CLK/LXT1CLK_RTC_Div;
static uint8_t flash_state = off;
static uint8_t count_mode = up;
static uint16_t last_PCF8575_Val[] = {0,0,0,0};
static uint8_t Led_Mode = single, new_Led_Mode = single;
static uint8_t volume_store;
extern uint32_t volume_TimeOut;
/************************************************************************
/! \fn			void change_circle_mode(uint8_t circle_Mode)
*  \brief		change the led circle mode
*  \param		uint8_t circle_Mode
*  \exception	none
*  \return		none
************************************************************************/
void change_circle_mode(uint8_t circle_Mode){
	new_Led_Mode = circle_Mode;
}

/************************************************************************
/! \fn			void setup_Time(void){uint8_t Hour, uint16_t Minute, uint16_t Seconds, uint8_t circle_Mode)
*  \brief		setup the Time
*  \param		none
*  \exception	none
*  \return		none
************************************************************************/
void setup_Time(uint8_t Hour, uint16_t Minute, uint16_t Seconds, uint8_t circle_Mode){
	uint8_t setup_loop, tmp_adr;
	timestamp.tm_sec = Seconds;
	timestamp.tm_min = Minute;
	timestamp.tm_hour = Hour;
	last_PCF8575_Val[0] = 0x0;
	last_PCF8575_Val[1] = 0x0;
	last_PCF8575_Val[2] = 0x0;
	last_PCF8575_Val[3] = 0x0;
	count_mode = up;
	flash_state = off;
	for(setup_loop=0; setup_loop < timestamp.tm_sec; setup_loop++){		//auf aktuellen stand bringen
		tmp_adr = (uint8_t)(adr_val[setup_loop]);
	}
	switch(tmp_adr){
		case PCF8575_1:{
			last_PCF8575_Val[0] |= clock_val[setup_loop];
		}
		case PCF8575_2:{
			last_PCF8575_Val[1] |= clock_val[setup_loop];
		}
		case PCF8575_3:{
			last_PCF8575_Val[2] |= clock_val[setup_loop];
		}
		case PCF8575_4:{
			last_PCF8575_Val[3] |= clock_val[setup_loop];
		}
	}
	switch(circle_Mode){
		case flash:
		case single :{
			I2C_WriteBuf[0] = 0;
			I2C_WriteBuf[1] = 0;
			write_i2c(PCF8575_1, I2C_WriteBuf, 2);
			write_i2c(PCF8575_2, I2C_WriteBuf, 2);
			write_i2c(PCF8575_3, I2C_WriteBuf, 2);
			write_i2c(PCF8575_4, I2C_WriteBuf, 2);
			break;
		}
		case inv_single :{
			I2C_WriteBuf[0] = 0xFF;
			I2C_WriteBuf[1] = 0xFF;
			write_i2c(PCF8575_1, I2C_WriteBuf, 2);
			write_i2c(PCF8575_2, I2C_WriteBuf, 2);
			write_i2c(PCF8575_3, I2C_WriteBuf, 2);
			write_i2c(PCF8575_4, I2C_WriteBuf, 2);
			break;
		}
		case count_toggle:
		case count_up :{
			count_mode = up;
			I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[0]);
			I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[0]>>8);
			write_i2c(PCF8575_1, I2C_WriteBuf, 2);
			I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[1]);
			I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[1]>>8);
			write_i2c(PCF8575_2, I2C_WriteBuf, 2);
			I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[2]);
			I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[2]>>8);
			write_i2c(PCF8575_3, I2C_WriteBuf, 2);
			I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[3]);
			I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[3]>>8);
			write_i2c(PCF8575_4, I2C_WriteBuf, 2);
			break;
		}
		case count_down :{
			I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[0]);
			I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[0]>>8);
			write_i2c(PCF8575_1, I2C_WriteBuf, 2);
			I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[1]);
			I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[1]>>8);
			write_i2c(PCF8575_2, I2C_WriteBuf, 2);
			I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[2]);
			I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[2]>>8);
			write_i2c(PCF8575_3, I2C_WriteBuf, 2);
			I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[3]);
			I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[3]>>8);
			write_i2c(PCF8575_4, I2C_WriteBuf, 2);
			break;
		}
	}
	//Zeit updaten

//	I2C_WriteBuf[0]=(uint8_t)(clock_val[Seconds]);
//	I2C_WriteBuf[1]=(uint8_t)(clock_val[Seconds]>>8);
//	act_adr = (uint8_t)(adr_val[Seconds]);
//	write_i2c(act_adr, I2C_WriteBuf, 2);
	old_sec = Seconds;
	Led_Mode = circle_Mode;
	new_Led_Mode = circle_Mode;
}
/************************************************************************
/! \fn			void show_time_on_7SegmentDisplay(void)
*  \brief		initial the 7Segment Display
*  \param		none
*  \exception	none
*  \return		none
************************************************************************/
void show_time_on_7SegmentDisplay(void){

	uint8_t temp_time;

	Dis0_Off;
	Dis1_Off;
	Dis2_Off;
	Dis3_Off;
	Dis4_Off;
	Dis5_Off;

	//anzuzeigende Zahl exrahieren
	switch(timestate){
		case Hour1:{
			temp_time = timestamp.tm_hour/10;
			break;
		}
		case Hour2:{
			temp_time = timestamp.tm_hour%10;
			break;
		}
		case Minute1:{
			temp_time = timestamp.tm_min/10;
			break;
		}
		case Minute2:{
			temp_time = timestamp.tm_min%10;
			break;
		}
		case Second1:{
			temp_time = timestamp.tm_sec/10;
			break;
		}
		case Second2:{
			temp_time = timestamp.tm_sec%10;
			break;
		}
	}
	switch(temp_time){
		case 0:{
			Show0;
			break;
		}
		case 1:{
			Show1;
			break;
		}
		case 2:{
			Show2;
			break;
		}
		case 3:{
			Show3;
			break;
		}
		case 4:{
			Show4;
			break;
		}
		case 5:{
			Show5;
			break;
		}
		case 6:{
			Show6;
			break;
		}
		case 7:{
			Show7;
			break;
		}
		case 8:{
			Show8;
			break;
		}
		case 9:{
			Show9;
			break;
		}
	}
	switch(timestate){
		case Hour1:{
			Dis0_On;
			break;
		}
		case Hour2:{
			Dis1_On;
			break;
		}
		case Minute1:{
			Dis2_On;
			break;
		}
		case Minute2:{
			Dis3_On;
			break;
		}
		case Second1:{
			Dis4_On;
			break;
		}
		case Second2:{
			Dis5_On;
			break;
		}
	}
	if (timestate>= Second2)
		timestate = Hour1;
	else
		timestate++;
}

/************************************************************************
/! \fn          void show_volume_on_7SegmentDisplay
*  \brief       print volume to 7Segment
*  \param       none
*  \exception   none
*  \return      none
************************************************************************/
void show_volume_on_7SegmentDisplay(void){

    uint8_t temp_volume;
    uint8_t scale_volume;

    Dis0_Off;
    Dis1_Off;
    Dis2_Off;
    Dis3_Off;
    Dis4_Off;
    Dis5_Off;

    //anzuzeigende Zahl exrahieren
    switch(volumestate){
        case houndred:{
            temp_volume = volume_store/100;
            break;
        }
        case tens:{
            temp_volume = (volume_store%100)/10;
            break;
        }
        case ons:{
            temp_volume = volume_store%10;
            break;
        }
    }
    switch(temp_volume){
        case 0:{
            Show0;
            break;
        }
        case 1:{
            Show1;
            break;
        }
        case 2:{
            Show2;
            break;
        }
        case 3:{
            Show3;
            break;
        }
        case 4:{
            Show4;
            break;
        }
        case 5:{
            Show5;
            break;
        }
        case 6:{
            Show6;
            break;
        }
        case 7:{
            Show7;
            break;
        }
        case 8:{
            Show8;
            break;
        }
        case 9:{
            Show9;
            break;
        }
    }
    switch(volumestate){
        case houndred:{
            Dis2_On;
            break;
        }
        case tens:{
            Dis3_On;
            break;
        }
        case ons:{
            Dis4_On;
            break;
        }
    }
    if (volumestate>= ons)
        volumestate = houndred;
    else
        volumestate++;

    change_circle_mode(count_up);
    //scale volume
    scale_volume = volume_store>>1;
    update_LED_Circle();
}

/************************************************************************
/! \fn          void update_LED_Circle_value(uint8_t count_value, uint8_t format)
*  \brief       show the value on the LED Circle
*  \param       uint8_t count_value, uint8_t format
*  \exception   none
*  \return      none
************************************************************************/
void update_LED_Circle_value(uint8_t count_value, uint8_t format){

    //Adresse bestimmen
    act_adr = (uint8_t)(adr_val[count_value]);
    switch(act_adr){
                case PCF8575_1:{
                    last_PCF8575_Val[0] |= clock_val[count_value];
                    break;
                }
                case PCF8575_2:{
                    last_PCF8575_Val[1] |= clock_val[count_value];
                    break;
                }
                case PCF8575_3:{
                    last_PCF8575_Val[2] |= clock_val[count_value];
                    break;
                }
                case PCF8575_4:{
                    last_PCF8575_Val[3] |= clock_val[count_value];
                    break;
                }
            }

}

/************************************************************************
/! \fn			void update_time(uint8_t hour, uint8_t minute,uint8_t second, bool new_time)
*  \brief		update the timestap
*  \param		uint8_t hour, uint8_t minute,uint8_t second, bool new_time
*  \exception	none
*  \return		none
************************************************************************/
void update_time(uint8_t hour, uint8_t minute,uint8_t second, bool new_time){

	//Count Down
	if(!(time_index--)||new_time){
	    //check if time set
	    time_index = LXT1CLK/LXT1CLK_RTC_Div;
	    if(!new_time){
            if(timestamp.tm_sec == 59){						//Sekunden ¸berlauf
                timestamp.tm_sec = 0;
                if(timestamp.tm_min == 59){					//Minuten ¸berlauf
                    timestamp.tm_min = 0;
                    if(timestamp.tm_hour==23)				//Stunden ¸berlauf
                        timestamp.tm_hour = 0;
                    else
                        timestamp.tm_hour++;
                }
                else
                    timestamp.tm_min++;
            }
            else
                timestamp.tm_sec++;
		}else{

		    timestamp.tm_sec = (Uart1_Rx_Buffer[7]-48)*10+(Uart1_Rx_Buffer[8]-48);
		    timestamp.tm_min = (Uart1_Rx_Buffer[4]-48)*10+(Uart1_Rx_Buffer[5]-48);
		    timestamp.tm_hour = (Uart1_Rx_Buffer[1]-48)*10+(Uart1_Rx_Buffer[2]-48);
		    valide_command = 0;

		}

	}
}

/************************************************************************
/! \fn          bool check_Volume()
*  \brief       check if volume has changed
*  \param       uint8_t new_volume
*  \exception   none
*  \return      true if change,
************************************************************************/
bool check_Volume(){

    bool return_value = false;
    uint8_t temp_volume;

    //if command receive check
    if(valide_command){

        temp_volume = ((Uart1_Rx_Buffer[10]-48)*100) + ((Uart1_Rx_Buffer[11]-48)*10) + (Uart1_Rx_Buffer[12]-48);
        if (temp_volume != volume_store){
            return_value = true;
            volume_TimeOut = _Volume_TimeOut_;
        }
        else
            return_value = false;

        volume_store = temp_volume;

    //Show volume

    }

    return return_value;

}

/************************************************************************
/! \fn			void update_LED_Circle(void)
*  \brief		update the LED Circle
*  \param		none
*  \exception	none
*  \return		none
************************************************************************/
void update_LED_Circle(void){

	if(new_Led_Mode != Led_Mode && !(time_index-2)){							//set new LedMode
		Led_Mode = new_Led_Mode;
		switch(Led_Mode){
		    case count_toggle:
		    	count_mode = up;
			case count_up:{
				I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[0]);
				I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[0]>>8);
				write_i2c(PCF8575_1, I2C_WriteBuf, 2);
				I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[1]);
				I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[1]>>8);
				write_i2c(PCF8575_2, I2C_WriteBuf, 2);
				I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[2]);
				I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[2]>>8);
				write_i2c(PCF8575_3, I2C_WriteBuf, 2);
				I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[3]);
				I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[3]>>8);
				write_i2c(PCF8575_4, I2C_WriteBuf, 2);
				break;
			}
			case count_down:{
				I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[0]);
				I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[0]>>8);
				write_i2c(PCF8575_1, I2C_WriteBuf, 2);
				I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[1]);
				I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[1]>>8);
				write_i2c(PCF8575_2, I2C_WriteBuf, 2);
				I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[2]);
				I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[2]>>8);
				write_i2c(PCF8575_3, I2C_WriteBuf, 2);
				I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[3]);
				I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[3]>>8);
				write_i2c(PCF8575_4, I2C_WriteBuf, 2);
				break;
			}
			case single:{
				I2C_WriteBuf[0]=0x00;
				I2C_WriteBuf[1]=0x00;
				write_i2c2(PCF8575_1, I2C_WriteBuf, 2);
				write_i2c2(PCF8575_2, I2C_WriteBuf, 2);
				write_i2c2(PCF8575_3, I2C_WriteBuf, 2);
				write_i2c2(PCF8575_4, I2C_WriteBuf, 2);
				break;
			}
			case inv_single:{
				I2C_WriteBuf[0]=0xFF;
				I2C_WriteBuf[1]=0xFF;
				write_i2c2(PCF8575_1, I2C_WriteBuf, 2);
				write_i2c2(PCF8575_2, I2C_WriteBuf, 2);
				write_i2c2(PCF8575_3, I2C_WriteBuf, 2);
				write_i2c2(PCF8575_4, I2C_WriteBuf, 2);
				break;
			}
		}
	}

	if(!(time_index-1)){														//reset time counter
		if(timestamp.tm_sec==59){
			if(count_mode == up)
				count_mode = down;
			else
				count_mode = up;
			last_PCF8575_Val[0] = 0x0;
			last_PCF8575_Val[1] = 0x0;
			last_PCF8575_Val[2] = 0x0;
			last_PCF8575_Val[3] = 0x0;
			switch(Led_Mode){
				case count_up:{
					I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[0]);
					I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[0]>>8);
					write_i2c(PCF8575_1, I2C_WriteBuf, 2);
					I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[1]);
					I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[1]>>8);
					write_i2c(PCF8575_2, I2C_WriteBuf, 2);
					I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[2]);
					I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[2]>>8);
					write_i2c(PCF8575_3, I2C_WriteBuf, 2);
					I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[3]);
					I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[3]>>8);
					write_i2c(PCF8575_4, I2C_WriteBuf, 2);
					break;
				}
				case count_down:{
					I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[0]);
					I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[0]>>8);
					write_i2c(PCF8575_1, I2C_WriteBuf, 2);
					I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[1]);
					I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[1]>>8);
					write_i2c(PCF8575_2, I2C_WriteBuf, 2);
					I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[2]);
					I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[2]>>8);
					write_i2c(PCF8575_3, I2C_WriteBuf, 2);
					I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[3]);
					I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[3]>>8);
					write_i2c(PCF8575_4, I2C_WriteBuf, 2);
					break;
				}
			}
		}
		switch(Led_Mode){
			case single:{
				I2C_WriteBuf[0]=0x00;
				I2C_WriteBuf[1]=0x00;
				write_i2c2(act_adr, I2C_WriteBuf, 2);
				break;
			}
			case inv_single:{
				I2C_WriteBuf[0]=0xFF;
				I2C_WriteBuf[1]=0xFF;
				write_i2c2(act_adr, I2C_WriteBuf, 2);
				break;
			}
			case flash :{
				if (flash_state == off){
					I2C_WriteBuf[0]=0x00;
					I2C_WriteBuf[1]=0x00;
					flash_state = on;
				}
				else{
					I2C_WriteBuf[0]=0xFF;
					I2C_WriteBuf[1]=0xFF;
					flash_state = off;
				}
				write_i2c(PCF8575_1, I2C_WriteBuf, 2);
				write_i2c(PCF8575_2, I2C_WriteBuf, 2);
				write_i2c(PCF8575_3, I2C_WriteBuf, 2);
				write_i2c(PCF8575_4, I2C_WriteBuf, 2);
				break;
			}
		}
	}

	if(timestamp.tm_sec != old_sec){						//Count time
		act_adr = (uint8_t)(adr_val[timestamp.tm_sec]);
		switch(act_adr){
			case PCF8575_1:{
				last_PCF8575_Val[0] |= clock_val[timestamp.tm_sec];
				break;
			}
			case PCF8575_2:{
				last_PCF8575_Val[1] |= clock_val[timestamp.tm_sec];
				break;
			}
			case PCF8575_3:{
				last_PCF8575_Val[2] |= clock_val[timestamp.tm_sec];
				break;
			}
			case PCF8575_4:{
				last_PCF8575_Val[3] |= clock_val[timestamp.tm_sec];
				break;
			}
		}
		switch(Led_Mode){
			case single:{
				I2C_WriteBuf[0]=(uint8_t)(clock_val[timestamp.tm_sec]);
				I2C_WriteBuf[1]=(uint8_t)(clock_val[timestamp.tm_sec]>>8);
				act_adr = (uint8_t)(adr_val[timestamp.tm_sec]);
				write_i2c2(act_adr, I2C_WriteBuf, 2);
				break;
			}
			case inv_single:{
				I2C_WriteBuf[0]=(uint8_t)(~clock_val[timestamp.tm_sec]);
				I2C_WriteBuf[1]=(uint8_t)(~clock_val[timestamp.tm_sec]>>8);
				act_adr = (uint8_t)(adr_val[timestamp.tm_sec]);
				write_i2c2(act_adr, I2C_WriteBuf, 2);
				break;
			}
			case count_up :{
				switch (act_adr){
					case PCF8575_1:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[0]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[0]>>8);
						break;
					}
					case PCF8575_2:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[1]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[1]>>8);
						break;
					}
					case PCF8575_3:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[2]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[2]>>8);
						break;
					}
					case PCF8575_4:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[3]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[3]>>8);
						break;
					}
				}
				break;
			}
			case count_down :{
				switch (act_adr){
					case PCF8575_1:{
						I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[0]);
						I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[0]>>8);
						break;
					}
					case PCF8575_2:{
						I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[1]);
						I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[1]>>8);
						break;
					}
					case PCF8575_3:{
						I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[2]);
						I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[2]>>8);
						break;
					}
					case PCF8575_4:{
						I2C_WriteBuf[0]=(uint8_t)(~last_PCF8575_Val[3]);
						I2C_WriteBuf[1]=(uint8_t)(~last_PCF8575_Val[3]>>8);
						break;
					}
				}
				break;
			}
			case count_toggle :{
				switch (act_adr){
					case PCF8575_1:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[0]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[0]>>8);
						break;
					}
					case PCF8575_2:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[1]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[1]>>8);
						break;
					}
					case PCF8575_3:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[2]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[2]>>8);
						break;
					}
					case PCF8575_4:{
						I2C_WriteBuf[0]=(uint8_t)(last_PCF8575_Val[3]);
						I2C_WriteBuf[1]=(uint8_t)(last_PCF8575_Val[3]>>8);
						break;
					}
				}
				if(count_mode == down){
					I2C_WriteBuf[0] = ~I2C_WriteBuf[0];
					I2C_WriteBuf[1] = ~I2C_WriteBuf[1];
				}
				break;
			}

		}
		write_i2c2(act_adr, I2C_WriteBuf, 2);
		old_sec = timestamp.tm_sec;
	}
}

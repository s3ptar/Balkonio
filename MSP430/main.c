

/************************************************************************/
/*                      Includes                                        */
/************************************************************************/
#include    "msp430f169.h"
#include    "Segment_drv.h"
#include    "application.h"
#include    "system.h"
#include    "config.h"
#include    "time.h"
#include    "PinConfig.h"
#include    "stdbool.h"
/************************************************************************/
/*                      Global Variables                                */
/************************************************************************/
uint8_t volume = 0;
//extern bool syncDCF;
extern struct tm timestamp;
extern struct tm dcf_time;
extern bool valide_command;
uint8_t old_hour, last_minute;
bool sync_comlete;
bool update_cycle = false;
void SendUSART1(char* str) ;
uint32_t volume_TimeOut = 0;
/************************************************************************
/! \fn          void main(void)
*  \brief       mainfunction
*  \param       none
*  \exception   none
*  \return      none
************************************************************************/
void main(void) {

//  uint8_t I2C_WriteBuf[I2CWriteBufferSize];
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P3SEL |= 0x0A;                            // Assign pins to module function
    BCSCTL1 &= ~XT2OFF;                       // Enable X2
    BCSCTL2 |= SELS;                          // Set SMCLK as XT2

    I2CTCTL |= I2CSSEL_2;                     // Set I2C CLk as SMCLK

    //Initialisieren
    init_7SegmentDisplay();                   // Init 7 Segment
    setup_Time(19,49,0,flash);
    old_hour = 0;
    last_minute = 0;
    //Init UART
    P3SEL |= BIT6 +BIT7;                        // P3.6,8 = USART0
    //P3DIR |= BIT7 ;                           //Rx Pin Inpute direction
    U1CTL |= CHAR+SWRST;                        // 8-bit character,
    UTCTL1 |= SSEL1 + SSEL0;                    // UCLK = SMCLK = LXT2CLK = 7372800
    UBR01 = 0x80;                               // 19200 baud from smclk0K
    UBR11 = 0x01;                               //
    UMCTL1 = 0x00;                              // No modulation
    ME2 |= (UTXE1 + URXE1);                     // Enabled USART1 TXD/RXD
    U1CTL &= ~SWRST;
    IE2 |= URXIE1;                        // Enable USART1 RX
    IFG2 &= ~UTXIFG1;                     // Clear inital flag on POR

    TACTL = TASSEL_1 + MC_1 + TAIE;           // ACLK = 32kHz, UpMode, interrupt
    TACCR0 = LXT1CLK_RTC_Div;                 // Set Timer to 1ms
    valide_command = false;


    /**********************************************
     *          Main loop                         *
    * ********************************************/

    while(1){

        /**********************************************
         *          update time               *
         * ********************************************/
        if(update_cycle){
            if(!check_Volume() && volume_TimeOut == 0){
                //no change and timer down, show time
                update_time(0,0,0,valide_command);
                show_time_on_7SegmentDisplay();
                update_LED_Circle();
            }
            else{
                show_volume_on_7SegmentDisplay();
                volume_TimeOut--;

            }

            update_cycle = false;
        }

        /**********************************************
         *          change display mode               *
         * *******************************************
        if(last_minute != timestamp.tm_min){
            last_minute = timestamp.tm_min;

            if(timestamp.tm_min > 1 && timestamp.tm_min < 13)
                change_circle_mode(single);
            else if(timestamp.tm_min > 12 && timestamp.tm_min < 25)
                change_circle_mode(inv_single);
            else if(timestamp.tm_min > 24 && timestamp.tm_min < 37)
                change_circle_mode(count_up);
            else if(timestamp.tm_min > 36 && timestamp.tm_min < 49)
                change_circle_mode(count_down);
            else
                change_circle_mode(count_toggle);

        }*/
    }

}





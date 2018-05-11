/*
 * interrupt.c
 *
 *  Created on: 22.10.2012
 *      Author: graeber
 */

/************************************************************************/
/*                		Includes                                		*/
/************************************************************************/
#include 	"msp430f169.h"
#include 	"application.h"
#include 	"config.h"
#include	"system.h"
#include    "stdbool.h"
/************************************************************************/
/*                		Global Variables                           		*/
/************************************************************************/
const uint8_t Max_RX1_Buffer_Size = 16;
extern uint8_t *I2C_WriteBufptr;
extern bool valide_command;
extern bool update_cycle;
extern struct tm received_time;
uint8_t Uart1_RX_Cnt = 0;
uint8_t Uart1_Rx_Buffer[16];
//extern bool checkDCF;
uint8_t I2C_WriteCounter = 0;


/************************************************************************
/! \fn			#pragma vector=TIMERA1_VECTOR
*				__interrupt void Timer_A(void
*  \brief		Timer_A3 Interrupt Vector (TAIV) handler
*  \param		none
*  \exception	none
*  \return		none
************************************************************************/
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A(void)
{
  switch( TAIV )
  {
    case  2:  break;                        // CCR1 not used
    case  4:  break;                        // CCR2 not used
    case 10:  {								// overflow

    	update_cycle = true;
    	break;
    }

  }
}


/************************************************************************
/! \fn			#pragma vector=USART0TX_VECTOR
*				__interrupt void I2C_ISR(void)
*  \brief		// Common ISR for I2C Module
*  \param		none
*  \exception	none
*  \return		none
************************************************************************/
#pragma vector=USART0TX_VECTOR
__interrupt void I2C_ISR(void)
{
	static unsigned int count = 0;              // Counter to determine CtlByte
	                                            // or MSByte or LSByte Transfer
	switch( I2CIV ){
		case 12:{                               // TXRDYIFG
			I2CDRB = I2C_WriteBufptr[count++];  // Transmit Control Byte
			if(count == I2CNDAT){
				count = 0;
				I2CIE &= ~TXRDYIE;
				LPM0_EXIT;                             // CPU Active on ISR exit
			}
			break;
	   }
		case  2: break;                         // Arbitration lost
		case  4: break;                         // No Acknowledge
		case  6: break;                         // Own Address
		case  8: break;						    // Register Access Ready
		case 10: break;                         // Receive Ready
		case 14: break;                         // General Call
		case 16: break;
	}
}

/************************************************************************
/! \fn          #pragma vector = UART1RX_VECTOR
*               __interrupt void Usart1_Rx(void)
*  \brief       // Common ISR for UART1
*  \param       none
*  \exception   none
*  \return      none
************************************************************************/
#pragma vector=USART1RX_VECTOR
 __interrupt void Usart1_Rx(void){
     valide_command = true;
     Uart1_RX_Cnt++;
     //Check overflow
     if(Uart1_RX_Cnt == Max_RX1_Buffer_Size)
         Uart1_RX_Cnt = 0;
     if(RXBUF1 == "t")
         Uart1_RX_Cnt = 0;
     if(RXBUF1 == 0x0d){

         ////String to parse 16:46:05, 20<\r>
         //check if cmd okay
         if(Uart1_RX_Cnt == 13){
             valide_command = true;
             //update_time(((Uart1_Rx_Buffer[Uart1_RX_Cnt]-48)*10+((Uart1_Rx_Buffer[Uart1_RX_Cnt]-48))),0,0,1);

         }
         Uart1_RX_Cnt = 0;
     }else{
         Uart1_Rx_Buffer[Uart1_RX_Cnt] = RXBUF1;
     }

}



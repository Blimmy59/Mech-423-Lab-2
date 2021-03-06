#include <msp430.h> 


/**
 * main.c
 */

volatile signed int queue[50] ; //empty Q array of signed ints
volatile unsigned int start = 0;             //start index
volatile unsigned int end = 0;               //end index
volatile unsigned int empty = 1;             //starts off empty
volatile unsigned int full = 0;             //starts off empty
char empty_error[16]  = "Queue is empty. ";
char full_error[15] = "Queue is full. ";

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
    BCSCTL1 = XT2OFF + XTS;               // XT2 off, XT1 high freq
    BCSCTL2 = SELM0 + SELM1 + SELS;       // XT1 -> MCLK, XT1 -> SMCLK
    BCSCTL3 = LFXT1S1;                    // XT1 freq 3-16 MHz

    P1DIR = BIT7;                         // port 1.7 pins as output
    P3SEL = BIT4 | BIT5;                  // Set up ports 3.4 and 3.5 as UART ports

    UCA0CTL1 = UCSSEL1;                   // Use SMCLK for UART
    UCA0BR0 = 1666 & 0xFF;                // Set baud rate
    UCA0BR1 = 1666 >> 8;
    UCA0MCTL = UCBRS_6;                   // Set modulation bits
    IE2 |= UCA0RXIE;                      // Enable Rx interrupt

    _EINT();                              // Global interrupt enable

    while (1){}                           //infinite loop
}



//ISR
#pragma vector=USCIAB0RX_VECTOR         // defined in "msp430x22x4.h"
__interrupt void UART_RX(void)          // interrupt on incoming data
{
  volatile unsigned int i;              //volatile to prevent optimization

  unsigned char rx_byte;
  rx_byte = UCA0RXBUF;                  // grab byte from RX buffer

  //if its a carriage return
  if(rx_byte == 0xD)
  {
      if (empty==1) //if it's empty
      {
          //don't pop
          //ERROR MESSAGE
          volatile unsigned int i=0;
          for (i; i<16; i++)
          {
              while((IFG2 & UCA0TXIFG) == 0);
              {
                  UCA0TXBUF = empty_error[i];
              }
          }
      }
      else
      {
          while((IFG2 & UCA0TXIFG) == 0);   //wait for TX buffer to be clear (UCA0TXIFG=1 when interrupt pending)
          {
              UCA0TXBUF = queue[start];     //POP front byte
          }

          if (start == end)                 //if just read byte from shared index
          {
              empty = 1;                    //it's now empty
          }
          else                              //if it's not empty
          {
              start=start+1;                //increment start
          }
          if (start == (50))                //make circular
              start = 0;                    //by looping back to front
          full = 0;
      }
  }

  //ELSE ENQUEUE
  else
  {
      if (empty==1 | full == 1)
      {
          //empty = 0;                //just added something, no longer empty, want to stay at same index
      }
      else
      {
          end= end+1;                    //if it wasn't empty, just increment
      }
      if (end == (50))              //make circular
      {
          end = 0;                  //by looping back to front
      }
      if (full==1)
      {
          //ERROR MESSAGE FULL
          volatile unsigned int j=0;
          for (j; j<15; j++)
          {
              while((IFG2 & UCA0TXIFG) == 0);
                  {UCA0TXBUF = full_error[j];}
          }
      }
      else
      {
          queue[end] = rx_byte;         //add byte at current index
          empty = 0; //not empty
      }
      if (start==end+1)
          full = 1;
      if (start==0 && end == 49)
          full = 1;
  }
}

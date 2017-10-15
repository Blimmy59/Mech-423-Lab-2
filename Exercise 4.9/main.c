#include "msp430x22x4.h"

/*This code echoes back the input character in PuTTY, as well as the next character in the ASCII table.
 * Additionally, if a "j" is inputed, LED3 turns on. If a "k" is inputed, LED3 turns off. Putty baud rate must be set to 57554
 * Set baud rate to 57554 ***IN PUTTY****
 */


void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  BCSCTL1 = XT2OFF + XTS;               // XT2 off, XT1 high freq
  BCSCTL2 = SELM0 + SELM1 + SELS;       // XT1 -> MCLK, XT1 -> SMCLK
  BCSCTL3 = LFXT1S1;                    // XT1 freq 3-16 MHz

  P1DIR = BIT7;                         //port 1.7 pins as output
  P3SEL = BIT4 | BIT5;                  // Set up ports 3.4 and 3.5 as UART ports

  UCA0CTL1 = UCSSEL1;                   // Use SMCLK for UART
  UCA0BR0 = 277 & 0xFF;                // Set baud rate to 57.6kHz ***SET THIS RATE IN PUTTY****
  UCA0BR1 = 277 >> 8;
  UCA0MCTL = UCBRS_6;                   // Set modulation bits
  IE2 |= UCA0RXIE;                      // Enable Rx interrupt

  _EINT();                              // Global interrupt enable

  while (1);
}

#pragma vector=USCIAB0RX_VECTOR         // defined in "msp430x22x4.h"
__interrupt void UART_RX(void)          // interrupt on incoming data
{
  volatile unsigned int w;              //volatile to prevent optimization
  unsigned char rx_byte;
  rx_byte = UCA0RXBUF;                  // grab byte from RX buffer

  //if its a j
  if(rx_byte == 0b1101010)
      P1OUT = BIT7;                     //turn on LED3

  //if its a k
  if(rx_byte == 0b1101011)
      P1OUT = 0b00000000;                     //turn off LED3



  while((IFG2 & UCA0TXIFG) == 0);       // wait for TX buffer to be clear
  {
      UCA0TXBUF = rx_byte;                  // Echo the byte back
      //Delay

      w = 2500;

      do w--;
      while (w != 0);
      //output next character in ASCII table
      UCA0TXBUF = (rx_byte+1);                 // send that byte

  }
}

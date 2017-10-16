#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
// Clock Stuffs (16MHz setup)
	BCSCTL1 = XT2OFF + XTS;               // XT2 off, XT1 high freq
	BCSCTL2 = SELM0 + SELM1 + SELS;       // XT1 -> MCLK, XT1 -> SMCLK
	BCSCTL3 = LFXT1S1;                    // XT1 freq 3-16 MHz

// Analog Stuffs
	ADC10AE1 = BIT5 + BIT6 + BIT7;        // Setup Analog input on P4.4, P4.5, and P4.6

// I/O Setup
	P3SEL = BIT4 + BIT5;         // Set up ports 3.4 and 3.5 as UART ports

// UART Stuffs
	UCA0CTL1 = UCSSEL1;          // Use SMCLK for UART (Page 430)
	UCA0BR0 = 104;       // Set baud rate (Table 15-4 on page 425)
	UCA0BR1 = 0b00000000;
	UCA0MCTL = UCBRF0 + UCBRF1 + UCOS16;          // Set modulation bits (Page 431) Set UCOS16 for Oversampling table on page 425




// Interrupt Stuffs
	IE2 |= UCA0RXIE;                      // Enable UCA0 Rx interrupt

	_EINT();



	return 0;
}

#include <msp430.h> 

// Exercise 3

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

        //Clock Setup (16MHz setup)
        DCOCTL = DCO1 + DCO2; // DCO7 for 500kHz
        BCSCTL1 = XT2OFF + XTS + RSEL2;               // XT2 off, XT1 high freq
        BCSCTL2 = SELM0 + SELM1 + DIVS1; // SELS;       // XT1 -> MCLK, XT1 -> SMCLK
        BCSCTL3 = LFXT1S1;                    // XT1 freq 3-16 MHz

        // Port Setup
        P2SEL = BIT1;
        P2DIR = BIT1;
        P1DIR   = 0b11111110;           //Sets P1.1 to 1.7 to outputs
        P1OUT   = 0b10010010;           //Sets pins 1,4,7 to high

        // Interrupt Enable
        P1IE = 0b00000001; // Enable P1.0 interrupt
        P1IES = 0b00000000; // BIT0 = 0, Rising Edge
        _EINT(); // Global interrupt enable

        while(1){
                P1OUT ^= 0b10000000;    //Toggles P1.7
                volatile unsigned int i = 0;
                while(i < 30000){
                    i++;
                }
            }
    return 0;
}

// Interrupt Service Routine (ISR)
#pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void){
        P1OUT ^= 0b00000010;

        P1IFG = 0b00000000;
    }

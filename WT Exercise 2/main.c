#include <msp430.h> 

// Exercise 2

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
        P1DIR   = 0b11111111;           //Sets P1.1 to 1.7 to outputs
        P1OUT   = 0b10010011;           //Sets pins 0,1,4,7 to high

        while(1){
                P1OUT ^= 0b10000000;    //Toggles P1.7
                volatile unsigned int i = 0;
                while(i < 30000){
                    i++;
                }
            }
    return 0;
}

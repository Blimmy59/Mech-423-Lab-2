#include <msp430.h> 

// Exercise 5

/**
 * main.c
 */

    volatile unsigned int toggle = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

        // Clock Stuffs (16MHz setup)
        BCSCTL1 = XT2OFF + XTS;               // XT2 off, XT1 high freq
        BCSCTL2 = SELM0 + SELM1 + SELS;       // XT1 -> MCLK, XT1 -> SMCLK
        BCSCTL3 = LFXT1S1;                    // XT1 freq 3-16 MHz

        // I/O Setup
        P1DIR   = 0b01100000;           // Sets P1.1 to 1.7 to Outputs, and sets P1.0 to an input
        P1OUT   = 0b00000000;           // Initializes all outputs to zero.

        // Timer A Setup
        TACTL = TASSEL_2 + MC_1 + TAIE;     // Specify SMCLK as source, set up mode, enable interrupt
        TACCTL0 = CCIE;                 // Enables interrupt, Sets output to high
        TACCR0 = 0b11111010000000;     // 16,000 in binary
        TACCR1 = 0b111110100000;      // 4,000
        TACCTL1 = CCIE + OUTMOD_4;

        // Interrupt Enable
        _EINT();

        while(1){}
    return 0;
}
//Timer A0 ISR
#pragma vector = TIMERA0_VECTOR
__interrupt void TIMER_A0 (void){
    P1OUT ^= BIT5;            //toggles P1.5
    if (toggle == 0){
        P1OUT |= BIT6;        //Sets P1.6 to high
    }
    toggle ^= BIT0;         //Toggle the toggle

    TACTL ^= TAIFG;
}

//Timer A1 ISR
#pragma vector = TIMERA1_VECTOR
__interrupt void TIMER_A1 (void){
    P1OUT &= ~BIT6;
    TACCTL1 ^= CCIFG;
}

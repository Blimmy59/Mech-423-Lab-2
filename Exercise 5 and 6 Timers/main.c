#include <msp430.h> 
/**
 * main.c
 */
    int toggle = 0;
    int risingToggle = 0b0;
    int pulseWidth = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    //Sets clock configuration
    BCSCTL1 = 0b11000101;           //Sets XT2 to off, XTS to High-Frequency Mode, DIVAx to divide by 1, and RSELx to 5
    BCSCTL2 = 0b11000100;
    BCSCTL3 = LFXT1S_3;
    DCOCTL  = 0b00100000;           //DCOx = 1, and MODx = 0

    // I/O setup
    P2DIR   = 0b00000011;           // Sets P2.0 and P2.1 to an output
    P2SEL   = 0b00000011;           // Sets P2.0 to output ACLK and sets P2.1 to output SMCLK

    P1DIR   = 0b01100000;           // Sets P1.1 to 1.7 to Outputs, and sets P1.0 to an input
    P1OUT   = 0b00000000;           // Initializes all outputs to zero.

    P4DIR   = 0b00000000;           // Set P4.0 to take TB0
    P4SEL   = 0b00000001;           // Set P4.0 to take in TB0

	//Timer A Setup
    TACTL = TASSEL_2|MC_1|TAIE;     // Specify SMCLK as source, set up mode, enable interrupt
	TACCTL0 = CCIE;                 // Enables interrupt, Sets output to high
    TACCR0 = 0b1111011;             // 123 in binary
    TACCR1 = 0b11111;               // 31
    TACCTL1 = CCIE|OUTMOD_4;

    //Timer B Setup
    TBCTL = TBSSEL_2 | MC_2; // Connect to SMCLK, and set Timer B in continuous count mode
    TBCCTL0 = CAP + CM0 + CCIE; // Set to capture mode, and rising edge capture, input to CCIxB, enable interrupt

    _EINT();                        //Global interrupt enable

	while(1){

	}
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

#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B0(void)
{
    risingToggle ^= BIT0;
    if (risingToggle == 1){
        //Start timer & set to falling edge capture
        TBCTL |= TBCLR;
        TBCCTL0 &= ~CM0;
        TBCCTL0 |= CM1;

    } else{
        //Record Value & set to rising edge capture
        pulseWidth = TBR;
        TBCCTL0 &= ~CM1;
        TBCCTL0 |= CM0;
    }

}

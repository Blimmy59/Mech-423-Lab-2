#include <msp430.h> 

volatile unsigned int rising_detect_flag = 1;
volatile unsigned int rising_count = 0;
volatile unsigned int time_interval = 0;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT5 + BIT6;                        // Set P1.5 and P1.6 to output direction
    P4DIR &= ~BIT0;                             // Set P4.0 to input direction

    BCSCTL1 = XT2OFF + XTS;               // XT2 off, XT1 high freq
    BCSCTL2 = SELM0 + SELM1 + SELS;       // XT1 -> MCLK, XT1 -> SMCLK
    BCSCTL3 = LFXT1S1;                    // XT1 freq 3-16 MHz

    TACTL = TASSEL1 + MC0; // Connect to SMCLK, and set Timer A in up count mode
    TACCR0 = 0x7D00; // Set register0 to 32000
    TACCR1 = 0x7080; // 1:31680->7BC0, 10%:28800->7080, 25%:24000->5DC0, 50%:16000->3E80, 75%:8000->1F40, 90%:3200->C80
    TACCTL0 = OUTMOD2;
    TACCTL1 = OUTMOD1 + OUTMOD0;

    TBCTL = TBSSEL1 + MC1; // Connect to SMCLK, and set Timer B in continuous count mode
    TBCCTL0 = CAP + CM0 + CCIE; // Set to capture mode, and rising edge capture, input to CCIxB, enable interrupt

    P1SEL |= BIT5 + BIT6; // Set P1.5 to give out TA0, and P1.6 to give out TA1
    P4SEL |= BIT0; // Set P4.0 to take in TB0

    _EINT();

    while (1);

    return 0;
}

// Timer B capture interrupt service routine
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B0(void)
{
    if (rising_detect_flag == 1) {
        rising_count = TBCCR0;
        TBCCTL0 &= ~CM0;
        TBCCTL0 |= CM1;
        rising_detect_flag = 0;
    }
    else {
        if (TBCCTL0 & COV == COV) {
            time_interval = rising_count - TBCCR0 - 1;
            TBCCTL0 &= ~COV;
        }
        else {
            time_interval = TBCCR0 - rising_count - 1;
        }

        if (time_interval > TACCR0) {
            time_interval -= TACCR0 + 1;
        }

        TBCCTL0 |= CM0;
        TBCCTL0 &= ~CM1;
        rising_detect_flag = 1;
    }
    TBCCTL0 &= ~CCIFG;
}

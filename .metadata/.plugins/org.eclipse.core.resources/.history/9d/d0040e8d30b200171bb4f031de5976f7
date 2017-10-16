#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
	
	//Sets clock configuration
	BCSCTL1 = 0b11110101;           //Sets XT2 to off, XTS to High-Frequency Mode, DIVAx to divide by 1, and RSELx to 5
	BCSCTL2 = 0b11000100;
	BCSCTL3 = 0b00000000; //LFXT1S_3;
	DCOCTL  = 0b00100000;           //DCOx = 1, and MODx = 0


	//Pin Setup
	P2DIR   = 0b00000011;           //Sets P2.0 and P2.1 to an output
	P2SEL   = 0b00000011;           //Sets P2.0 to output ACLK and sets P2.1 to output SMCLK

	P1DIR   = 0b11111110;           //Sets P1.1 to 1.7 to Outputs, and sets P1.0 to an input
	P1OUT   = 0b10010010;           //Sets "1" high

	//Interrupt stuff
	P1IES = 0b00000000;             //Sets Rising edge on all interruptible inputs
	P1IE = 0b00000001;              //Enables interrupt on P1.0
	_EINT();                        //Global interrupt enable

	//Main Loop
	while(1){
	    P1OUT ^= 0b10000000;
	    volatile unsigned int i = 0;
	    while(i < 30000){
	        i++;
	    }
	}
}
	//P1.0 interrupt service routine
#pragma vector = PORT1_VECTOR
	__interrupt void Port_1(void){
	    P1OUT ^= 0b00000010;

	    P1IFG = 0b00000000;
	}

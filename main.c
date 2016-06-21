/*
 * Here we are using MSP430G2553
 * Description: proper implimentation of the MSP430 Timer module.
 * In this code I amusing two capture compare registers to generate time delay,
 *  and also keeeping the CPU in low power mode for saving the battery.
 *  The code is perfectly working and tested, using MSP430UIF FET and MSP430G2 Launchpad on Mac OS X
 */
#include <msp430.h>
unsigned int count=0;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 |= DIVA_3;                        // ACLK/8
  BCSCTL3 |= LFXT1S_2;                      // ACLK = VLO
  //WDTCTL = WDT_ADLY_1000;                   // Interval timer
  //IE1 |= WDTIE;                             // Enable WDT interrupt
  P1DIR = 0xFF;                             // All P1.x outputs
  P1OUT = 0;                                // All P1.x reset
  P2DIR = 0xFF;                             // All P2.x outputs
  P2OUT = 0;                                // All P2.x reset
  // from timer
  CCTL0 =  CCIE;                  // CCR0 toggle, interrupt enabled
  CCTL1 =  + CCIE;                  // CCR1 toggle, interrupt enabled
  //CCTL2 = OUTMOD_4 + CCIE;                  // CCR1 toggle, interrupt enabled
  TACTL = TASSEL_1 + ID_3 + MC_2;                  // ACLK, Timer A input divider: 3 - /8 ,Continous up

  //P1OUT |= BIT0 ;
  CCR0 = 20;
  CCR1 = 30;

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
} //Main end

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)

{
  P1OUT ^= BIT0 ;
  CCR0 = TAR + 20 ;                              // Add Offset to CCR1
}

// Timer_A2 Interrupt Vector (TA0IV) handler for CCR1 and CCR2
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1(void)


{
  switch( TA0IV )
  {
  case  2:
	  //CCR1 += 1000 ;                    // Add Offset to CCR1
	  P1OUT ^= BIT6 ;
	  CCR1 = TAR + 30   ;

           break;
  case  4:
	  //CCR2 += 1000;                    // Add Offset to CCR2
             break;
  case 10:
	  //P1OUT ^= 0x01;                   // Timer_A3 overflow
           break;
 }
}

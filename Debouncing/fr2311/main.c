
// Include all the msp430 defines
#include <msp430.h>

// define what pin the led is connected to
#define LED1_PIN BIT0
#define BUTTON_PIN BIT1

#define LED2_PIN BIT0

// Button interrupt
// Needed for gcc compiler
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void) {
    P1IFG &= ~BUTTON_PIN;

    // button pressed
    // Need to start timer

    // reset timer
    TB0R = 0;

    // Start timer in continuous mode
    TB0CTL &= ~MC_3;
    TB0CTL |= MC_2;
}

// Timer A interrupt
// CCR0 only
void __attribute__((interrupt(TIMER0_B0_VECTOR))) Timer_B0 (void) {
    TB0CTL &= ~TBIFG;

    // Stop timer
    TB0CTL &= ~MC_3;

    if (!(P1IN & BUTTON_PIN)) {
        // Toggle led output
        P1OUT ^= LED1_PIN;
    }
}

// the rest of the interrupts
void __attribute__((interrupt(TIMER0_B1_VECTOR))) Timer_B1 (void) {

    // need to touch T0IV to reset interrupt flag
    switch (TB0IV) {
        case 2: // CCR1
            __no_operation();
            break;
        case 4: // CCR2
            break;
        case 10: // Overflow
            break;
    }
}

void setup_watchdog() {
    // disable the watchdog
    WDTCTL = WDTPW | WDTHOLD;
}

void setup_led() {

    // set the correct pin on port 1 to be an output for the led
    P1DIR |= LED1_PIN;

    // turn off the led output
    P1OUT &= ~LED1_PIN;
}

void setup_button() {

    // set button to input
    P1DIR &= ~BUTTON_PIN;

    // enable pullup resistor
    P1REN |= BUTTON_PIN;
    P1OUT |= BUTTON_PIN;

    // set button to use peripheral function
    //P1SEL |= BUTTON_PIN;

    // clear interrupt flag
    P1IFG = 0x00;

    // set interrupt slope to falling
    P1IES |= BUTTON_PIN;

    // Enable interrupt for button pin
    P1IE |= BUTTON_PIN;
}

void setup_timer() {

    // Select clock source to SMCLK
    TB0CTL |= TBSSEL_2;

    // Divide SMCLK by 8
    //BCSCTL2 |= DIVS_3;

    // Set to continuous mode
    TB0CTL |= MC_2;

    // divide timer clock by 8
    //TB0CTL |= ID_3;

    // Set capture/compare 0 to 1999
    // Sets delay to 2ms
    TB0CCR0 = 0x1999;

    // Enable CCR0 interrupt
    TB0CCTL0 |= CCIE;

    // Enable Timer A0 interrupts
    TB0CTL |= TBIE;
}

int main() {
    // run setup functions above
    setup_watchdog();
    setup_led();
    setup_button();
    setup_timer();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Enable global interrupt flag, enter low power mode
    _BIS_SR(GIE);

    // Enter infinite loop
    // since there is nothing to exit to, there is no point
    // in not having an infinite loop
    while(1) {
        __no_operation();
    }
}

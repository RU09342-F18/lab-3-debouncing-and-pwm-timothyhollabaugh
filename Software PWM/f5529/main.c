
// Include all the msp430 defines
#include <msp430.h>

// define what pin the led is connected to
#define LED1_PIN BIT0
#define BUTTON_PIN BIT1

#define LED2_PIN BIT7

// Button interrupt
// Needed for gcc compiler
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void) {
    P1IFG &= ~BUTTON_PIN;

    // button pressed
    // Need to start timer

    // reset timer
    TA1R = 0;

    // Start timer in continuous mode
    TA1CTL &= ~MC_3;
    TA1CTL |= MC_2;
}

// Timer A1 interrupt
// CCR0 only
void __attribute__((interrupt(TIMER1_A0_VECTOR))) Timer1_A0 (void) {
    TA1CTL &= ~TAIFG;

    // Stop timer
    TA1CTL &= ~MC_3;

    if ((P1IES & BUTTON_PIN) && !(P1IN & BUTTON_PIN)) {
        // Toggle led output
        P1OUT |= LED1_PIN;

        // Toggle detect slope
        P1IES &= ~BUTTON_PIN;

        // Adjust led brightness
        if (TA0CCR1 <= 900) {
            TA0CCR1 += 100;
            TA0CTL |= MC_1;
        } else {
            TA0CCR1 = 0;
            TA0CTL &= ~MC_3;
            P4OUT &= ~LED2_PIN;
        }
    }

    if (!(P1IES & BUTTON_PIN) && (P1IN & BUTTON_PIN)) {
        // Toggle led output
        P1OUT &= ~LED1_PIN;

        // Toggle detect slope
        P1IES |= BUTTON_PIN;
    }
}

// the rest of the interrupts
void __attribute__((interrupt(TIMER1_A1_VECTOR))) Timer1_A1 (void) {

    // need to touch T0IV to reset interrupt flag
    switch (TA1IV) {
        case 2: // CCR1
            __no_operation();
            break;
        case 4: // CCR2
            break;
        case 10: // Overflow
            break;
    }
}

// Timer A0 interrupt
// CCR0 only
void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer0_A0 (void) {
    TA0CTL &= ~TAIFG;

    P4OUT |= LED2_PIN;
}

// the rest of the interrupts
void __attribute__((interrupt(TIMER0_A1_VECTOR))) Timer0_A1 (void) {

    // need to touch T0IV to reset interrupt flag
    switch (TA0IV) {
        case 2: // CCR1
            P4OUT &= ~LED2_PIN;
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
    P4DIR |= LED2_PIN;

    // turn off the led output
    P1OUT &= ~LED1_PIN;
    P4OUT &= ~LED2_PIN;
}

void setup_button() {

    // set button to input
    P1DIR &= ~BUTTON_PIN;

    // enable pullup
    P1REN |= BUTTON_PIN;
    P1OUT |= BUTTON_PIN;

    // clear interrupt flag
    P1IFG = 0x00;

    // set interrupt slope to falling
    P1IES |= BUTTON_PIN;

    // Enable interrupt for button pin
    P1IE |= BUTTON_PIN;
}

void setup_timer_1() {
    // sets up timer 1 for debouncing

    // Select clock source to SMCLK
    TA1CTL |= TASSEL_2;

    // Set capture/compare 0 to 1999
    // Sets delay to 2ms
    TA1CCR0 = 1999;

    // Enable CCR0 interrupt
    TA1CCTL0 |= CCIE;

    // Enable Timer A0 interrupts
    TA1CTL |= TAIE;
}

void setup_timer_0() {
    // sets up timer 0 for pwm

    // Select clock source to SMCLK
    TA0CTL |= TASSEL_2;

    // Set to up mode
    TA0CTL |= MC_1;

    // Set capture/compare 0 to 999
    // 1kHz rate
    TA0CCR0 = 999;

    // Set capture/compare 1 to 499
    // 50% duty cycle
    TA0CCR1 = 499;

    // Enable CCR0 and CCR1 interrupt
    TA0CCTL0 |= CCIE;
    TA0CCTL1 |= CCIE;

    // Enable Timer A0 interrupts
    TA0CTL |= TAIE;
}

int main() {
    // run setup functions above
    setup_watchdog();
    setup_led();
    setup_button();
    setup_timer_1();
    setup_timer_0();

    // Enable global interrupt flag, enter low power mode
    _BIS_SR(GIE);

    // Enter infinite loop
    // since there is nothing to exit to, there is no point
    // in not having an infinite loop
    while(1) {
        __no_operation();
    }
}

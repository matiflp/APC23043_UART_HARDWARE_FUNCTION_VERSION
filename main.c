#include "apc230-43.h"

extern uint8_t TXData;
extern uint8_t check;

int main(void)
{
    // Stop WatchDog
    WDT_A_hold(WDT_A_BASE);

    // Configure Clock
    __bis_SR_register(SCG0);                    // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                  // Set REFO as FLL reference source
    CSCTL0 = 0;                                 // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                     // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_3;                        // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 243;                      // DCODIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                    // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));  // Poll until FLL is locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;  // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                                // default DCODIV as MCLK and SMCLK source

    // Configure Communication
    APC230_configurePins();
    APC230_configureUART();
    APC230_interrupt();


    //Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
    PMM_unlockLPM5();

    // Set Pins
    APC230_EN;
    APC230_SET;

    // wait until rf respond.
    __enable_interrupt();
    while(check != 1);
    check = 0;

    while(1);
}

#include "apc230-43.h"

extern uint16_t PtrTransmit;
extern uint8_t rxData[5];
extern uint8_t txData[5];
extern uint8_t check;
extern uint8_t i;

int main(void)
{
    // Stop Watchdog
    WDT_A_hold(WDT_A_BASE);

    // Configure Clock
    __bis_SR_register(SCG0);                 // disable FLL
    CSCTL3 |= SELREF__REFOCLK;               // Set REFO as FLL reference source
    CSCTL0 = 0;                              // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                  // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_3;                     // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 243;                   // DCODIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                 // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                             // default DCODIV as MCLK and SMCLK source

    // Transceptor APC230-43.
    APC230_initPort();
    APC230_initUART();
    APC230_start();

    //Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
    PMM_unlockLPM5();

    // Set pins to able to communicate with RF
    APC230_PIN_EN;
    APC230_PIN_SET;

    // Example of transmission
    PtrTransmit = 5;
    txData[0] = 0x15;
    txData[1] = 0x13;
    txData[2] = 0x11;
    txData[3] = 0x18;
    txData[4] = 0x17;

    while(1){
        // wait until rf respond.
        __enable_interrupt();
        while(check != 1);
        check = 0;
    }


}

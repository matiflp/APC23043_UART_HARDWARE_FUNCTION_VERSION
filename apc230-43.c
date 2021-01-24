/*
 * apc230-43.c
 *
 *  Created on: 4 dic. 2019
 *      Author: Matías López - Jesús López
 */
//*****************************************************************************
//
// apc230-43.c - Driver para manejar el Transceptor RF.
//
//*****************************************************************************

#include "apc230-43.h"

int PtrTransmit;
uint8_t rxData[5];
uint8_t txData[5];
uint8_t check = 0;
uint8_t i = 0;

//*****************************************************************************
void APC230_initPort(void)
{
    // Configura los pines para el modo de funcionamiento normal en el RF.
    APC230_PIN_EN;      // HABILITADO = 1 LÓGICO ; SLEEP = 0 LÓGICO
    //APC230_PIN_SET;     // MODO SETEO = 0 LÓGICO ; MODO NORMAL = 1 LÓGICo

    // Configure UART pins
    P1SEL0 |= BIT0 | BIT1;                    // set 2-UART pin as second function
}
//*****************************************************************************
void APC230_initUART(void)
{
    // Configure UART
    UCA0CTLW0 |= UCSWRST;                                               // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;                                         // SMCLK
    UCA0CTLW0 &= ~(UCMODE_0 + UCSYNC + UCSPB + UC7BIT + UCMSB + UCPEN); // UART mode, Asynchronous mode, One bit-stop, 8-bit data, LSB firt, No parity
    // Baud Rate calculation
    UCA0BR0 = 52;                                                       // N = (8000000/9600) = 833.3333333 => N > 16 => UCA0BR0 = INT(N/16) = 52
    UCA0MCTLW = 0x4900 | UCOS16 | UCBRF_1;                              // N > 16 => setear OCS16 = 1 ;
                                                                        // UCBRFx =  INT([(N/16) – INT(N/16)] × 16) = INT((52.083-52)*16) = 1 => setear UCBRF_1
                                                                        // 8M/9600 - INT(8M/9600) = 0.33 => UCBRSx value = 0x49 (Ver 14.3.10 "Setting a Baud Rate" User's Guide)
}
//*****************************************************************************
void APC230_start(void)
{
    UCA0CTLW0 &= ~UCSWRST;                                                            // Initialize eUSCI
    UCA0IFG &= ~(EUSCI_A_UART_RECEIVE_INTERRUPT | EUSCI_A_UART_TRANSMIT_INTERRUPT);   // Limpia interrupciones pendientes
    UCA0IE |= UCRXIE | UCTXIE;                                                        // Enable USCI_A0 RX interrupt. Solo es necesario esta interrupcion ya que cuando se
}
//*****************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void EUSCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            rxData[i] = UCA0RXBUF;
            i++;
            if(i == 1)
            {
                __bis_SR_register(LPM0_bits + GIE);
            }
            if(rxData[i-1] == 0x0D)
            {
                i = 0;
                UCA0IFG = 0x00;
                UCA0STATW = 0x00;
                __bic_SR_register_on_exit(LPM0_bits + GIE); // Exit LPM0
                UCA0IE &= ~UCRXIE;
                UCA0TXBUF = txData[PtrTransmit];
            }
            break;
        case USCI_UART_UCTXIFG:
            i++;
            PtrTransmit--;                          // Decrement TX byte counter
            UCA0TXBUF = txData[PtrTransmit];// Load TX buffer
            if(i == 1)
            {
                __bis_SR_register(LPM0_bits + GIE);
            }
            if(PtrTransmit < 0)
            {
                check = 1;
                i = 0;
                __bic_SR_register_on_exit(LPM0_bits + GIE); // Exit LPM0
            }
            break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
    }
}

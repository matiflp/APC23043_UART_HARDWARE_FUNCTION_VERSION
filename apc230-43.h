/**
  * @file     apc230-43.h
  * @brief    Driver para manejar el Transceptor RF.
  * @date     Created on: 4 dic. 2019
  * @authors  Matías López - Jesús López
  * @version  1.0
  */
//*****************************************************************************
//
// apc230-43.h - Driver para manejar el Transceptor RF.
//
//*****************************************************************************

#ifndef APC230_43_H_
#define APC230_43_H_

//*****************************************************************************
//                              Include
//*****************************************************************************
#include "driverlib.h"

//*****************************************************************************
//                              Defines
//*****************************************************************************
//*****************************************************************************
//! @name Pines del Transceptor APC230:
//! \brief Terminales necesarios para comunicarse con el transceptor.
//! @{
//*****************************************************************************
//*****************************************************************************
//! \details Pin GPIO utlizado para habilitar el módulo de RF (Pin EN).
//*****************************************************************************
#define APC230_PIN_EN   {P2DIR |= BIT3 ; P2OUT |= BIT3;}

//*****************************************************************************
//! \detals Pin GPIO que se deja en cero para colocar el RF en modo
//!         funcionamiento normal (Pin SET).
//*****************************************************************************
#define APC230_PIN_SET  {P5DIR |= BIT0 ; P5OUT |= BIT0;}

//*****************************************************************************
//! @}
//*****************************************************************************

//*****************************************************************************
//                              Funciones Prototipos
//*****************************************************************************
//*****************************************************************************
//! \brief Configura los pines que se utilizarán en la comunicación UART.
//!
//! \details \b Descripción \n
//!          Se configura el pin EN del RF para permitir la comunicación con
//!          el \b MCU, y además el pin SET se coloca en uno para un modo de
//!          funcionamiento normal ya que de lo contrario sería para una
//!          configuración de los parametros de la comunicación. Luego se
//!          configuran los pines UART del MCU, \b P1.0 (TXD) y \b P1.1 (RXD).
//!          Esto se consigue seleccionando la segunda función de las que
//!          pueden realizar dichos pines (recordar que los pines \b MCU
//!          tienen funciones multiplexadas).
//!
//! \return \c void.
//!
//! \attention Modifica los bits del registro \b P1DIR y del registro
//!            \b P1SEL.
//*****************************************************************************
void APC230_initPort(void);

//*****************************************************************************
//! \brief Configura la comunicación UART.
//!
//! \details \b Descripción \n
//!          Configura los parámetros comunicación de manera que sean iguales a los
//!          del transceptor. En primer lugar, se define e inicializa la estructura.
//!          Luego se elige la fuente de reloj, la cual es \b SMCLK. El Baudrate
//!          configurado en el transceptor es igual a 9600 bps, la fuente fBRCLK = 8 MHz,
//!          entonces el factor N = fBRCLK/Baudrate requerido para obtener este Baudrate
//!          es un valor no entero, por lo tanto, se utiliza un divisor y una etapa
//!          de modulador para cumplir con que el factor sea lo más proximo posible.
//!          Al ser N > 16 se utilizará el sobremuestro seteando \b OS16 = 1 en el
//!          registro \b UCA0MCTLW, luego en el registro \b UCA0BRW se coloca
//!          \b UCBRx = INT(N/16) y en el registro \b UCA0MCTLW se coloca
//!          \b UCBRFx = INT([(N/16) - INT(N/16)] × 16) obteniendo con presición el
//!          Baudrate deseado. Por último, se configuran los parámetros restantes como
//!          que no hay paridad, cual byte va primero, el bit de stop y el modo UART.
//!          Se inicia la comunicación pasando  como parametros la direccion base de
//!          \b EUSCIA y la estructura. \note Para mas información visitar en el documento
//!          <em>"MSP430FR4xx and MSP430FR2xx family"</em> la sección <em>"22.3.10 Setting a Baud
//!          Rate"</em>.
//!
//! \return \c Un booleano que indica si la comunicación fue exitosa.
//!
//! \attention Modifica los bits del registro \b UCA0CTLW0, \b UCA0BR0 y
//!            \b UCA0MCTLW.
//*****************************************************************************
void APC230_initUART(void);

//*****************************************************************************
//! \brief Inicializa la comunicación UART.
//!
//! \details \b Descripción \n
//!          Una vez finalizada la configuración se resetea el bit \b UCSWRST.
//!          Luego se resetea el flag en el \b UCA0IFG ya que alguna interrupción
//!          podría haber quedado pendiente en el \b UCAxRXBUF. Por último, se
//!          se habiltan las interrupciones de RX.
//!
//! \return \c void.
//!
//! \attention Modifica los bits del registro \b UCA0CTLW0, \b UCA0IFG y
//!            \b UCA0IE.
//*****************************************************************************
void APC230_start(void);

#endif /* APC230_43_H_ */

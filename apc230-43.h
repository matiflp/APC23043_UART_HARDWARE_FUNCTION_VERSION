/**
  * @file     apc230-43.h
  * @brief    Driver para manejar el Transceptor RF.
  * @date     Created on: 4 dic. 2019
  * @authors  Mat�as L�pez - Jes�s L�pez
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
//! \details Pin GPIO utlizado para habilitar el m�dulo de RF (Pin EN).
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
//! \brief Configura los pines que se utilizar�n en la comunicaci�n UART.
//!
//! \details \b Descripci�n \n
//!          Se configura el pin EN del RF para permitir la comunicaci�n con
//!          el \b MCU, y adem�s el pin SET se coloca en uno para un modo de
//!          funcionamiento normal ya que de lo contrario ser�a para una
//!          configuraci�n de los parametros de la comunicaci�n. Luego se
//!          configuran los pines UART del MCU, \b P1.0 (TXD) y \b P1.1 (RXD).
//!          Esto se consigue seleccionando la segunda funci�n de las que
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
//! \brief Configura la comunicaci�n UART.
//!
//! \details \b Descripci�n \n
//!          Configura los par�metros comunicaci�n de manera que sean iguales a los
//!          del transceptor. En primer lugar, se define e inicializa la estructura.
//!          Luego se elige la fuente de reloj, la cual es \b SMCLK. El Baudrate
//!          configurado en el transceptor es igual a 9600 bps, la fuente fBRCLK = 8 MHz,
//!          entonces el factor N = fBRCLK/Baudrate requerido para obtener este Baudrate
//!          es un valor no entero, por lo tanto, se utiliza un divisor y una etapa
//!          de modulador para cumplir con que el factor sea lo m�s proximo posible.
//!          Al ser N > 16 se utilizar� el sobremuestro seteando \b OS16 = 1 en el
//!          registro \b UCA0MCTLW, luego en el registro \b UCA0BRW se coloca
//!          \b UCBRx = INT(N/16) y en el registro \b UCA0MCTLW se coloca
//!          \b UCBRFx = INT([(N/16) - INT(N/16)] � 16) obteniendo con presici�n el
//!          Baudrate deseado. Por �ltimo, se configuran los par�metros restantes como
//!          que no hay paridad, cual byte va primero, el bit de stop y el modo UART.
//!          Se inicia la comunicaci�n pasando  como parametros la direccion base de
//!          \b EUSCIA y la estructura. \note Para mas informaci�n visitar en el documento
//!          <em>"MSP430FR4xx and MSP430FR2xx family"</em> la secci�n <em>"22.3.10 Setting a Baud
//!          Rate"</em>.
//!
//! \return \c Un booleano que indica si la comunicaci�n fue exitosa.
//!
//! \attention Modifica los bits del registro \b UCA0CTLW0, \b UCA0BR0 y
//!            \b UCA0MCTLW.
//*****************************************************************************
void APC230_initUART(void);

//*****************************************************************************
//! \brief Inicializa la comunicaci�n UART.
//!
//! \details \b Descripci�n \n
//!          Una vez finalizada la configuraci�n se resetea el bit \b UCSWRST.
//!          Luego se resetea el flag en el \b UCA0IFG ya que alguna interrupci�n
//!          podr�a haber quedado pendiente en el \b UCAxRXBUF. Por �ltimo, se
//!          se habiltan las interrupciones de RX.
//!
//! \return \c void.
//!
//! \attention Modifica los bits del registro \b UCA0CTLW0, \b UCA0IFG y
//!            \b UCA0IE.
//*****************************************************************************
void APC230_start(void);

#endif /* APC230_43_H_ */

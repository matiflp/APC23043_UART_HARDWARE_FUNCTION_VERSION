/*
 * apc230-43.h
 *
 *  Created on: 4 dic. 2019
 *      Author: Matías López - Jesús López
 */

#ifndef APC230_43_H_
#define APC230_43_H_

#include "driverlib.h"
#include "Board.h"

#define APC230_EN   {P1DIR |= BIT6 ; P1OUT |= BIT6;}
#define APC230_SET  {P5DIR |= BIT0 ; P5OUT |= BIT0;}

void AP230C_interrupt(void);
void APC230_configurePins(void);
bool APC230_configureUART(void);



#endif /* APC230_43_H_ */

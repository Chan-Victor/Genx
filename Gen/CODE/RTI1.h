/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : RTI1.h
**     Project   : GensetX
**     Processor : MC9S12G128MLL
**     Component : Init_RTI
**     Version   : Component 01.097, Driver 02.02, CPU db: 3.00.002
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 2018-7-14, 19:54
**     Abstract  :
**          This file implements the RTI (RTIfree)
**          module initialization according to the Peripheral Initialization
**          Component settings, and defines interrupt service routines prototypes.
**          The RTI can be used to generate a hardware interrupt
**          at a fixed periodic rate. If enabled (by setting RTIE=1),
**          this interrupt will occur at the rate selected by the RTICTL
**          register.The RTI runs with a gated OSCCLK ).At the end of the
**          RTI time-out period the RTIF flag is set to one and a new RTI
**          time-out period starts immediately.
**     Settings  :
**          Component name                                 : RTI1
**          Device                                         : RTI
**          Settings                                       : 
**            Clock settings                               : 
**              Clock source                               : OSCCLK
**              Prescaler                                  : 10^4
**              Modulus                                    : 16
**              Divider                                    : Decimal
**              Period                                     : 20 ms
**            RTI enable in Pseudo Stop Mode               : no
**          Interrupts                                     : 
**            RTI Interrupt                                : 
**              RTI Interrupt                              : Enabled
**              Interrupt                                  : Vrti
**              Priority                                   : 1
**              ISR name                                   : OSTickISR
**          Initialization                                 : 
**            Call Init method                             : yes
**     Contents  :
**         Init - void RTI1_Init(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __RTI1
#define __RTI1

/* MODULE RTI1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited components */

#include "Cpu.h"


void RTI1_Init(void);
/*
** ===================================================================
**     Method      :  RTI1_Init (component Init_RTI)
**
**     Description :
**         This method initializes registers of the RTI module
**         according to this Peripheral Initialization settings. Call
**         this method in user code to initialize the module. By
**         default, the method is called by PE automatically; see "Call
**         Init method" property of the component for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/*
** ===================================================================
** The interrupt service routine must be implemented by user in one
** of the user modules (see RTI1.c file for more information).
** ===================================================================
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void OSTickISR(void);
#pragma CODE_SEG DEFAULT
/* END RTI1. */

#endif /* ifndef __RTI1 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/

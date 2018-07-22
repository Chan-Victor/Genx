/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : AS1.h
**     Project   : GensetX
**     Processor : MC9S12G128MLL
**     Component : AsynchroSerial
**     Version   : Component 02.483, Driver 01.22, CPU db: 3.00.002
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 2018/7/22, 19:53
**     Abstract  :
**         This component "AsynchroSerial" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial channel.
**     Settings  :
**         Serial channel              : SCI1
**
**         Protocol
**             Init baud rate          : 9600baud
**             Width                   : 8 bits
**             Stop bits               : 1
**             Parity                  : none
**             Breaks                  : Disabled
**             Input buffer size       : 0
**             Output buffer size      : 0
**
**         Registers
**             Input buffer            : SCI1DRL   [$00D7]
**             Output buffer           : SCI1DRL   [$00D7]
**             Control register        : SCI1CR1   [$00D2]
**             Mode register           : SCI1CR2   [$00D3]
**             Baud setting reg.       : SCI1BD    [$00D0]
**             Special register        : SCI1SR1   [$00D4]
**
**             Priority                : 
**
**             Priority                : 
**
**         Used pins:
**         ----------------------------------------------------------
**           Function | On package           |    Name
**         ----------------------------------------------------------
**            Input   |     84               |  PS2_RXD1
**            Output  |     85               |  PS3_TXD1
**         ----------------------------------------------------------
**
**
**
**     Contents  :
**         RecvChar        - byte AS1_RecvChar(AS1_TComData *Chr);
**         SendChar        - byte AS1_SendChar(AS1_TComData Chr);
**         GetCharsInRxBuf - word AS1_GetCharsInRxBuf(void);
**         GetCharsInTxBuf - word AS1_GetCharsInTxBuf(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __AS1
#define __AS1

/* MODULE AS1. */

#include "Cpu.h"



#ifndef __BWUserType_AS1_TError
#define __BWUserType_AS1_TError
  typedef union {
    byte err;
    struct {
      /*lint -save  -e46 Disable MISRA rule (6.4) checking. */
      bool OverRun  : 1;               /* Overrun error flag */
      bool Framing  : 1;               /* Framing error flag */
      bool Parity   : 1;               /* Parity error flag */
      bool RxBufOvf : 1;               /* Rx buffer full error flag */
      bool Noise    : 1;               /* Noise error flag */
      bool Break    : 1;               /* Break detect */
      bool LINSync  : 1;               /* LIN synchronization error */
      bool BitError  : 1;              /* Bit error flag - mismatch to the expected value happened. */
      /*lint -restore Enable MISRA rule (6.4) checking. */
    } errName;
  } AS1_TError;                        /* Error flags. For languages which don't support bit access is byte access only to error flags possible. */
#endif

#ifndef __BWUserType_AS1_TComData
#define __BWUserType_AS1_TComData
  typedef byte AS1_TComData ;          /* User type for communication. Size of this type depends on the communication data witdh. */
#endif

#pragma DATA_SEG AS1_DATA              /* Select data segment "AS1_DATA" */
#pragma CODE_SEG AS1_CODE



byte AS1_RecvChar(AS1_TComData *Chr);
/*
** ===================================================================
**     Method      :  AS1_RecvChar (component AsynchroSerial)
**
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). This method is enabled only if the receiver property
**         is enabled.
**         [Note:] Because the preferred method to handle error and
**         break exception in the interrupt mode is to use events
**         <OnError> and <OnBreak> the return value ERR_RXEMPTY has
**         higher priority than other error codes. As a consequence the
**         information about an exception in interrupt mode is returned
**         only if there is a valid character ready to be read.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to a received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_BREAK - Break character is detected
**                           (only when the <Interrupt service> property
**                           is disabled and the <Break signal> property
**                           is enabled)
**                           ERR_COMMON - common error occurred (the
**                           <GetError> method can be used for error
**                           specification)
** ===================================================================
*/

byte AS1_SendChar(AS1_TComData Chr);
/*
** ===================================================================
**     Method      :  AS1_SendChar (component AsynchroSerial)
**
**     Description :
**         Sends one character to the channel. If the component is
**         temporarily disabled (Disable method) SendChar method only
**         stores data into an output buffer. In case of a zero output
**         buffer size, only one character can be stored. Enabling the
**         component (Enable method) starts the transmission of the
**         stored data. This method is available only if the
**         transmitter property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/

#define AS1_GetCharsInRxBuf() ((word)(SCI1SR1_RDRF)) /* Return number of chars in receive buffer */
/*
** ===================================================================
**     Method      :  AS1_GetCharsInRxBuf (component AsynchroSerial)
**
**     Description :
**         Returns the number of characters in the input buffer. This
**         method is available only if the receiver property is enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the input
**                           buffer.
** ===================================================================
*/

#define AS1_GetCharsInTxBuf() (SCI1SR1_TDRE ? (word)0U : (word)1U) /* Return number of chars in the transmitter buffer */
/*
** ===================================================================
**     Method      :  AS1_GetCharsInTxBuf (component AsynchroSerial)
**
**     Description :
**         Returns the number of characters in the output buffer. This
**         method is available only if the transmitter property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the output
**                           buffer.
** ===================================================================
*/

void AS1_Init(void);
/*
** ===================================================================
**     Method      :  AS1_Init (component AsynchroSerial)
**
**     Description :
**         Initializes the associated peripheral(s) and internal 
**         variables of the component. The method is called automatically 
**         as a part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


#pragma DATA_SEG DEFAULT               /* Select data segment "DEFAULT" */
#pragma CODE_SEG DEFAULT
/* END AS1. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/

#endif /* ifndef __AS1 */
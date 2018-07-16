//*********************************************************************************************************
//                                          kebole 汽车电子
//                                             PTC项目
//                                             PWM子组
//                                              
//
//                                      (c) Copyright 2017-2020 
//                                        All Rights Reserved
//
//Framework : YIN DAHU
//Author    : YIN DAHU
//Time      : 2017-11-09
//*********************************************************************************************************
#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef unsigned char   OS_STK;                   /* Each stack entry is 16-bit wide                    */
typedef unsigned char   OS_CPU_SR;                /* The CPU Status Word is 16-bit wide                 */



#define  OS_STK_GROWTH        1                  /* Define stack growth: 1 = Down, 0 = Up  */
#define  OS_TASK_SW() asm swi;

/* 
*********************************************************************************************************
*                              MC9S12G48
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if 
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/
#define  OS_CRITICAL_METHOD   3

#if   OS_CRITICAL_METHOD==1
#define  OS_ENTER_CRITICAL()  asm  sei            /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   asm  cli                  /* Enable  interrupts                        */
#endif
  
#if   OS_CRITICAL_METHOD==2
#define   OS_ENTER_CRITICAL() asm pshc;asm sei
#define   OS_EXIT_CRITICAL()  asm pulc
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  asm tpa; asm sei; asm staa cpu_sr
#define  OS_EXIT_CRITICAL()   asm ldaa cpu_sr; asm tap
#endif

///=======================================
/// <summary>
/// save stack pointer
/// </summary>
///=======================================
#define  OS_SAVE_SP();  if(OSIntNesting==1){asm ldy OSTCBCur; asm sts 0,y;}

///=======================================
/// <summary>
/// Memory Page Register operate
/// </summary>
///=======================================
#define  OS_SAVE_PR();    asm ldaa PPAGE; asm psha;     
#define  OS_RESTORE_PR(); asm pula;asm staa PPAGE;asm nop; 
 
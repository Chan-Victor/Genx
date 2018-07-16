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
#define  OS_CPU_GLOBALS
#include "includes.h"

#pragma CODE_SEG __NEAR_SEG NON_BANKED
/******时钟节拍函数*****/
__interrupt void OSTickISR(void)
{                   
  CPMUFLG_RTIF=1;
  OS_SAVE_PR();  
                  
  
  OSIntEnter();                       
  OS_SAVE_SP();  
  OSTimeTick();
  OSIntExit();
   
  OS_RESTORE_PR();
  asm rti;                   
}
   
          
//任务级上下文切换函数   
__interrupt  void OSCtxSw(void)
{  
  asm
  {
  ldx	 OSTCBCur					;  3~, Point to current task's TCB						
	sts	 0,x						 ;	3~, Save stack pointer in preempted task's TCB	
  }

  OS_SAVE_PR();
  asm
  {
    ldx	 OSTCBCur					;  3~, Point to current task's TCB						
	  sts	 0,x						 ;	3~, Save stack pointer in preempted task's TCB		 
  
	  call	 OSTaskSwHook				;  4~, Call user task switch hook						
	  
	  ldx	 OSTCBHighRdy				;  3~, OSTCBCur  = OSTCBHighRdy
	  stx	 OSTCBCur					;  3~						  
	  
	  ldab	 OSPrioHighRdy				;  3~, OSPrioCur = OSPrioHighRdy						
		stab	 OSPrioCur					;  3~
		  
		lds	 0,x						 ;	3~, Load SP into 68HC12
  }
  OS_RESTORE_PR(); 
  
  asm  rti;  
}


//中断级任务切换 
void  OSIntCtxSw(void)
{
  asm
  {
    call    OSTaskSwHook               ;  4~, Call user task switch hook                
    
    ldx    OSTCBHighRdy               ;  3~, OSTCBCur  = OSTCBHighRdy
    stx    OSTCBCur                   ;  3~                         
    
    ldab   OSPrioHighRdy              ;  3~, OSPrioCur = OSPrioHighRdy                        
    stab   OSPrioCur                  ;  3~
    
    lds    0,x                         ;  3~, Load SP into 68HC12  
  }  
  OS_RESTORE_PR();                    
  asm  rti;                
}

///=======================================
/// <summary>
/// Initialize the selected task`s own stack.made it like the scene of Interrupt. 
/// </summary>
/// <returns>null</returns>
///=======================================
OS_STK  *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    INT16U *wstk;
    INT8U  *bstk;

    // opt     = opt;                           /* 'opt' is not used, prevent warning                     */
    wstk    = (INT16U *)ptos;                /* Load stack pointer                                     */
    *--wstk = (INT16U)pdata;                 /* Simulate call to function with argument                */
    *--wstk = (INT16U)(((INT32U)task)>>8); /* Return address.  Format: PCH:PCL:PPAGE (cut out PPage) */
    *--wstk = (INT16U)0x0000;                /* Y Register                                             */
    *--wstk = (INT16U)0x0000;                /* X Register                                             */
    *--wstk = (INT16U)0x0000;                /* D Register                                             */    
    
    bstk    = (INT8U *)wstk;                 /* Convert WORD ptr to BYTE ptr to set CCR                */
    *--bstk = (0xC0);                        /* CCR Register (Disable STOP instruction and XIRQ)       */
    
    *--bstk = (INT8U)task;                   /* Save the PPAGE register                                */
    return ((OS_STK *)bstk);                 /* Return pointer to new top-of-stack                     */
}

void OSStartHighRdy(void)
{
  asm
  {
    call   OSTaskSwHook                ; Invoke user defined context switch hook            

    ldab   #$01                        ; Indicate that we are multitasking
    stab   OSRunning
                           

    ldx    OSTCBHighRdy              
    stx    OSTCBCur
    lds    0,x                         
  }
  
  OS_RESTORE_PR();
                                
  asm rti;                                 
}
#pragma CODE_SEG DEFAULT 


/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
   
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
    
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) I decided to change the options on the statistic task to allow for floating-point in
*                 case you decide to do math. in OSTaskStatHook().
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void OSTaskCreateHook (OS_TCB *ptcb)
{
  // ptcb=ptcb; 
}
#endif

/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void OSTaskDelHook (OS_TCB *ptcb)
{
   // ptcb=ptcb;
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN && OS_VERSION >= 251
void  OSTaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void OSTaskStatHook (void)
{
}
#endif





/*$PAGE*/
/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void OSTaskSwHook (void)
{
   
}
#endif

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
    //ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void OSTimeTickHook (void)
{
    
}
#endif

                     
                       
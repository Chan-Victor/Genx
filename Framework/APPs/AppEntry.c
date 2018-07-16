//*********************************************************************************************************
//                                          kebole ��������
//                                             PTC��Ŀ
//                                             PWM����
//                                              
//
//                                      (c) Copyright 2017-2020 
//                                        All Rights Reserved
//
//Framework : YIN DAHU
//Author    : YIN DAHU
//Time      : 2018-7-10
//*********************************************************************************************************
#include "HAL_Define.h"
#include "GensetProperty.h"


///=======================================
/// <summary>
/// ���ݶ���
/// </summary>
///=======================================

                                                
///=======================================
/// <summary>
/// ��������
/// </summary>
///=======================================
#define STK_SIZE  128
#define STK_TOP   STK_SIZE-1

OS_STK  PeriodTask_Stk[STK_SIZE];
void  PeriodTask(void *pdata);

OS_STK  RecorderTask_Stk[STK_SIZE];
void  RecorderTask(void *pdata);

OS_STK  CalibTask_Stk[STK_SIZE];
void  CalibTask(void *pdata);

OS_STK  ExtendComTask_Stk[STK_SIZE];
void  ExtendComTask(void *pdata);


///=======================================
/// <summary>
/// ���
/// </summary>
///======================================= 
void AppsEntry(void) {
  INT8U err=0;
  
  //��װOS
  OSInit();
  

  err= OSTaskCreate(PeriodTask, (void *)0, (void *)&PeriodTask_Stk[STK_TOP], 1);//������ȼ�   
  err= OSTaskCreate(RecorderTask, (void *)0, (void *)&RecorderTask_Stk[STK_TOP], 2);
  err= OSTaskCreate(CalibTask, (void *)0, (void *)&CalibTask_Stk[STK_TOP], 3);
  err= OSTaskCreate(ExtendComTask, (void *)0, (void *)&ExtendComTask_Stk[STK_TOP], 4);
  
  __EI();
  
  OSStart();
}

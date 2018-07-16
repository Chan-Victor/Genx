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
//Time      : 2018-7-10
//*********************************************************************************************************
#include "HAL_Define.h"
#include "GensetProperty.h"


///=======================================
/// <summary>
/// 数据定义
/// </summary>
///=======================================

                                                
///=======================================
/// <summary>
/// 任务声明
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
/// 入口
/// </summary>
///======================================= 
void AppsEntry(void) {
  INT8U err=0;
  
  //安装OS
  OSInit();
  

  err= OSTaskCreate(PeriodTask, (void *)0, (void *)&PeriodTask_Stk[STK_TOP], 1);//最高优先级   
  err= OSTaskCreate(RecorderTask, (void *)0, (void *)&RecorderTask_Stk[STK_TOP], 2);
  err= OSTaskCreate(CalibTask, (void *)0, (void *)&CalibTask_Stk[STK_TOP], 3);
  err= OSTaskCreate(ExtendComTask, (void *)0, (void *)&ExtendComTask_Stk[STK_TOP], 4);
  
  __EI();
  
  OSStart();
}

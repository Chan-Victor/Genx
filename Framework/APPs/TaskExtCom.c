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
#include "GensetProperty.h"
#include "HAL_Define.h"
 
INT16U tickcnt=0; 
INT8U flag;                              
 
///=======================================
/// <summary>
/// ��չͨ�Ž���
/// </summary>
///=======================================
void  ExtendComTask(void *pdata)
{    	 
  pdata=pdata;


  for (;;) 
  {      
    tickcnt++;
    

     
    OSTimeDly(50);  		
  }
}
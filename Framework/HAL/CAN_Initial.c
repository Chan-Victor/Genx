/*
***********************************************************************************************************
**  (C) Copyright 2018-2028 by LiLin. All rights reserved.
**
**  (C) Copyright 2018-2028 by LiLin.
**
**  The information contained herein is confidential, proprietary
**  to LiLin, and considered a trade secret as defined
**  in section of the vehicle recorder code based on MC9S12D family.
**  Use of this information by anyone other than authorized people
**  is granted only under a written non-disclosure agreement,
**  expressly prescribing the scope and
**  manner of such use.
**  
**  Reference Operation System: NONE
**  MainHardWare: Freescale Mc9s12d128mal
**  Compiler: CodeWarrior 5.1
**
**  Author: LiLin   created at: 2018-02-05
**  Version:             V1.0
***********************************************************************************************************
*/
/*
***********************************************************************************************************
** include file
***********************************************************************************************************
*/
#include "CRC_Cal.h"
#include "CAN_Initial.h"


/*
***********************************************************************************************************
** Variable and the Macro define
***********************************************************************************************************
*/
typedef union 
{
  dword dw;
  struct {
    byte b0;
    byte b1;
    byte b2;
    byte b3;
  }b;
}DwordSwap;

typedef struct 
{                       /* Message buffer structure */
  byte IDR0;
  byte IDR1;
  byte IDR2;
  byte IDR3;
  byte Data[8];
  byte DLR;
  byte TBPR;
  byte Reserved;
  byte Reserved2;
}TxMsgBuff;             /* Message buffer structure */

#define CAN_STANDARD_FRAME_MAX_ID    0x07FFU /* Max ID of the standard frame */
#define CAN_EXTENDED_FRAME_MAX_ID    0x1FFFFFFFUL /* Max ID of the extended frame */
#define CAN_EXTENDED_FRAME_MASK      0x60000000UL /* Invalid extended frame mask*/



CAN_TxRx_PARA     CAN_TxRx_Para_Info;
CAN_2B5_DAT       CAN_2B5_Frame_Dat;
CAN_2B6_DAT       CAN_2B6_Frame_Dat;
CAN_2A0_DAT       CAN_2A0_Frame_Dat;
CAN_295_DAT       CAN_295_Frame_Dat;
CAN_2A0_EEP       CAN_2A0_EEprom_Dat;

/*
***********************************************************************************************************
** Function build
***********************************************************************************************************
*/


/*
***********************************************************************************************************
** 函数名称：CAN_Msg_Tx_Fuc
** 函数描述：CAN报文发送函数
** 输入参数：txMsg        : 需要发送的数据报文内容信息
** 返回值  ：FALSE:发送失败;TRUE:发送成功
***********************************************************************************************************
*/
word txerrcnt=0;
static Bool CAN_Msg_Tx_Fuc(CAN_MSG_INFO txMsg)
{
  dword tmpId;
  dword txTimeout=9999;
  
  TxMsgBuff *MsgBuff;
  
  if(txMsg.len > 8)     /* 检查数据长度是否满足要求     */
    return(FALSE);      /* 返回发送失败信息             */ 

  if(CANCTL0_SYNCH==0)  /* 检查总线时钟                 */
    return(FALSE);      /* 返回发送失败信息             */

  if ((txMsg.IDE  == 0) && (txMsg.msgId > CAN_STANDARD_FRAME_MAX_ID))   /*扩展帧标志为0,帧ID大于0x7FF,返回错误信息      */
  { 
    return FALSE;      /*返回错误信息     */            
  }
  if ((txMsg.IDE  == 1) && ((txMsg.msgId & CAN_EXTENDED_FRAME_MASK) != 0U)) /*判断扩展帧ID是否满足要求    */
  { 
    return FALSE;                  
  }
  if (txMsg.len > 8)  /* 判断发送长度是否符合要求     */
  {     
    return FALSE;     /*返回错误信息                  */
  }
  DisableInterrupts;  /*禁止中断                      */
  if ((CANTFLG & 0x07U) == 0U) /*判断发送缓冲区是否已满 */
  {       
    EnableInterrupts; /*打开中断                      */
    return FALSE;     /*返回发送错误信息              */
  }
  CANTBSEL = CANTFLG; /*取发送空闲发送缓冲区          */
  MsgBuff = (TxMsgBuff *)&CANTXIDR0;/*设置指针地址    */
  if (txMsg.IDE==1)   /*判断是否为扩展帧              */
  {
    tmpId = (((txMsg.msgId & 0x1FFC0000UL) << 3) | 0x00180000UL | ((txMsg.msgId & 0x0003FFFFUL) << 1)); /*计算帧ID        */
  }
  else 
  {
    tmpId = txMsg.msgId << 21;  /*标准帧ID            */
  }
  
  MsgBuff->IDR3 = ((DwordSwap *)&tmpId)->b.b3; /* 将帧ID信息填入寄存器     */
  MsgBuff->IDR2 = ((DwordSwap *)&tmpId)->b.b2;
  MsgBuff->IDR1 = ((DwordSwap *)&tmpId)->b.b1;
  MsgBuff->IDR0 = ((DwordSwap *)&tmpId)->b.b0;

  if (txMsg.RTR == 0) /*判断帧类型是否为数据帧        */
  {      
    *((CAN_FRAM_DAT*)&MsgBuff->Data) = *((CAN_FRAM_DAT*)&txMsg.data);  /*读取帧内数据          */

    if (txMsg.IDE==1) /*判断是否为扩展帧              */
    { 
      MsgBuff->IDR3 &= 0xFEU; /*设置帧类型是数据帧    */       
    }
    else 
    {
      MsgBuff->IDR1 &= 0xEFU; /*设置帧类型是数据帧    */            
    }
  }
  else 
  {                               
    /*设置帧类型是数据帧    */   
    if (txMsg.IDE==1) /*判断是否为扩展帧              */
    { 
      MsgBuff->IDR3 |= 0x01U;  /*设置帧类型是远程帧    */      
    }
    else 
    {
      MsgBuff->IDR1 |= 0x10U;  /*设置帧类型是远程帧    */      
    }
  }
  EnableInterrupts;            /*打开中断              */
  CANTXDLR_DLC = txMsg.len;    /*设置发送长度信息      */
  CANTXTBPR = txMsg.prty;      /*设置发送优先级        */
  CANTFLG = CANTBSEL;          /*启动发送              */
  while(!(CANTFLG&CANTBSEL))
  {
    if(txTimeout>0)
    {
      txTimeout--;
    }
    else 
    {
      txerrcnt++;
      return FALSE;         /*返回发送失败信息      */
    }
  };  /*等待发送成功          */
  return TRUE;                 /*返回发送成功信息      */
}

/*
***********************************************************************************************************
** 函数名称：CAN_Msg_Rx_Fuc
** 函数描述：CAN报文接收函数
** 输入参数：*pRxMsg    : 接收的CAN数据     
** 返回值  ：FALSE : 未收到CAN数据帧;TRUE : 成功接收到CAN数据帧
***********************************************************************************************************
*/
static Bool CAN_Msg_Rx_Fuc(CAN_MSG_INFO *pRxMsg)
{
  dword tmpId = 0U;

  if(CANRFLG_RXF==0)        /* 检测是否接收到CAN数据帧    */
    return(FALSE);          /* 返回未收到CAN数据标志      */      
  if(CANRXIDR1_IDE==1)      /* 检测 CAN协议报文模式 （一般/扩展） 标识符   */
    pRxMsg->IDE = 1;        /* 设置接收的数据帧为扩展帧   */
  else pRxMsg->IDE = 0;     /* 设置接收的数据帧为标准帧   */

  ((DwordSwap*)&tmpId)->b.b0 = CANRXIDR0;
  ((DwordSwap*)&tmpId)->b.b1 = CANRXIDR1;
  ((DwordSwap*)&tmpId)->b.b2 = CANRXIDR2;
  ((DwordSwap*)&tmpId)->b.b3 = CANRXIDR3;
  if(pRxMsg->IDE==1)        /*如果接收到的数据为扩展帧    */
  {
    if(CANRXIDR3_RTR==1)
    {
      pRxMsg->RTR = 1;
    }                       /*读取帧类型标志              */
    else 
    {
      pRxMsg->RTR = 0;
    }
    pRxMsg->msgId = (((tmpId >> 1) & 0x3FFFFUL) | ((tmpId >> 3) & 0x1FFC0000UL));   /*保存扩展帧消息ID     */
  }
  else
  {
    pRxMsg->msgId = tmpId >> 21;   /*保存标准帧消息ID     */
    if(CANRXIDR1&0x10)             /*读取帧类型标志       */
    {
      pRxMsg->RTR = 1;
    }
    else 
    {
      pRxMsg->RTR = 0;
    }
  }
  pRxMsg->len = CANRXDLR_DLC;   /*读取数据长度          */
  *((CAN_FRAM_DAT*)&pRxMsg->data) = *((CAN_FRAM_DAT*)&CANRXDSR0);  /*读取帧内数据          */
  //CANRFLG = 0x01;     /* 清 RXF 标志位 (缓冲器准备接收)   */
  CANRFLG_RXF = 1;
  return(TRUE);
}


/*
***********************************************************************************************************
** 函数名称：CAN_2B5_Dat_Update_Fuc
** 函数描述：CAN数据帧2B5更新
** 输入参数：无
** 返回值  ：无
***********************************************************************************************************
*/
static void CAN_2B5_Dat_Update_Fuc(void)
{
  if(CAN_TxRx_Para_Info.PTCPCBTem>200) CAN_TxRx_Para_Info.PTCPCBTem=200;
  else if(CAN_TxRx_Para_Info.PTCPCBTem<-40) CAN_TxRx_Para_Info.PTCPCBTem=-40;
  
  CAN_2B5_Frame_Dat.PTCOverHeat = CAN_TxRx_Para_Info.PTCOverHeat;
  CAN_2B5_Frame_Dat.PTCOverCrnt = CAN_TxRx_Para_Info.PTCOverCrnt;
  CAN_2B5_Frame_Dat.PTCOverVol = CAN_TxRx_Para_Info.PTCOverVol;
  CAN_2B5_Frame_Dat.PTCRlngCtr = CAN_TxRx_Para_Info.PTCRlngCtr;
  
  CAN_2B5_Frame_Dat.PTCPCBTemMsb = ((((word)(CAN_TxRx_Para_Info.PTCPCBTem+40)*2)>>8)&0x01);
  CAN_2B5_Frame_Dat.PTCPCBTemLsb = (((word)(CAN_TxRx_Para_Info.PTCPCBTem+40)*2)&0xFF);         //新增
  CAN_2B5_Frame_Dat.PTCActuCrntMsb = ((((word)CAN_TxRx_Para_Info.PTCActuCrnt+200)>>2)&0xFF);
  CAN_2B5_Frame_Dat.PTCActuCrntLsb = (((word)CAN_TxRx_Para_Info.PTCActuCrnt+200)&0x03);       //新增
  
  CAN_2B5_Frame_Dat.PTCActuPwr = CAN_TxRx_Para_Info.PTCActuPwr; 
  CAN_2B5_Frame_Dat.PTCUnderVol = CAN_TxRx_Para_Info.PTCUnderVol;
  CAN_2B5_Frame_Dat.PTCIGBTSts = CAN_TxRx_Para_Info.PTCIGBTSts;
  CAN_2B5_Frame_Dat.PTCHVLckSts = CAN_TxRx_Para_Info.PTCHVLckSts;
  CAN_2B5_Frame_Dat.PTCIGBTOverHeat = CAN_TxRx_Para_Info.PTCIGBTOverHeat;
  CAN_2B5_Frame_Dat.PTCDTC = CAN_TxRx_Para_Info.PTCDTC;                         //新增
  CAN_2B5_Frame_Dat.reserved0 = 0;
  CAN_2B5_Frame_Dat.PTCChksm = Crc_CRC8_Cal((byte*)&CAN_2B5_Frame_Dat,7);
}


/*
***********************************************************************************************************
** 函数名称：CAN_2B6_Dat_Update_Fuc
** 函数描述：CAN数据帧2B6更新
** 输入参数：无
** 返回值  ：无
***********************************************************************************************************
*/
static void CAN_2B6_Dat_Update_Fuc(void)
{
  if(CAN_TxRx_Para_Info.PTCActuPWMDuty>100) CAN_TxRx_Para_Info.PTCActuPWMDuty=100;
  
  if(CAN_TxRx_Para_Info.PTCOtptTem>200) CAN_TxRx_Para_Info.PTCOtptTem=200;
  else if(CAN_TxRx_Para_Info.PTCOtptTem<-40) CAN_TxRx_Para_Info.PTCOtptTem=-40;
  
  if(CAN_TxRx_Para_Info.PTCIGBTTem>200) CAN_TxRx_Para_Info.PTCIGBTTem=200;
  else if(CAN_TxRx_Para_Info.PTCIGBTTem<-40) CAN_TxRx_Para_Info.PTCIGBTTem=-40;
  
  if(CAN_TxRx_Para_Info.PTCActuVol<100) CAN_TxRx_Para_Info.PTCActuVol=100;
  else if(CAN_TxRx_Para_Info.PTCActuVol>600) CAN_TxRx_Para_Info.PTCActuVol=600;
  
  if(CAN_TxRx_Para_Info.PTCInptTem>200) CAN_TxRx_Para_Info.PTCInptTem=200;
  else if(CAN_TxRx_Para_Info.PTCInptTem<-40) CAN_TxRx_Para_Info.PTCInptTem=-40;
  
  CAN_2B6_Frame_Dat.PTCIGBTOvallSts = CAN_TxRx_Para_Info.PTCIGBTOvallSts.PTC_IGBTSts;
  CAN_2B6_Frame_Dat.PTCSecRlngCtr = CAN_TxRx_Para_Info.PTCSecRlngCtr;
  CAN_2B6_Frame_Dat.PTCActuPWMDuty = ((word)CAN_TxRx_Para_Info.PTCActuPWMDuty*2);
  CAN_2B6_Frame_Dat.PTCActuVolMsb = (byte)((((word)CAN_TxRx_Para_Info.PTCActuVol-100)*2)>>2);
  CAN_2B6_Frame_Dat.PTCActuVolLsb = (byte)((((word)CAN_TxRx_Para_Info.PTCActuVol-100)*2)&0x3);
  CAN_2B6_Frame_Dat.PTCNTCSts = CAN_TxRx_Para_Info.PTCNTCSts.NTC_Sts;
  CAN_2B6_Frame_Dat.PTCIGBTTemMsb = (((((word)(CAN_TxRx_Para_Info.PTCIGBTTem+40))*2)>>6)&0x07);
  CAN_2B6_Frame_Dat.PTCIGBTTemLsb = ((((word)(CAN_TxRx_Para_Info.PTCIGBTTem+40))*2)&0x3F);
  CAN_2B6_Frame_Dat.PTCInptTemMsb = (((((word)(CAN_TxRx_Para_Info.PTCInptTem+40))*2)>>7)&0x03);
  CAN_2B6_Frame_Dat.PTCInptTemLsb = (byte)((((word)(CAN_TxRx_Para_Info.PTCInptTem+40))*2)&0x7F);
  CAN_2B6_Frame_Dat.PTCOtptTemMsb = ((((word)(CAN_TxRx_Para_Info.PTCOtptTem+40))*2)>>8)&0x1;
  CAN_2B6_Frame_Dat.PTCOtptTemLsb = ((((word)(CAN_TxRx_Para_Info.PTCOtptTem+40))*2)&0xFF);
  CAN_2B6_Frame_Dat.reserved0 = 0;
  CAN_2B6_Frame_Dat.PTCSecChksm = Crc_CRC8_Cal((byte*)&CAN_2B6_Frame_Dat,7);
}

/*
***********************************************************************************************************
** 函数名称：CAN_2A0_Dat_Update_Fuc
** 函数描述：CAN数据帧2A0更新
** 输入参数：无
** 返回值  ：无
***********************************************************************************************************
*/
static void CAN_2A0_Dat_Update_Fuc(void)
{
  CAN_TxRx_Para_Info.BMSReqPwr = CAN_2A0_Frame_Dat.BMSReqPwr;
  CAN_TxRx_Para_Info.BMSReqRlngCtr = CAN_2A0_Frame_Dat.BMSReqRlngCtr;
  CAN_TxRx_Para_Info.BMSEnbPTC = CAN_2A0_Frame_Dat.BMSEnbPTC;
  CAN_TxRx_Para_Info.BMSChksm = CAN_2A0_Frame_Dat.BMSChksm;
  CAN_TxRx_Para_Info.BMSInptClntTem = (word)((word)((word)CAN_2A0_Frame_Dat.BMSInptClntTemMsb<<8)|CAN_2A0_Frame_Dat.BMSInptClntTemLsb); 
  CAN_TxRx_Para_Info.BMSReqMd = CAN_2A0_Frame_Dat.BMSReqMd;
  CAN_TxRx_Para_Info.BMSReqPWM = CAN_2A0_Frame_Dat.BMSReqPWM;
  CAN_TxRx_Para_Info.BMSReqTapPos = CAN_2A0_Frame_Dat.BMSReqTapPos;
  
  //计数器返回
  CAN_TxRx_Para_Info.PTCRlngCtr = CAN_TxRx_Para_Info.BMSReqRlngCtr+1;
  CAN_TxRx_Para_Info.PTCSecRlngCtr=CAN_TxRx_Para_Info.BMSReqRlngCtr+1;
}


/*
***********************************************************************************************************
** 函数名称：CAN_295_Dat_Update_Fuc
** 函数描述：CAN数据帧295更新
** 输入参数：无
** 返回值  ：无
***********************************************************************************************************
*/
static void CAN_295_Dat_Update_Fuc(void)
{
  unsigned long packVolt;
  
  packVolt = ((word)((word)CAN_295_Frame_Dat.BMSPackVol_h6Msb*256+CAN_295_Frame_Dat.BMSPackVol_h6Lsb));
  CAN_TxRx_Para_Info.BMSPackVol_h6 = packVolt *25;
  
  CAN_TxRx_Para_Info.PID_Kp=CAN_295_Frame_Dat.BMS_Kp;
  CAN_TxRx_Para_Info.PID_Ki=CAN_295_Frame_Dat.BMS_Ki;
}


/*
***********************************************************************************************************
** 函数名称：CAN_Frame_Tx_Fuc
** 函数描述：CAN数据帧发送函数
** 输入参数：无
** 返回值  ：TRUE:发送成功,FALSE:发送失败
***********************************************************************************************************
*/
Bool CAN_Frame_Tx_Fuc(void)
{
  Bool err = FALSE;
  CAN_MSG_INFO canFrameTx1,canFrameTx2;
  
  canFrameTx1.IDE = 0;      /*设置数据帧为标准帧        */
  canFrameTx1.RTR = 0;      /*设置帧类型为数据帧        */
  canFrameTx1.len = 8;      /*设置数据帧长度为8字节     */
  canFrameTx1.prty = 0;     /*设置数据帧优先级为0       */
  canFrameTx1.msgId = 0x2B5;/*设置数据帧ID为0x2B5       */
  CAN_2B5_Dat_Update_Fuc(); /*更新数据内容              */
  *((CAN_FRAM_DAT*)&canFrameTx1.data) = *((CAN_FRAM_DAT*)&CAN_2B5_Frame_Dat); /*设置数据帧发送的数据内容  */
  
  canFrameTx2.IDE = 0;      /*设置数据帧为标准帧        */
  canFrameTx2.RTR = 0;      /*设置帧类型为数据帧        */
  canFrameTx2.len = 8;      /*设置数据帧长度为8字节     */
  canFrameTx2.prty = 0;     /*设置数据帧优先级为0       */
  canFrameTx2.msgId = 0x2B6;/*设置数据帧ID为0x2B6       */
  CAN_2B6_Dat_Update_Fuc(); /*更新数据内容              */
  *((CAN_FRAM_DAT*)&canFrameTx2.data) = *((CAN_FRAM_DAT*)&CAN_2B6_Frame_Dat); /*设置数据帧发送的数据内容  */

  err = CAN_Msg_Tx_Fuc(canFrameTx1);  /*发送数据帧1           */
  if(err == TRUE)
  {
    err = CAN_Msg_Tx_Fuc(canFrameTx2);  /*发送数据帧2           */
  }  
  return err;
}

/*
***********************************************************************************************************
** 函数名称：CAN_Frame_Rx_Fuc
** 函数描述：CAN数据帧接收函数
** 输入参数：无
** 返回值  ：无
***********************************************************************************************************
*/
void CAN_Frame_Rx_Fuc(void)
{
  Bool err = FALSE;
  CAN_MSG_INFO canFrameRx;

  err = CAN_Msg_Rx_Fuc(&canFrameRx);
  if(err==TRUE)
  {
    if(canFrameRx.msgId==0x2A0)
    {
      *((CAN_FRAM_DAT*)&CAN_2A0_Frame_Dat) = *((CAN_FRAM_DAT*)&canFrameRx.data); /*读取接收到的数据内容     */
      
      CAN_2A0_Dat_Update_Fuc();
    } 
    else if(canFrameRx.msgId==0x295)
    {
      *((CAN_FRAM_DAT*)&CAN_295_Frame_Dat) = *((CAN_FRAM_DAT*)&canFrameRx.data); /*读取接收到的数据内容     */
      CAN_295_Dat_Update_Fuc();
    }
  }
}
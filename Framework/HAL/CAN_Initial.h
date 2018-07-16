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
#ifndef  __CAN_INITIAL__H__
#define  __CAN_INITIAL__H__

/*
***********************************************************************************************************
** include file
***********************************************************************************************************
*/
#include <hidef.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "includes.h"
/*
***********************************************************************************************************
** Variable and the Macro define
***********************************************************************************************************
*/
typedef struct  
{
  byte   data[8]; /*数据内容               */
}CAN_FRAM_DAT;    /*CAN 数据帧数据         */


typedef struct      
{
    dword msgId;  /*CAN数据帧ID            */
    Bool  RTR;    /*远程帧标志             */
    Bool  IDE;    /*扩展帧标志             */
    byte  data[8];/*数据内容               */
    byte  len;    /*数据长度               */
    byte  prty;   /*发送数据优先级         */
}CAN_MSG_INFO;    /*CAN 数据帧信息         */

/*通过CAN线发送/接收的数据           */
typedef struct 
{
  unsigned char  PTCOverVol           :1;   /*PTC过压                     */
  unsigned char  PTCOverCrnt          :1;   /*PTC过流                     */
  unsigned char  PTCOverHeat          :1;   /*PTC超温                     */
  unsigned char  PTCIGBTOverHeat      :1;   /*PTC驱动IGBT超温             */
  unsigned char  PTCHVLckSts          :1;   /*PTC HV锁止                  */
  unsigned char  PTCIGBTSts           :1;   /*PTC的IGBT状态               */
  unsigned char  PTCUnderVol          :1;   /*PTC欠压                     */
  unsigned char  BMSEnbPTC            :1;   /*PTC工作许可                 */
  unsigned char  PTCRlngCtr;                /*PTC波动计数                 */
  unsigned char  PTCSecRlngCtr;             /*PTC波动计数                 */
  union
  {
    unsigned char PTC_IGBTSts         :2;   /*PTC驱动的IGBT状态信息       */
    struct
    {
      byte IGBT_1_Sts     :1;               /*IGBT1状态信息;0:正常,1:损坏 */
      byte IGBT_2_Sts     :1;               /*IGBT2状态信息;0:正常,1:损坏 */
    }Bits;
  }PTCIGBTOvallSts;
  union
  {
    unsigned char NTC_Sts             :3;   /*传感器状态                  */
    struct
    {
      byte IGBT_NTC_Sts     :1;             /*IGBT传感器信息;0:正常,1:损坏*/
      byte PTC_IN_NTC_Sts   :1;             /*PTC输入传感器信息;0:正常,1:损坏 */
      byte PTC_OUT_NTC_Sts  :1;             /*PTC输出传感器信息;0:正常,1:损坏 */
    }Bits;
  }PTCNTCSts;
  unsigned char  PTCDTC               :4;
  int  PTCInptTem;                /*PTC输入温度                 */
  int  PTCOtptTem;                /*PTC输出温度                 */
  int  PTCIGBTTem;                /*PTC驱动IGBT温度             */
  unsigned int   PTCActuVol ;               /*PTC驱动电压                 */
  unsigned char  PTCChksm;                  /*PTC校验值                   */
  unsigned char  PTCActuPWMDuty;            /*PTC驱动PWM占空比            */
  unsigned int   PTCActuCrnt;
  unsigned char  PTCSecChksm;               /*PTC校验值                   */
  unsigned int   BMSReqPwr;                 /*驱动电源PWM占空比           */
  unsigned char  BMSReqRlngCtr;             /*电池管理计数次数请求        */ 
  unsigned char  BMSReqTapPos;
  unsigned int   BMSInptClntTem;
  unsigned char  BMSReqPWM;
  unsigned char  BMSReqMd;
  unsigned char  BMSChksm;  
  unsigned long  BMSPackVol_h6;             /*扩大100倍                   */               
  unsigned int   PTCActuPwr;                /*PTC电源                     */
  int   PTCPCBTem;
  
  //测试接口
  byte PID_Kp;
  byte PID_Ki;
  
  unsigned char  CanBusErr;                 /*CAN 总线错误标志            */
}CAN_TxRx_PARA;



typedef struct 
{
  byte  PTCPCBTemMsb        :1; 
  byte  PTCOverHeat         :1; 
  byte  PTCOverCrnt         :1;            
  byte  PTCOverVol          :1;
  byte  PTCRlngCtr          :4;
  byte  PTCPCBTemLsb;
  byte  PTCActuCrntMsb;
  byte  reserved0           :6;
  byte  PTCActuCrntLsb      :2;
  word  PTCActuPwr;
  byte  PTCDTC              :4;
  byte  PTCUnderVol         :1;
  byte  PTCIGBTSts          :1;
  byte  PTCHVLckSts         :1;
  byte  PTCIGBTOverHeat     :1;
  byte  PTCChksm;                           
}CAN_2B5_DAT;


typedef struct 
{
  byte  PTCIGBTOvallSts       :2;
  byte  reserved0             :2;
  byte  PTCSecRlngCtr         :4;
  byte  PTCActuPWMDuty;
  byte  PTCActuVolMsb;
  byte  PTCIGBTTemMsb         :3;
  byte  PTCNTCSts             :3;
  byte  PTCActuVolLsb         :2;
  byte  PTCInptTemMsb         :2;
  byte  PTCIGBTTemLsb         :6;
  byte  PTCOtptTemMsb         :1;
  byte  PTCInptTemLsb         :7;
  byte  PTCOtptTemLsb;
  byte  PTCSecChksm;    
}CAN_2B6_DAT;

typedef struct 
{
  byte  BMSReqTapPos        :4;             /*BMS请求位置                 */
  byte  BMSReqRlngCtr       :4;             /*电池管理计数次数请求        */
  word  BMSReqPwr;                          /*驱动电源PWM占空比           */
  byte  BMSInptClntTemMsb   :1;
  byte  BMSReqPWM           :7;
  byte  BMSInptClntTemLsb;             
  byte  reserved0           :5;             /*保留                        */
  byte  BMSEnbPTC           :1;             /*电池管理计数次数请求        */
  byte  BMSReqMd            :2;
  byte  reserved2;                          /*保留                        */
  byte  BMSChksm;
}CAN_2A0_DAT;

typedef struct 
{ 
  byte  BMSPackVol_h6Msb:4;                 /*电压高字节部分              */
  byte  reserved0       :4;                 /*保留                        */
  byte  BMSPackVol_h6Lsb;                   /*电压低字节部分              */
  byte  BMS_Kp;                             /*保留                        */
  byte  BMS_Ki;                             /*保留                        */
  byte  reserved3;                          /*保留                        */
  byte  reserved4;                          /*保留                        */
  byte  reserved5;                          /*保留                        */
  byte  reserved6;                          /*保留                        */
}CAN_295_DAT;

typedef struct 
{
  word  BMSReqPwr;                          /*驱动电源PWM占空比           */
  byte  BMSEnbPTC;                          /*PTC工作许可                 */
}CAN_2A0_EEP;

extern  CAN_TxRx_PARA     CAN_TxRx_Para_Info;
extern  CAN_2B5_DAT       CAN_2B5_Frame_Dat;
extern  CAN_2B6_DAT       CAN_2B6_Frame_Dat;
extern  CAN_2A0_DAT       CAN_2A0_Frame_Dat;
extern  CAN_295_DAT       CAN_295_Frame_Dat;
extern  CAN_2A0_EEP       CAN_2A0_EEprom_Dat;

/*
***********************************************************************************************************
** Assert the function
***********************************************************************************************************
*/
Bool CAN_Frame_Tx_Fuc(void);
void CAN_Frame_Rx_Fuc(void);

#endif

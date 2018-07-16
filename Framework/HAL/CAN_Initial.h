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
  byte   data[8]; /*��������               */
}CAN_FRAM_DAT;    /*CAN ����֡����         */


typedef struct      
{
    dword msgId;  /*CAN����֡ID            */
    Bool  RTR;    /*Զ��֡��־             */
    Bool  IDE;    /*��չ֡��־             */
    byte  data[8];/*��������               */
    byte  len;    /*���ݳ���               */
    byte  prty;   /*�����������ȼ�         */
}CAN_MSG_INFO;    /*CAN ����֡��Ϣ         */

/*ͨ��CAN�߷���/���յ�����           */
typedef struct 
{
  unsigned char  PTCOverVol           :1;   /*PTC��ѹ                     */
  unsigned char  PTCOverCrnt          :1;   /*PTC����                     */
  unsigned char  PTCOverHeat          :1;   /*PTC����                     */
  unsigned char  PTCIGBTOverHeat      :1;   /*PTC����IGBT����             */
  unsigned char  PTCHVLckSts          :1;   /*PTC HV��ֹ                  */
  unsigned char  PTCIGBTSts           :1;   /*PTC��IGBT״̬               */
  unsigned char  PTCUnderVol          :1;   /*PTCǷѹ                     */
  unsigned char  BMSEnbPTC            :1;   /*PTC�������                 */
  unsigned char  PTCRlngCtr;                /*PTC��������                 */
  unsigned char  PTCSecRlngCtr;             /*PTC��������                 */
  union
  {
    unsigned char PTC_IGBTSts         :2;   /*PTC������IGBT״̬��Ϣ       */
    struct
    {
      byte IGBT_1_Sts     :1;               /*IGBT1״̬��Ϣ;0:����,1:�� */
      byte IGBT_2_Sts     :1;               /*IGBT2״̬��Ϣ;0:����,1:�� */
    }Bits;
  }PTCIGBTOvallSts;
  union
  {
    unsigned char NTC_Sts             :3;   /*������״̬                  */
    struct
    {
      byte IGBT_NTC_Sts     :1;             /*IGBT��������Ϣ;0:����,1:��*/
      byte PTC_IN_NTC_Sts   :1;             /*PTC���봫������Ϣ;0:����,1:�� */
      byte PTC_OUT_NTC_Sts  :1;             /*PTC�����������Ϣ;0:����,1:�� */
    }Bits;
  }PTCNTCSts;
  unsigned char  PTCDTC               :4;
  int  PTCInptTem;                /*PTC�����¶�                 */
  int  PTCOtptTem;                /*PTC����¶�                 */
  int  PTCIGBTTem;                /*PTC����IGBT�¶�             */
  unsigned int   PTCActuVol ;               /*PTC������ѹ                 */
  unsigned char  PTCChksm;                  /*PTCУ��ֵ                   */
  unsigned char  PTCActuPWMDuty;            /*PTC����PWMռ�ձ�            */
  unsigned int   PTCActuCrnt;
  unsigned char  PTCSecChksm;               /*PTCУ��ֵ                   */
  unsigned int   BMSReqPwr;                 /*������ԴPWMռ�ձ�           */
  unsigned char  BMSReqRlngCtr;             /*��ع��������������        */ 
  unsigned char  BMSReqTapPos;
  unsigned int   BMSInptClntTem;
  unsigned char  BMSReqPWM;
  unsigned char  BMSReqMd;
  unsigned char  BMSChksm;  
  unsigned long  BMSPackVol_h6;             /*����100��                   */               
  unsigned int   PTCActuPwr;                /*PTC��Դ                     */
  int   PTCPCBTem;
  
  //���Խӿ�
  byte PID_Kp;
  byte PID_Ki;
  
  unsigned char  CanBusErr;                 /*CAN ���ߴ����־            */
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
  byte  BMSReqTapPos        :4;             /*BMS����λ��                 */
  byte  BMSReqRlngCtr       :4;             /*��ع��������������        */
  word  BMSReqPwr;                          /*������ԴPWMռ�ձ�           */
  byte  BMSInptClntTemMsb   :1;
  byte  BMSReqPWM           :7;
  byte  BMSInptClntTemLsb;             
  byte  reserved0           :5;             /*����                        */
  byte  BMSEnbPTC           :1;             /*��ع��������������        */
  byte  BMSReqMd            :2;
  byte  reserved2;                          /*����                        */
  byte  BMSChksm;
}CAN_2A0_DAT;

typedef struct 
{ 
  byte  BMSPackVol_h6Msb:4;                 /*��ѹ���ֽڲ���              */
  byte  reserved0       :4;                 /*����                        */
  byte  BMSPackVol_h6Lsb;                   /*��ѹ���ֽڲ���              */
  byte  BMS_Kp;                             /*����                        */
  byte  BMS_Ki;                             /*����                        */
  byte  reserved3;                          /*����                        */
  byte  reserved4;                          /*����                        */
  byte  reserved5;                          /*����                        */
  byte  reserved6;                          /*����                        */
}CAN_295_DAT;

typedef struct 
{
  word  BMSReqPwr;                          /*������ԴPWMռ�ձ�           */
  byte  BMSEnbPTC;                          /*PTC�������                 */
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

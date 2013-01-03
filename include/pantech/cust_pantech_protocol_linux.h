#ifndef __CUST_PANTECH_PROTOCOL_LINUX_H__
#define __CUST_PANTECH_PROTOCOL_LINUX_H__

/* ========================================================================
FILE: cust_pantech_protocol_linux.h

Copyright (c) 2010 by PANTECH Incorporated.  All Rights Reserved.

USE the format "FEATURE_P_VZW_XXXX"
=========================================================================== */ 

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---      ----------------------------------------------------------
111006     swchoi  add FEATURE_P_VZW_DS_IMS_PROVIDER
111006     swchoi  add FEATURE_P_VZW_DS_IMS_SERVICE
111006     swchoi  add FEATURE_P_VZW_DS_IMS_PST_UICC


===========================================================================*/

/*===========================================================================
Feature 처리 MANUAL

-. 본 파일 변경 시 반드시 file header에 history를 기입하도록 한다.

-. system vob에 공통 처리되는 Feature는 System VOB Features section에 둔다.
   system vob에 처리되는 Feature는 FL과 상의하여 적용하도록 한다.

-. feature이름은 FEATURE_P_VZW_[CS,DATA,LTE,UICC,GPS or LBS, ...]_[name] 으로 처리한다.
   각각의 section에 두도록 한다.

-. feature에는 author, date, summary를 comment한다.
   본 파일은 feature specification을 대체하므로
   summary는 feature로 구현된 소스를 보지 않더라도 feature의 내용을 대강 파악할 수 있을 정도로 자세하게 적는다.

-. JAVA source에는 comment로 feature 처리된 사항을 남긴다.

===========================================================================*/

/*===========================================================================
    System VOB Features
===========================================================================*/
/*
20110114, 이상욱
Summary:
  -. VZW net pref 미 적용 시 handset capability가 맞지 않아 linux에서 modem state가
	 power off로 되어 착발신 되지 않음.
  -. network mode menu에서 설정이 제대로 되지 않음.
20111017, 박규태 
  -. 우선 Apache만 적용   
*/
//#define FEATURE_P_VZW_SETTING_NET_PREF

/*
20110114, 이상욱
summary:
  -. APACHE는 Voice로 CDMA RAT를 사용하고 있음. phone factory에서 voice Phone 생성이
	 default GSM phone인데 이를 CDMA Phone이 생성되도록 함.
  -. LTE+WCDMA 모델의 경우는 해당되지 않는 내용임.
20110503, drgnyp
  -. RTRE configuration의 default value가 NV로 되어있어,
	 Phone Process에서 error가 발생하는 만약의 경우, RTRE가 NV로 초기화가 되어
	 사용자가 다시 RUIM으로 변경할 수 없는 문제가 있어 default value 수정함.
*/
#define FEATURE_P_VZW_SETTING_CDMA_PHONE

/*
  20120209, ï¿½ï¿½ï¿½ï¿½Ã¶ 
  summary:
  -.CDMA+LTEï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½â¿¡ ï¿½ï¿½ï¿½ï¿½ ril,telephoney ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ EF46Lï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï°ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ö±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 
*/
//#define FEATURE_LGU_CP_COMMON_GLOBAL_SD
//#define FEATURE_VZW_CP_COMMON_CDMA_EVDO_LTE_SD  //LTE only mode : GSM phone type, CD_EV_LTE : CDMA phone type setting

/*===========================================================================
    Interface Features
===========================================================================*/

/*
20110118, hksong
summary:
  -. 히든메뉴 구현을 위한 히든코드 Interface
  -. 기존 내수모델의 FEATURE_KT_CP_HIDDEN_CODE 과 동일
  -. 다이얼과 관련된 specialcharSequence 에 import pantechhiddenmenu 파일 해준다 
  -. 아파치 source 참고 amj 2011 11 01
*/
#define FEATURE_P_VZW_CP_HIDDEN_CODE
#define FEATURE_P_VZW_CP_MANAGER_DAEMON_INTERFACE
#ifdef FEATURE_P_VZW_CP_MANAGER_DAEMON_INTERFACE
#define FEATURE_P_VZW_CP_CPMGRIF_QMI_CLIENT
#define FEATURE_P_VZW_CP_OEM_QMI_ACCESS
#define FEATURE_P_VZW_CP_OEM_COMMANDS_WITH_QMI

/*
20110114, 이상욱
summary:
  -. process가 다른 class간에 IPC통신이 가능하도록 하는 AIDL 및 interface manager 적용.
*/
#define FEATURE_P_VZW_COMMON_TELEPHONY_IF

#define FEATURE_P_VZW_CP_MANAGER_GW_DAEMON_INTERFACE
#define FEATURE_P_VZW_CP_GW_COMMON_TELEPHONY_IF
#define FEATURE_P_VZW_CP_LOCAL_DB_GW_ACCESS_CPMGRIF
#endif /* FEATURE_P_VZW_CP_MANAGER_DAEMON_INTERFACE */

/*BKS_20111013 Debug Screen FEATURE*/
#define FEATURE_P_VZW_COMMON_DEBUG_SCREEN

#define FEATURE_P_VZW_CP_COMMON_MSM_NVIO_WITH_QMI

/*
20110130, 이상욱
summary:
  -. eng. nam menu
*/
//#define FEATURE_P_VZW_CS_ENG_NAM

/*
20110120, 이상욱
summary:
  -. cpmgrif.c daemon을 통한 NV Access 
  */ 
//#define FEATURE_P_VZW_INTERFACE_NV_ACCESS


/*===========================================================================
    1x Voice Features
===========================================================================*/
#define FEATURE_P_VZW_CP_1X_LOCAL_DB

#ifdef FEATURE_P_VZW_CP_1X_LOCAL_DB
#define FEATURE_P_VZW_CP_1X_LOCAL_DB_WITH_QMI
#endif /* FEATURE_P_VZW_CP_1X_LOCAL_DB */

#ifdef FEATURE_P_VZW_COMMON_DEBUG_SCREEN
#define FEATURE_P_VZW_CP_1X_DEBUG_SCREEN
#endif /* FEATURE_P_VZW_COMMON_DEBUG_SCREEN */

/*
20110211, 이상욱
summary:
  -. P REV Change Menu  설정
*/
#define FEATURE_P_VZW_SETTING_P_REV_CHANGE_MENU

/*
20110217, 이상욱
summary:
  -. SW Test Setting Menu
*/
#define FEATURE_P_VZW_SETTING_SW_TEST_MENU

/*
20110217, 이상욱
summary:
  -. Test Call 구현(8k, 13k, markov, etc) 
*/
#define FEATURE_P_VZW_CS_TEST_CALL

/*
20110302, 이상욱
summary:
  -. NBPCD, HBPCD
*/
#define FEATURE_P_VZW_CS_PLUS_CODE_DIALING

/*
20110304, 이상욱
summary:
  -. NBPCD
*/
#define FEATURE_P_VZW_CS_NBPCD

/*
20110412 이상욱
summary:
  - MSM/MDM MRU Table을 삭제하기 위한 Menu 제공
  - field test를 위해 필요함.
*/
#define FEATURE_P_VZW_CS_MRU_CLR_MENU

/*
20110215, 이상욱
summary:
  -. CDG2 Setting Menu 
*/
#define FEATURE_P_VZW_SETTING_CDG2_MENU

/*
20110118, 이상욱
summary:
  -. VZW Wireless Priority Service
*/
#define FEATURE_P_VZW_CS_WPS

/*
20110207, 이상욱
summary:
  -. A Key Input process
*/
#define FEATURE_P_VZW_CS_AKEY_INPUT_PROC

/*
20110214, 이상욱
summary:
  -. cm_ph_cmd_nam_sel
*/
#define FEATURE_P_VZW_CS_NAM_SEL

/*
20110330, 이상욱
summary:
   -. 망으로부터 Lock Order를 수신 시 내용을 표시
*/
#define FEATURE_P_VZW_CS_AUTH_REJ_DISPLAY

/*
20110404, 이상욱
summary:
   -. QCRIL RIL_UNSOL Log 순서가 맞지 않아 조절함.
*/
#define FEATURE_P_VZW_CS_QCRIL_LOG

/*
20110429 고화영 
summary:
  - NV, SIM mode 모두 ERI 동작 시킴.
  - ERI 적용에 의한 home system 정의 
*/
#define FEATURE_P_VZW_CS_ERI


/*
20110513 이상욱  
summary:
  -1x Keypad PST
*/
#define FEATURE_P_VZW_CS_KEYPAD_PST

/* 
20110530 이상욱
summary:
  -In service 상태에서 Network name 으로 NO Service display되는 문제
  - ERI off 시에도 ERI 내용이 SPN string으로 display되는 문제 
*/
#define FEATURE_P_VZW_CS_ROAMING_INDICATOR


/*
20110624 이상욱  
summary:
  - 한국에서 시간정보가 동티모르로 표시되는 현상 수정
  - 동티모르 대신 무조건 한국 표시하도록 수정
*/
#define FEATURE_P_VZW_CS_TIMEZONE_FIX

/*
20110321 이상욱
- SDM CDMA
*/
#define FEATURE_P_VZW_CS_SDM_CDMA

/*
info : SDM의 Voice/Data RSSI, Current Network System,
       Roaming Status 항목을 지원
when : 20110324
who : drgnyp
*/
#define FEATURE_P_VZW_CP_SDM_DIAG_MON


/*
20110328 이상욱
- Verizon System Selection Requirement
*/
#define FEATURE_P_VZW_CS_SYSTEM_SELECTION

/*
info : PST Tool 에서 Network mode pref 항목의 get/set을 지원
when : 20110330
who : drgnyp
*/
#define FEATURE_P_VZW_CP_PST_NET_PREF

/*
20120105
summary:
  - QCRIL LOG format 문제로 Message가 d/p되지 않는 현상 수정
*/
#define FEATURE_P_VZW_CS_QCRIL_LOG_BUG_FIX

/*
20110325, 고화영
summary:
  -. 6.2	AUTO CUSTOMER WARRANTY DATE CODE 
*/
#define FEATURE_P_VZW_WARRANTY_DATE_CODE

/*
20110318, 이상욱
summary:
  -. CDG2 Display Info record에 "RES"로 올 경우 private number로 표시되어야 하나 unknown number로 표시됨.
  -. CDG2 Display Info, Ext Displat Info record 모두 올 경우 cnap가 display Indo 처럼 팝업 발생하는 문제.  
*/
#define FEATURE_P_VZW_CS_PROCESS_DISPLAY_INFO


/*
kim.jeongmin2 PS2-AJANTECH 120224
Incoming Call Å½ AWIM Signal Info RIL  . (PS5 Call UI  Ã»)
*/
#define FEATURE_P_VZW_CS_SIGNAL_REC_EXCLUDE_AWIM_LINUX

/*
kim.jeongmin2 PS2-AJANTECH 120320
summary:
 -. Qualcomm 기본 코드에서 RIL_UNSOL_CALL_RING Event 올리지 않아 수정함.
 */
#define FEATURE_P_VZW_CP_SEND_RIL_CALL_RING_LINUX

/*
20120210 BANG KYOUNGSU
RSSI BAR 보정 
*/
#define FEATURE_P_VZW_COMMON_RSSI
#define FEATURE_P_VZW_CP_GW_RSSI_LEVEL

/*
kim.jeongmin2 PS2-AJANTECH 120330
summary:
  -. Date & Time 설정에서 시간 수동 설정하더라도 Network Time을 알 수 있도록 함수를 추가함.
*/
#define FEATURE_P_VZW_GET_NETWORK_TIME

/*
kim.jeongmin2 PS2-AJANTECH 0120612
summary:
  -. qcril_qmi_voice_voip_lock_overview, qcril_qmi_voice_voip_unlock_overview
*/
#define FEATURE_P_VZW_QMI_VOICE_MUTEX_BUG_FIX

/*===========================================================================
    Data Service Features
===========================================================================*/


/*===========================================================================
    IMS Features
===========================================================================*/
 
/*
20110322, swchoi
summary:
  - IMSï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ã¿ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½.
  - Å¸ï¿½ï¿½ï¿½Ã¿ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï±ï¿½ï¿½ï¿½ï¿½ï¿½ frameworksï¿½ï¿½ ï¿½ï¿½ï¿½Ô½ï¿½Å´
  - IMSï¿½ï¿½ ï¿½ï¿½ï¿½Ã°ï¿½ ï¿½ï¿½ï¿½Ã¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ï¿½ï¿½ï¿½Ï±â¶§ï¿½ï¿½ï¿½ï¿½ BootUpListenerï¿½ï¿½ ï¿½ï¿½ï¿½Ô½ï¿½Å´.

Android.mk (LINUX\android\frameworks\base) : ï¿½ï¿½ï¿½ï¿½
app_icon.png (LINUX\android\pantech\providers\SkyIMSProvider\res\drawable) : ï¿½ï¿½ï¿½ï¿½
strings.xml (LINUX\android\pantech\providers\SkyIMSProvider\res\values) : ï¿½ï¿½ï¿½ï¿½
strings.xml (LINUX\android\pantech\providers\SkyIMSProvider\res\values-ko) : ï¿½ï¿½ï¿½ï¿½
Android.mk (LINUX\android\pantech\providers\SkyIMSProvider) : ï¿½ï¿½ï¿½ï¿½
AndroidManifest.xml (LINUX\android\pantech\providers\SkyIMSProvider) : ï¿½ï¿½ï¿½ï¿½
BootUpListener.java (LINUX\android\pantech\providers\SkyIMSProvider\src\com\pantech\provider\skyims) : ï¿½ï¿½ï¿½ï¿½
SkyIMS.java (LINUX\android\pantech\frameworks\skyims\java\com\pantech\provider\skyims) : ï¿½ï¿½ï¿½ï¿½
SkyIMSProvider.java (LINUX\android\pantech\providers\SkyIMSProvider\src\com\pantech\provider\skyims) : ï¿½ï¿½ï¿½ï¿½
*/
//#define FEATURE_P_VZW_DS_IMS_PROVIDER

  
/*
20110404, swchoi
summary:
  - ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ IMSï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Contents Providerï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½  system propertiesï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
  - IMS Contents Provider ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ system properties update

AndroidManifest.xml (LINUX\android\packages\apps\Phone) : ï¿½ï¿½ï¿½ï¿½
SkyIMSService.java (LINUX\android\packages\apps\Phone\src\com\android\phone) : ï¿½ï¿½ï¿½ï¿½

20111006 : cheetahï¿½ï¿½ imsserviceï¿½ï¿½ startï¿½ï¿½ SkyPhoneBroadcastReceiver.java ï¿½ï¿½ ï¿½Æ´ï¿½ ï¿½Æ·ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½.
                ï¿½ï¿½ï¿½ï¿½ SkyPhoneBroadcastReceiver ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½ï¿½Ç¸ï¿½ IMSï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ßºï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ê´ï¿½ï¿½ï¿½ Ã¼Å©ï¿½Ê¿ï¿½ï¿½ï¿½.
IMSStartupReceiver.java (LINUX\android\packages\apps\Phone\src\com\android\phone) : ï¿½ï¿½ï¿½ï¿½

2010.04.12 add
- ISIM EF ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½
- IMPU(PUBLIC_USER_ID), IMPI(PRIVATE_USER_ID), DOMAIN(HOME_DOMAIN_NAME) 3ï¿½ï¿½ï¿½ï¿½ efï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½.
- ï¿½ï¿½Ä¡ï¿½Îºï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Å¸ï¿½ï¿½ byte->hex->charï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½ï¿½ï¿½ï¿½
- IMPU(PUBLIC_USER_ID)ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 3ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Úµå¸¦ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½Ö¾ï¿½ arrayï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 
  IMSI-Based SIP URI, MDN-Based SIP URI, Tel URIï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½ï¿½ï¿½ï¿½. 

2010.04.24 add
- vzwimsï¿½ï¿½ IP ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½
- property_perms[] ï¿½ï¿½ imsï¿½ï¿½ï¿½ï¿½ profixï¿½ß°ï¿½ "ims."-> AID_RADIO, AID_SYSTEM ï¿½Ó¼ï¿½ï¿½ß°ï¿½  

20111006 : FEATURE_P_VZW_DS_LTE_MULTIPLE_APN ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ MobileDataStateTracker.java ï¿½ß°ï¿½ ï¿½Û¾ï¿½ ï¿½Ê¿ï¿½

MobileDataStateTracker.java (LINUX\android\frameworks\base\core\java\android\net) ï¿½ï¿½ï¿½ï¿½ ï¿½Ì¿Ï·ï¿½.
property_service.c (LINUX\android\system\core\init) : ï¿½ï¿½ï¿½ï¿½  
*/
#ifdef FEATURE_P_VZW_DS_IMS_PROVIDER  
//#define FEATURE_P_VZW_DS_IMS_SERVICE
#endif  
  
/*
20110403, swchoi
FEATURE_P_VZW_DS_IMS_PST_UICC:
  - IMSï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ UICCï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ system propertiesï¿½ï¿½ set
  - ISIMï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ê¾ï¿½ USIMï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ã³ï¿½ï¿½
  - IMSI(pantech.ims.imsi)ï¿½ï¿½ï¿½ï¿½ IMSI NAI(pantech.ims.imsi.nai)ï¿½ï¿½ï¿½ï¿½ set
  - 20110908 : ISIM EF ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö¼Ò±ï¿½ï¿½ï¿½ Ã¼Å© ï¿½ß°ï¿½.

FEATURE_P_VZW_DS_IMS_INTENT:
  - USIM ï¿½ï¿½ ISIM EF Loading È®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ APPï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ïµï¿½ï¿½ï¿½ intent broadcast

SIMRecords.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony\gsm)  : ï¿½ï¿½ï¿½ï¿½
ISIMRecords.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony\gsm)  : ï¿½ï¿½ï¿½ï¿½

*/
#ifdef FEATURE_P_VZW_DS_IMS_PROVIDER  
//#define FEATURE_P_VZW_DS_IMS_PST_UICC
//#define FEATURE_P_VZW_DS_IMS_INTENT
#endif

  /*
  20111012, swchoi
  summary:
    - LTE/eHRPD IMS Connectedï¿½ï¿½ï¿½ï¿½ PCO Dataï¿½ï¿½ Linuxï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    - P-CSCF Addr ï¿½ï¿½ DNS Addr ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï°ï¿½ IMS APPï¿½ï¿½ ï¿½ï¿½Ã» ï¿½ï¿½ ï¿½Ñ°ï¿½ï¿½ï¿½
    - IPv6 ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ã³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½.
    
  20111012 : ï¿½ï¿½ï¿½ï¿½Ä¡ï¿½ï¿½ï¿½ï¿½ RPCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ç¾ï¿½ï¿½ï¿½ ï¿½Îºï¿½ï¿½ï¿½ ï¿½Ö¼ï¿½Ã³ï¿½ï¿½- QMIï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ê¿ï¿½
    - cpmgrif.c ï¿½ï¿½ï¿½ï¿½ cpmgrif_sip_fusion_get() ï¿½Ô¼ï¿½ï¿½ï¿½ ï¿½Ö¼ï¿½Ã³ï¿½ï¿½ï¿½ï¿½ ï¿½Îºï¿½ï¿½ï¿½ï¿½ï¿½.
  
cust_pantech_protocol_linux.h (LINUX\android\pantech\include)
cpmgrif.c (LINUX\android\vendor\qcom\proprietary\cpManager\cpmgrif)
cpmgrif.h (LINUX\android\vendor\qcom\proprietary\cpManager\cpmgrif)
cpmgrnative.cpp (LINUX\android\vendor\qcom\proprietary\cpManager\cpmgrjni)
ISkyTelephony.aidl (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony\cdma)
MobileDataStateTracker.java (LINUX\android\frameworks\base\core\java\android\net)
ps_in.h (modem_proc\modem\api\datamodem)
SIPInterface.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony) : ï¿½ß°ï¿½
SkyTelephonyInterfaceManager.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony\cdma)
  */
//#define FEATURE_P_VZW_DS_IMS_SERVER_ADDR

/*===========================================================================
    eHRPD Features
===========================================================================*/
#ifdef FEATURE_P_VZW_COMMON_DEBUG_SCREEN
#define FEATURE_P_VZW_CP_EVDO_DEBUG_SCREEN
#define FEATURE_P_VZW_CP_EVDO_DB_QMI
#endif /* FEATURE_P_VZW_COMMON_DEBUG_SCREEN */


/*===========================================================================
    LTE Features
===========================================================================*/
#ifdef FEATURE_P_VZW_COMMON_DEBUG_SCREEN
#define FEATURE_P_VZW_CP_LTE_DEBUG_SCREEN
#define FEATURE_P_VZW_CP_LTE_DB_QMI
#endif /* FEATURE_P_VZW_COMMON_DEBUG_SCREEN */

/*
20110127, drgnyp
summary:
  -. VZW Type1 단말 Preferred network mode TEST MENU UI 설정
*/
#define FEATURE_P_VZW_SETTING_NET_PREF_TEST_MENU

/*
info : t_mpsr, tbsr_cdma Timer R/W 占쏙옙占쏙옙占싹깍옙 占쏙옙占싹울옙
       占쌔댐옙 sd cfg nv 占쏙옙占쏙옙 android app占쏙옙占쏙옙 R/W 占쏙옙占쏙옙占싹듸옙占쏙옙 占쏙옙占쏙옙
when : 20110317
who : drgnyp
*/
#define FEATURE_P_VZW_CP_SDM_SYSTEM_SCAN_VALUE

/*
info : Ecrio 솔루션에서 IMS PDN에 대한 제어를 위해
       T3402를 get 할 수 있도록 지원함
when : 20110524
who : drgnyp
*/
#undef FEATURE_P_VZW_CP_GET_T3402_FOR_IMS

/*
info : Ecrio ï¿½Ö·ï¿½ï¿½Ç¿ï¿½ï¿½ï¿½ IMS PDNï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½î¸¦ ï¿½ï¿½ï¿½ï¿½
       PS attach/detachï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ ï¿½Öµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
when : 20110525
who : drgnyp
*/

#define FEATURE_P_VZW_CP_PS_ATTACH_DETACH_FOR_IMS

/*===========================================================================
    LBS Features
===========================================================================*/

/*
20110118, hksong
summary:
  -. NMEA 형식중 하나인 GLL 방식의 단말지원을 위한 피쳐
*/
//#define FEATURE_P_VZW_LBS_SUPPORT_NMEA_GPGLL

/*
20110118, hksong
summary:
  -. GPS 테스트 메뉴 관련 피쳐
*/
#define FEATURE_P_VZW_LBS_GPS_TEST_MENU
#ifdef FEATURE_P_VZW_LBS_GPS_TEST_MENU
#define FEATURE_P_VZW_LBS_GPS_TEST
#define FEATURE_P_VZW_LBS_GPS_CNO_TEST
#endif

/*
20110119, hksong
summary:
  -. NV_GPS1_LOCK_I 과 위치설정(Location Setting) 을 연동하는 피쳐
  -. LocationManagerService에 위치
  -. Settings.Secure.LOCATION_PROVIDERS_ALLOWED 의 변동을 관찰하다가 변경되면 그에 따라 NV 변경하는 구조
*/
#define FEATURE_P_VZW_LBS_GPS_LOCK

/*
20110125, hksong
summary:
  -. GPS Privacy Setting 관련하여 안드로이드 단에서의 설정값을 VZW향에 맞도록 수정함
  -. Framework단 Settings쪽의 LOCATION_PROVIDERS_ALLOWED (일종의 DB Key 값임) 이용
  -. TBD: ASSISTED_GPS_ENABLED 는 Setting되지 않도록 막음. 
     GpsLocationProvider가 항상 Standalone으로 동작할 것임
  -. TBD: VzwGpsLocationProvider의 이름을 SPEC대로 "vzw_lbs"로 정했음. 추후 변경될 수 있음
     CDMAPhone.java, SecuritySettings.java
*/
#define FEATURE_P_VZW_LBS_PRIVACY_SETTING

/*
20110131, hksong
summary:
  -. Location Manager Service에서 Privacy Settings의 변경내용을 update하는데 200ms의 delay를 줌
  -. SKT 모델 적용사항을 시스템 안정성을 목적으로 반영함
*/
#define FEATURE_P_VZW_LBS_DELAY_UPDATING_PROVIDERS

/*
20110210, hksong
summary:
  -. APACHE의 system 폴더 속성이 ro 로 바뀜에 따라 데몬 파일 생성 경로를 바꾸어야 함
  -. APACHE 이전 모델과 동일하게 data/gpsone_d/ 경로를 사용하도록 수정
*/
//#define FEATURE_P_VZW_LBS_GPSONE_DMN_PATH_FIX

/*
20110308, hksong
summary:
  -. SDM GPS Methods 구현
  -. 2010 9월 요구사항 기준으로 구현
  -. \pantech\apps\OtadmExtensions\src\com\innopath\activecare\dev\oem\IdevDcMo.java
*/
#define FEATURE_P_VZW_LBS_SDM

/*
20110125, hksong
summary:
  -. Config.xml 에서 NetworkLocationProvider와 GeocodeProvider 를 강제로 추가함
  -. 추가하지 않으면 해당 Provider가 LocationManagerService에서 제대로 load되지 못함
*/
#define FEATURE_P_VZW_LBS_LOADING_PROVIDER_BUG_FIX

/*
20110318, hksong
summary:
  -. Added for LBS MMS Debug Screen
  -. local db RPC 작업에 dependent함
  -. Cpmgrjniutil.cpp, Cpmgrnative.cpp, db.h, DBInterface.java, db_xdr.c, LbsScreen.java
*/
#define FEATURE_P_VZW_LBS_DEBUG_SCREEN

/*
20110318, hksong
summary:
  -. Added for LBS MMS Debug Screen
  -. Screen 관련 피쳐
*/
//#define FEATURE_P_VZW_LBS_MMS_DEBUG_SCREEN

/*
20110323, hksong
summary:
  -. WITS solution integration
  -. build\core\Makefile 에서 system.img 만들기 직전에 VZW GPS Provider와 VZW Location Manager Service 연결함
  -. framework\base 에 Android.mk 내 LOCAL_STATIC_JAVA_LIBRARIES 에 vzwandroid 추가함
  -. solution file들은 모두 external\wits 안에 위치하도록 함
  -. 나머지 system 파일들의 변경은 WITS Location Framework Integration 문서를 따름
*/
#define FEATURE_P_VZW_LBS_WITS_SOLUTION

/*
20110425, hksong
summary:
  -. VZW Stack의 Pram 전달이 제대로 이루어지지 않는 문제 수정
*/
#define FEATURE_P_VZW_LBS_VZW_STACK_SET_PARAM

/*
20110425, hksong
summary:
  -. GPSONE_DAEMON 시작을 위한 Script 수정
  -. AGPS 동작을 위해 반드시 필요한 피쳐임
  -. 본래 system/core/rootdir/etc/ init.qcom.sh, init.qcom.rc 에 실행 코드가 있지만 실제 동작하지 않음
  -. init.qcom.sh 에서
     case "$target" in
        "apache" | "ADR8995" | "msm7630_fusion")
        start gpsone_daemon
     esac
     위와 같이 ADR8995, apache 가 추가되어야 함
  -. 기록을 위해 Feature의 흔적을 남김
*/
//#define FEATURE_P_VZW_LBS_GPSONE_DAEMON_START

/*
20110425, hksong
summary:
  -. 필드 테스트시 End Call 에서 Location Fix 정보가 보이도록 수정
*/
#define FEATURE_P_VZW_LBS_GPS_FIELD_POP_UP

/*
20110503, hksong
summary:
  -. VzwGpsProvider State Machine 수정
*/
#define FEATURE_P_VZW_LBS_STATE_MACHINE

/*
20110606, hksong
summary:
  -. GPSONE DAEMON 메세지 Block
  -. User Mode 빌드 시에는 필히 적용 필요함
*/
//#define FEATURE_P_VZW_LBS_BLOCK_GPSONE_DAEMON_MSG

/*
 * 20110624, hksong, ##Program 에서 PDE IP/PORT 를 읽을 수 있도록 함
 */
#define FEATURE_P_VZW_LBS_PST_PDE_READ

/*
20120412, AMJ
summary:
 -. AFLT¸¦ MSA·Î ´ëÃ¼ÇÏ¸ç pref_time 0À¸·Î ³»·ÁÁÜ 
*/
#define FEATURE_P_VZW_LBS_AFLT

/*
kim.jeongmin2 PS2-AJANTECH 120419
summary:
 -. UTS dll�� �̿��� LBS Auto Test �� Reset�� �ٷ� Data Call�� �õ��Ͽ� 
 Registration ���ϴ� �����߻��Ͽ� ������ ���ʰ��� Data Call�� ����
*/
#define FEATURE_P_VZW_LBS_UTS_AUTO


/*===========================================================================
    UICC Features
===========================================================================*/
#ifdef FEATURE_P_VZW_CP_MANAGER_DAEMON_INTERFACE
#define FEATURE_P_VZW_UICC_CARD_TYPE
#endif

/*************************************************/
/*                    UICC                       */
/*************************************************/
/*
info : IMS 지원을 위한 ISIM interface 추가함.
when : 20111013
who  : hyko
*/
#define FEATURE_P_VZW_SUPPORT_FOR_ISIM_APPLICATION

#define FEATURE_P_VZW_APPLY_UICC_CARD_DEFINITION
#ifdef FEATURE_P_VZW_APPLY_UICC_CARD_DEFINITION
#define FEATURE_P_VZW_CPMGR_GET_CARD_TYPE_MODI
#define FEATURE_P_VZW_SEND_PROVISION_STATUS_WITH_CARD_TYPE
#endif

/*
   -. From EF10, ds1_cp_card_power_down_not_support
   -. card power down Ä¿ï¿½Çµï¿½ Ã³ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Êµï¿½ï¿½ï¿½ ï¿½ï¿½.
      < ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ß»ï¿½ï¿½Ï´ï¿½ ï¿½Ö¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ >
      1. ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ô½ï¿½ USIM card accessï¿½ï¿½ï¿½ï¿½ ï¿½Ê´ï¿½ ï¿½ï¿½ï¿½ï¿½
      2. ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ô½ï¿½ SIM cardï¿½ï¿½ ï¿½Î½Äµï¿½ï¿½ï¿½ ï¿½Ê´Â´Ù´ï¿½ ï¿½ï¿½ï¿½ï¿½ Ç¥ï¿½Ãµï¿½.
      3. on chip sim modeï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½î°¡ï¿½ï¿½ cardï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Â¿ï¿½ï¿½ï¿½ UIMï¿½ï¿½ï¿½ï¿½ card power downÄ¿ï¿½Çµå°¡ ï¿½ï¿½ï¿½ÞµÇ¾ï¿½
         page fault ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ß»ï¿½ï¿½ï¿½....
when : 20120215
who  : kim.youngchai@pantech.com
*/
#define FEATURE_P_VZW_CP_CARD_POWER_DOWN_NOT_SUPPORT


#define FEATURE_P_VZW_ANDROID_USES_MEID_ME


#define FEATURE_P_VZW_ADD_CARD_REMOVED_STATE


/*===========================================================================
    Wi-Fi Features
===========================================================================*/


/*===========================================================================
    Call UI Features
===========================================================================*/


/*===========================================================================
    UTS Features
===========================================================================*/
/*
20110216, GEE 
summary:
  -. UTS Feature 작업 
*/
#define FEATURE_P_VZW_UTS
#ifdef FEATURE_P_VZW_UTS
/*
20110216, GEE 
summary:
  -. UTS verizon stage 2를 위한 FEATURE (verizon stage2 by C2k)
*/
#define FEATURE_P_VZW_UTS_UDM_C2K
#define FEATURE_P_VZW_UTS_NOTSUPPORT_RELEASE_A
/*
20110321, GEE
summary:
  -. ANDROID MODEL의 W2Bi TEST를위한 FEATURE 
*/
#define FEATURE_P_VZW_UTS_W2Bi_ANDROID
/*
20110324, GEE
summary:
  -. ANDROID MODEL의UTS EXTENDED COMMAND를 위한 FEATURE  
*/
#define FEATURE_P_VZW_UTS_XCMD_ANDROID
#endif

/*
kim.jeongmin2 PS2-AJANTECH 111121
summary:
  -. Power Save Mode에서 LCD On 되면 Wakeup(QMI)
*/
#define FEATURE_P_VZW_WAKEUP_IN_PWR_SAVE

/*
kim.jeongmin2 PS2-AJANTECH 111214
summary:
  -. RIL_REQUEST_CDMA_SUBSCRIPTION 처리 과정 중 PRL Version 가져오는 부분 실패하더라도 다른 정보는 업데이트 하도록 수정
*/
#define FEATURE_P_VZW_CDMA_INFO_SUBSCRIPTION

#endif/* __CUST_PANTECH_PROTOCOL_LINUX_H__ */


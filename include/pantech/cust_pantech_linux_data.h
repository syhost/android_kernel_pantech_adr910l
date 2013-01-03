#ifndef __CUST_PANTECH_LINUX_DATA_H__
#define __CUST_PANTECH_LINUX_DATA_H__

/* ========================================================================
FILE: cust_pantech_linux_data.h

Copyright (c) 2010 by PANTECH Incorporated.  All Rights Reserved.

USE the format "FEATURE_P_VZW_DS_XXXX"
=========================================================================== */ 

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---      ----------------------------------------------------------


===========================================================================*/

/*===========================================================================
Feature 처리 MANUAL

-. ��??�일 변��???반드??file header??history��?기입?�도��??�다.

-. system vob??공통 처리?�는 Feature??System VOB Features section???�다.
   system vob??처리?�는 Feature??FL��??�의?�여 ?�용?�도��??�다.

-. feature?�름?��?FEATURE_P_VZW_DS_[name] ?�로 처리?�다.
   각각??section???�도��??�다.

-. feature?�는 author, date, summary��?comment?�다.
   ��??�일?��?feature specification???�체?��?��?
   summary??feature��?구현???�스��?보�? ?�더?�도 feature???�용???����??�악?????�을 ?�도��??�세?�게 ?�는??

-. JAVA source?�는 comment��?feature 처리???�항???�긴??

-. feature???�언?��??�래 목차??Feature 그룹��??�절??곳에 ?�도��??�다.

    1. Port Bridge & AT COMMAND Feature
    2. DUN & NDIS & RNDIS Feature 
    3. Hidden Code Feature
    4. Framework Feature
    5. Kernel & init.rc & VPN Feature
    6. SDM Feature

===========================================================================*/

/*===========================================================================
    Interface Features
===========================================================================*/


/*===========================================================================
    Data Service Features
===========================================================================*/

//CHEETAH compile error
//FEATURE_P_VZW_DS_CHEETAH_COMPILE

//DS1_REMOVE_WIFI_EVENT_LOG //찾아???�요???�용

//feature name ?�서 PS��?DS ��?변경할��?


/*
//20120119_yunsik_DATA
cust_pantech_linux_data.h ?�일??컴파???�도��?CUST_PANTECH.h??include ??
*/
#define FEATURE_P_VZW_DS_CUST_INCLUDE

/*
//20111227_yunsik_DATA
- system\core\liblog\Logd_write.c  : CDMA, GSM tag - RADIO to MAIN
//20120207_yunsik_DATA
- LINUX\android\bionic\libc\netbsd\resolv\res_cache.c ?�서 DNS��?보기 ?�해 ?�시 ?��?�음. 막을 ��?
*/
#define FEATURE_P_VZW_DS_ANDROID_LOG

/*==========================================================================
    1. Port Bridge & AT COMMAND Feature
===========================================================================*/

/*
//20120229_yunsik_DATA, ICS User Build ?�서 DUN???�는 것으��??�인 ?�어 ?�래 ?�쳐 ??�� ??

//20111107_yunsik_DATA, feature name rename define, comment ?�리
<Cheetah : Apache?�서 ?�팅>
 - Android.mk (frameworks\base) : ISkyDun.aidl 추�?
 - AndroidManifest.xml (packages\apps\phone) : SkyDunService 추�?
 - AndroidManifest.xml (vendor\qcom\proprietary\qualcommsettings) : DunService block
 - Dun_Autoboot.java (vendor\qcom\proprietary\qualcommsettings\src\com\android\qualcommsettings) : DunService block
 - SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone) : ?�성 (SkyDunService start)
 - SkyDunService.java (packages\apps\phone\src\com\android\phone) : ?�성
 - SkyTelephonyInterfaceManager.java (frameworks\base\telephony\java\com\android\internal\telephony\cdma) 

  20110121, swchoi
  summary:
    - EF14L??DS1_DS_DUN_USER_MODE��?APACHE��??�팅
 - 문제??: QualcommSetting ?�렉?�리???�는 ?�일?��? user mode ???�행?��? ?�으므��? dun, sync manager, data manger, curi explore가 ?�행?��? ?�음.
#define FEATURE_P_VZW_DS_DUN_USER_MODE
*/


/*==========================================================================
    2. DUN & NDIS & RNDIS Feature 
===========================================================================*/


  /*
//20111107_yunsik_DATA, feature name rename define
<Cheetah : Apache?�서 ?�팅 and modified>
 - AndroidManifest.xml (packages\apps\phone)
 - ISkyDun.aidl (frameworks\base\telephony\java\com\android\internal\telephony)
 - DataConnectionTracker.java (frameworks\base\telephony\java\com\android\internal\telephony)
 - SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone)
 - SkyDataIntents.java (frameworks\base\telephony\java\com\android\internal\telephony)
 - Strings_cp.xml (packages\apps\phone\res\values)
 - dun_service.png (packages\apps\Phone\res\drawable) : 추�?  

  summary:
  -. DUN ?�속??notification ??DUN ?�용중임???�시
  -. DUN init, end ??Toast Msg 출력

  SkyDunService.java (E-APACHE_MSM_3035\LINUX\android\packages\apps\Phone\src\com\android\phone)
  strings_cp.xml (E-APACHE_MSM_3035\LINUX\android\packages\apps\Phone\res\values)
  dun_service.png (E-APACHE_MSM_3035\LINUX\android\packages\apps\Phone\res\drawable) : 추�?  
  */
#define FEATURE_P_VZW_DS_DUN_UI

/*==========================================================================
    3. Hidden Code Feature
===========================================================================*/

/*
//20111102_yunsik_DATA
Related feature is FEATURE_P_VZW_CP_HIDDEN_CODE
  -> SkyHiddenCode.java

DATA Hidden code
- SkyDataBroadcastReceiver.java create
- SkyDataIntents.java create
*/
#define FEATURE_P_VZW_DS_HIDDEN_CODE

/*
//20120301_yunsik_DATA
- Test enable test code (##*328236*#), only NV access

AndroidManifest.xml (packages\apps\phone)
Cust_pantech_linux_data.h (pantech\include)
SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone)
SkyHiddenCode.java (frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_TEST_ENABLE_TESTCODE

/*
//20120301_yunsik_DATA
- DUN enable/disable code (##*36386*#)
- When P1 submission, MUST default value to false. (SkyDataBroadcastReceiver.java, Dun_service.c)

AndroidManifest.xml (packages\apps\phone)
Cust_pantech_linux_data.h (pantech\include)
Dun_service.c (vendor\qcom\proprietary\data\port-bridge)
SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone)
SkyHiddenCode.java (frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_ENABLE_DUN_LINUX

/*
*  2012.05.22 P11546 Add DMLogging Hidden Menu
*    
*    LINUX/android/package/apps/Contacts/src/com/android/Contacts/SpecialCharSequenceMgr.java
*    LINUX/android/package/apps/DMLogging/AndroidManifest.xml
*    LINUX/android/package/apps/DMLogging/src/com/android/dmlogging/DMLoggingHiddenMenuBroadcastReceiver.java (Added)
*    LINUX/android/package/apps/Phone/src/com/android/Phone/SpecialCharSequenceMgr.java
*    LINUX/android/pantech/apps/Dialer/src/com/android/contacts/PCUSpecialCharSequenceMgr.java
*    LINUX/android/vendor/qcom/proprietary/dm-monitor/etc/Android.mk
*    LINUX/android/vendor/qcom/proprietary/dm-monitor/qdminterface/Android.mk
*
*  2012.06.11 P11546 Remove hidden code for DMLogging due to VZW QC issue
*    - Delete FEATURE_P_VZW_DS_DMLOGGING_HIDDEN_MENU in below files
*
*    LINUX/android/package/apps/Contacts/src/com/android/Contacts/SpecialCharSequenceMgr.java
*    LINUX/android/package/apps/DMLogging/AndroidManifest.xml
*    LINUX/android/package/apps/DMLogging/src/com/android/dmlogging/DMLoggingHiddenMenuBroadcastReceiver.java (Added)
*    LINUX/android/package/apps/Phone/src/com/android/Phone/SpecialCharSequenceMgr.java
*    LINUX/android/pantech/apps/Dialer/src/com/android/contacts/PCUSpecialCharSequenceMgr.java    
*/
#define FEATURE_P_VZW_DS_DMLOGGING_HIDDEN_MENU

/*
//20120613_yunsik_DATA
- DNS block test code (##*367999*#, ##*DNSXXX*#), dormant maintains for test equipment.
- Yunsik wanted to modify to InetAddress.java, but System.getProperty fuction does not work in InetAddress.java

- AndroidManifest.xml (packages\apps\phone)
- Cust_pantech_linux_data.h (pantech\include)
- Getaddrinfo.c (bionic\libc\netbsd\net)
- SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone)
- SkyHiddenCode.java (frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_DNS_BLOCK_TESTCODE

/*==========================================================================
    4. Framework Feature
===========================================================================*/

/*
  20110307, swchoi
  summary:
    - ef30 FEATURE_SKY_DS_ADD_DATA_AIDL 참조.
    - Phone Interface��??�근 못하??APP��??�하??AIDL??추�?    
  - aidl ?�언 : ISkyDataConnection.aidl
    - ?�터?�이??구현 : MMDataConnectionTracker.java
    - 추�? 구현 ?�일 : CDMAPhone.java
    - aidl ?�비??추�? : service_manager??.aidl 추�?
    - make ?�일 ?�정 : android/framework/base/Android.mk ?�정
	
   Android.mk (LINUX\android\frameworks\base)
   ISkyDataConnection.aidl (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony) ?�일추�?
   MMDataConnectionTracker.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony)
   service_manager.c (LINUX\android\frameworks\base\cmds\servicemanager)
   SkyDataConInterfaceManager.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony) ?�일추�?	
*/
#define FEATURE_P_VZW_DS_ADD_DATA_AIDL


/*
  20110307, swchoi
  summary:
    - ef30 DS1_DS_DISCONNECT_ALL_NETWORK_FEATURE 참조.
    - startUsingNetwrokFeature��??�용??모든 APP???��?�여 stopUsingNetworkFeature��??�출???�에
      3g Data Disable ?�도��?추�???

   ConnectivityManager.java (E-APACHE_MSM_3035\LINUX\android\frameworks\base\core\java\android\net)
   ConnectivityService.java (E-APACHE_MSM_3035\LINUX\android\frameworks\base\services\java\com\android\server)
   IConnectivityManager.aidl (E-APACHE_MSM_3035\LINUX\android\frameworks\base\core\java\android\net)
   MMDataConnectionTracker.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_DISCONNECT_ALL_NETWORK_FEATURE

/*
//20111115_yunsik_DATA, for Cheetah modified (from apache)
  20110307, swchoi
  summary:
   - ef30 DS1_DS_PS_DISABLE_FOR_SKT , DS1_DS_EASY_SETTING 참조.
   - 3g Data Enable/Disable ?�수 추�?     
   - Easy Setting??3G Data?��?Mobile network setting?�의 Data enabled간의 sync?�업
   - Easy Setting?�서 3G Data ?�팅 변��???Settings.java??변경값 즉시 반영

 - ConnectivityService.java (frameworks\base\services\java\com\android\server)
 - EasySetting.java (frameworks\base\packages\systemui\src\com\android\systemui\statusbar)
 - ISkyDataConnection.aidl (frameworks\base\telephony\java\com\android\internal\telephony)
 - MMDataConnectionTracker.java (frameworks\base\telephony\java\com\android\internal\telephony)
 - Settings.java (packages\apps\phone\src\com\android\phone)
 - SkyDataConInterfaceManager.java (frameworks\base\telephony\java\com\android\internal\telephony)
*/
 #define FEATURE_P_VZW_DS_PS_DISABLE

/*
//Multiple APN
//cheetah changed verizon -> vzwapp
*/
#define FEATURE_P_VZW_DS_LTE_MULTIPLE_APN

/*
//20120127 P11546
- config.xml (frameworks\base\core\res\res\values)
- ConnectivityManager.java
- ConnectivityService.java
- Phone.java
- MobileDataStateTracker.java
- DataConnectionTracker.java
- GsmDataConnectionTracker.java
- RILConstants.java
- ServiceTypeListActivity.java

//20120116_yunsik_DATA
- 800 APN added
- apns.xml apns-conf.xml
*/
#define FEATURE_P_VZW_DS_LTE_MULTIPLE_APN_800APN


/*
//20120110_yunsik_DATA, LINUX\android\packages\apps\VerizonPermissonsVZWapnPermission 추�?
//20120110_yunsik_DATA, Config.xml (frameworks\base\core\res\res\values) ?�서 config_protectedNetworks ?�정
//201104021_ckhong
- multiole apn requirement sing. & meta data check :: vzw-reqs-117-37
- vzw app apn 관?�내??  connectivity service function ?�서 net type hipri ��??�청?�는 경우 meta-data, sign 경우 vzw app apn ?�로처리 
- ConnectivityService.java (E-APACHE_MSM_3035\LINUX\android\frameworks\base\services\java\com\android\server)
*/
#define FEATURE_P_VZW_DS_PS_VZW_APP_APN_REQ
#define FEATURE_P_VZW_DS_PS_VZW_APP_APN_REQ_NEW_IMPLEMENT //20120510_yunsik_DATA

/*
//201104021_ckhong
- vzw apn ??경우 inactivity timer 1439 min. ?�로 ?�정
- ConnectivityService.java (\LINUX\android\frameworks\base\services\java\com\android\server)
//20120502_yunsik_DATA,//20120525_yunsik_DATA, HIPRI 30mins blocked because of removing of ECID app
- HIPRI added getRestoreHipriNetworkDelay(). 30minutes. 
*/
#define FEATURE_P_VZW_DS_PS_INACTIVITY_TIMER

/*
//20120116_yunsik_DATA
- eHRPD Internet PDN IPv6 ignore.
*/
#define FEATURE_P_VZW_DS_IGNORE_eHRPD_INTERNET_IPV6

/*
//20120131_yunsik_DATA
- eHRPD Internet PDN IPv6 enable/disable setting
- 관??feature : FEATURE_P_VZW_DS_SDM

<Req>------------------
5.3.4.1.4 IPV6 on eHRPD
The device shall support the enabling and disabling of IPV6Enable Mode. See "Reqs-Data Requirements" for full set of functional requirements and behavior of the IPV6 enabled node.  
If a Get command is sent to the ./ManagedObjects/ConnMO/IPV6Enable/Settings/Operations node, the device shall return the value of "enabled" 
if IPV6 Traffic on the eHPRD is enabled, and a value of "Disabled" if the IPV6 Traffic is currently disabled.  

5.3.4.1.4.2 Disable IPV6 Traffic on eHPRD
When an Exec command is sent to the device on this node, the device shall not allow any IPv6 data from either an application on the device or an application on a tethered laptop to pass 
through to the Internet PDN. The default value for IPV6 Enable shall be "disabled" 

5.3.4.1.4.3 Enable IPV6 Traffic on eHPRD
When an Exec command is sent to the device on this node, the device shall support normal operation and allow any IPv6 data from either an application on the device 
or an application on a tethered laptop to pass through to the Internet PDN.  
-----------------------

secure.setting?��?모두 write?�고 eHRPD Modem???�용???�항
android/LTE   eHRPD       enable    disable  
    V4V6          V4(V4V6)     V4V6        V4        => android ??무조��?V4V6��??�팅?�서 disconnect 처리
      V4                V4          nothing   nothing   => return true
      V6                V6          nothing   nothing   => android ??무조��?V6��??�팅?�서 disconnect 처리

Cust_pantech_linux_data.h (linux\android\pantech\include)
DataConnectionTracker.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
GsmDataConnection.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony\gsm)
GsmDataConnectionTracker.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony\gsm)
IdevConnMo.java (linux\android\pantech\apps\otadmextensions\src\com\innopath\activecare\dev\oem)
ISkyDataConnection.aidl (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
SkyDataBroadcastReceiver.java (linux\android\packages\apps\phone\src\com\android\phone)
SkyDataConInterfaceManager.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)

2012.03.27 SJM
Modify eHRPD IPv6
DataConnection.java
GsmDataConnection.java
GsmDataConnectionTracker.java
ApnContext.java

*/
#define FEATURE_P_VZW_DS_eHRPD_INTERNET_IPV6

/*
//20120604_yunsik_DATA
- if eHRPD Internet PDN IPv6 default value is changed?  (false -> true)
- Please this feature DEFINE !
- SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone)
- DataConnectionTracker.java (frameworks\base\telephony\java\com\android\internal\telephony)
- 1. android property 2.modem nv 3. eHRPD profile
*/
#define FEATURE_P_VZW_DS_eHRPD_INTERNET_IPV6_CHANGE_DEFAULT_IS_TRUE_ANDROID

/* P12239_DATA 
-SVDO enable setting
- 검?��? ?�걸��?.. FEATURE_P_VZW_SVDO_ENABLE .. 바�?��?
*/
#define FEATURE_P_VZW_DS_SVDO_ENABLE

/*
//2012.01.09 P11546 ICS porting - ICS 버전??변경된 구조??Apache 코드 ?�용?? 
- DataConnectionTracker.java, GsmDataConnectionTracker.java

//20110526_ckhong_DATA
- IMS REG/DE-REG  ?�태 체크 , power off/airplane mode ��?de-reg ??wait.
- DataConnectionTracker.java, MMDataConnectionTracker.java  (\android\frameworks\base\telephony\java\com\android\internal\telephony)
*/

#define FEATURE_P_VZW_DS_IMS_REG_DEREG

/*
//2012.01.09 P11546 ICS porting - ICS 버전??변경된 구조??Apache 코드 ?�용?? 
- GsmDataConnectionTracker.java
- MobileDataStateTracker.java

//20110527_ckhong_DATA
- Data Mode/Data Roaming Mode ???��??�이 LTE/EHRPD ?�서 ?�결.
- MMDataConnectionTracker.java  (\android\frameworks\base\telephony\java\com\android\internal\telephony)
- ConnectivityService.java (\android\frameworks\base\services\java\com\android\server)
- MobileDataStateTracker.java (LINUX\android\frameworks\base\core\java\android\net)
*/
#define FEATURE_P_VZW_DS_IMS_ALWAYS_ON

/*
//2012.03.13 swchoi
- For hide data icon when ims apn only activated.
- NetworkController.java (android\frameworks\base\packages\SystemUI\src\com\android\systemui\statusbar\policy)
*/
#define FEATURE_P_VZW_DS_IMS_DATA_ICON_VISIBLE

/*
//2012.02.04 swchoi
- 모�??�서 AT command��??�해 IMS TEST MODE ?�정???�용?�는 NV_IMS_CLIENT_ENABLED_I ???�어?��?Ecrio IMS?�서 ?�용
- NV_EHRPD_ENABLED_I ??NV값으��??�합.
 NVInterface.java (android\frameworks\base\telephony\java\com\android\internal\telephony)
 */
#define FEATURE_P_VZW_DS_AT_CMD_LTE_IMS_TEST_MODE

/*
//20120206_yunsik_DATA
- APN2��?disable?�을 경우 CdmaDataConnectionTracker.java ?�서 exception 발생
- workaround code?? 추후 patch ?�황??볼것.
*/
#define FEATURE_P_VZW_DS_CDMA_TRACKER_EXCEPTION

/*
//20120207_yunsik_DATA
- getaddrinfo.c (linux\android\bionic\libc\netbsd\net)
- _have_ipv6(), _have_ipv4() ?�서??default 기�??�로 ?�어 ?�어 만약 vzwinternet??IPv6 only ?�면 ?�른 PDN?�서 V4��?DNS ?�수 ?�는 ?�상??발생??
- _test_connect() ?�서 return 1; ��??�정
//FEATURE_P_VZW_DS_DISABLE_TEST_CONNECT_FOR_DNS_DEBUG
*/
#define FEATURE_P_VZW_DS_DISABLE_TEST_CONNECT_FOR_DNS

/*
//2012.02.02 p11546 
   /proc/net/xt_qtaguid/stats ?�일??interface ��?uid ��?data traffic ?�보 기록?��?��??��? ?�어?�임

- NetworkStats.java (LINUX\android\frameworks\base\core\java\android\net) : ?�정 
- TrafficStats.java (LINUX\android\pantech\frameworks\vzw\java\com\verizon\net) : 추�? (com.verizon.net ?�키지��?구현??것을 VZW?�서 ?�구?? 
- Android.mk (LINUX\android\frameworks\base) : ?�정
- system/core/rootdir/init_STARQ.rc
- pantech/frameworks/vzw/Android.mk 
- pantech/frameworks/vzw/com.verizon.net.xml 
- dex_preopt.mk
- NetworkManagementService.java
- INetworkManagementService.aidl
*/
#define FEATURE_P_VZW_DS_TRAFFIC_MEASUREMENT

/*
//20120217_yunsik_DATA, modified
- APN ?�정?�서 default ��?보이��??�머지 vzw apn 리스??보이지 ?�도��?처리 
- "##276#7388464#" ?�로 apn ?�정 진입?�야 editing 가??

- AndroidManifest.xml (packages\apps\phone)
- ApnEditor.java (packages\apps\settings\src\com\android\settings)
- ApnPreference.java (packages\apps\settings\src\com\android\settings)
- ApnSettings.java (packages\apps\settings\src\com\android\settings)
- Cust_pantech_linux_data.h (pantech\include)
- SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone)
- SkyHiddenCode.java (frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_PREVENT_EDIT_DEFAULT_APN

/*
//20120222_yunsik_DATA
- test code (toast) of FEATURE_P_VZW_DS_MMS_ERROR_REASON modem implementation, Only for Test, blocked in CdmaDataConnectionTracker.java.
- CdmaDataConnectionTracker.java (frameworks\base\telephony\java\com\android\internal\telephony\cdma)
*/
#define FEATURE_P_VZW_DS_MMS_ERROR_REASON_TEST_TOAST

/* 2012.02.27 P11546 PS3?��??�정?�항 ?�용 (Data Usage ?�동??문제 ?�정)
- datausage 메뉴?�서 background data 차단 ??for loop ��??�며 UID 별로 ip table ??set ??주어 
   app ?�서 ?�간 ??받�? 못해 anr 발생 ��?background data 차단 ?�정 ??booting ��?system ?�서 
   anr 발생?�는 문제 ?�정   =>��?UID 별로 event ��?처리?�도��???
- android_filesystem_config.h  ??Define ??UID ��?system UID ��?변��? 
- system/bin/iptables ??system 권한?�로 ?�어 ?�으??ip6tables ??shell ��??�어 ?�어 system ?�로 변��?
   -NetworkPolicyManagerService.java
*/
#define FEATURE_P_VZW_DS_BACKGROUND_RESTRICT_BUG_FIX

/* 2012.02.27 P11546 PS3?��??�정?�항 ?�용 (Data Usage ?�동??문제 ?�정)
- system/bin/iptables ??system 권한?�로 ?�어 ?�으??ip6tables ??shell ��??�어 ?�어 system ?�로 변��?
   -android_filesystem_config.h
*/

#define FEATURE_P_VZW_DS_IP6TABLE_UID_BUG_FIX

/*
//20120229_yunsik_DATA
- Qualcomm patch 1031??추�???partial protocol failure???��???�외 처리
- APN setting??Dual IP ??경우 ??type��??�결?�면 partial protocol failure ��??�해 reconnect 루틴???�게??
- IMS??Dual IP ?��?��?IPv6��??�결?�고, eHRPD internet ??Dual IP ?��?��?IPv4��??�결?�키��??�문?????��?지 경우???��??처리 ?�요
- 관??feature : FEATURE_P_VZW_DS_IGNORE_eHRPD_INTERNET_IPV6, FEATURE_P_VZW_DS_eHRPD_INTERNET_IPV6
- dataconnection.java ( android\frameworks\base\telephony\java\com\android\internal\telephony\ )
*/
#define FEATURE_P_VZW_DS_SKIP_PARTIAL_FAILURE
//20120403 sjm removed this feature
//#define FEATURE_P_VZW_DS_SKIP_PARTIAL_FAILURE_PREVIOUS_LTE_CHECK

/* 2012.03.01 P11546
- LINUX\android\pantech\frameworks\vzw\net\ConnectivityManager.aidl
- LINUX\android\pantech\frameworks\vzw\server\VzwConnectivityService.java
- LINUX\android\frameworks\base\Android.mk
- LINUX\android\frameworks\base\services\java\Android.mk
- SystemServer.java (LINUX\android\frameworks\base\services\java\com\android\server)
- ProcFileReader.java
*/
#define FEATURE_P_VZW_DS_DATA_METER_API

/*
//20120314_yunsik_DATA, from apache
- "com.android.phone.action.SHOW_OTA_ACTIVATION" intent 받아??OTA start/end ???��??처리
- OTA start ??admin ?�외??apn ?��?disable ?�키��?not available ��?만들??data ?�도?��? 못하?�록 ?? 
- MobileDataStateTracker.java (LINUX\android\frameworks\base\core\java\android\net)
*/
#define FEATURE_P_VZW_DS_PS_OTA_REQ

/*
//20120314_yunsik_DATA
- isConcurrentVoiceAndDataAllowed() ?�수?�에 ?�래?��?같�? 부분이 ?�는?? mLteSS.getRadioTechnology() 가 ??�� 0?�로 return ?�고 ?�음
  if (mLteSS.getRadioTechnology() != ServiceState.RADIO_TECHNOLOGY_1xRTT)
- workaround��?mLteSS.getRadioTechnology() ��?networkType ?�로 변��?
*/
#define FEATURE_P_VZW_DS_GET_RADIO_TECHNOLOGY_BUG_WORKAROUND
/*
SUSPENDED ACTION
*/
#define FEATURE_P_VZW_DS_GET_RADIO_TECHNOLOGY_BUG_WORKAROUND_ADD

/*
//20120316_yunsik_DATA
- ACTIVE or DORMANT check API for PPP session/PDN management 
*/
#define FEATURE_P_VZW_DS_ACTIVE_DORMANT_CHECK_API

/*
* 2012.04.10 P11546 
*   In case IMS registration started before GsmDataConnectionTracker is not ready (e.g. mAllApnList is not created yet) so IMS apn request failed, GsmDataConnectionTracker should retry IMS apn request. 
*   GsmDataConnectionTracker.java
*/
#define FEATURE_P_VZW_DS_IMS_APN_REQ_RETRY

/*
* 2012.04.10 P11546 
*   If IMS APN request failed in eHRPD network, it should be done again after 15 min (VZW Requirement)
*   GsmDataConnectionTracker.java
*/
#define FEATURE_P_VZW_DS_IMS_RECONNECT_IN_EHRPD


/*
*  2012.04.11 p11546
*    Qcom SBA_1045_CR350813_Pantech_case821603_04102012
*    - Fix traversal of traffic control's flow_list while trying to find the tail of the doubly linked list.
*    netmgr_tc.c (android\vendor\qcom\proprietary\data\netmgr\src)
*/
#define FEATURE_P_VZW_DS_SBA_1045_CR350813
/*
 - P12239
 - FOR MMS IOT 1XRTT Buffersize setting
 */

#define FEATURE_P_VZW_DS_FOR_MMS_IOT_1XRTT  
/*
 - P12239 2012.04.14 Add CR347576
 - FIX Maintaining RSSI
 */
 #define FEATURE_P_VZW_DS_CR347576

/*
* 2012.04.14 p11546
*  - Change TCP buffer size according to the network type in case the network type is changed
*  - MobileDataStateTracker.java
*/
#define FEATURE_P_VZW_DS_SET_TCPBUF_IN_RAT_CHANGE

/*
* 2012.04.16 p11546
*  - Qcom 1045 source bug fix : There's a corner case qdi_cb_hdlr() may return without QDI_MUTEX_UNLOCK
*  - qdi.c (LINUX\android\vendor\qcom\proprietary\data\qdi\src)
*/
#define FEATURE_P_VZW_DS_QCOM_1045_MUTEX_UNLOCK_BUG_FIX

  /*
  20110511, swchoi
  summary:
    -. REQS-ANDROID 2.25 DISABLE SIP STACK IN 3G DEVICES 

    " android.net.sip.SipManager.isApiSupported() must return false by removing android.software.sip feature
    " android.net.sip.SipManager.isVoipSupported() must return false by removing android.software.sip.voip
    " The native SIP stack does not have to be removed
      
    common.mk (LINUX\android\device\qcom\common)  : ?�래?�줄 ??��.
    frameworks/base/data/etc/android.software.sip.xml:system/etc/permissions/android.software.sip.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
  */

#define FEATURE_P_VZW_DS_DISABLE_SIP_STACK

/*
* 2012.04.21 p11546
*  - Bug fix for data icon and wifi icon are coexist in case USB tethering -> Data off -> Data on -> Wifi on 
*  - Tethering.java
*/
//20120503_yunsik_DATA, block because connect/disconnect ping pong issue occurred during 1X/HRPD in case of Data off->Data on
//#define FEATURE_P_VZW_DS_TETHERD_ICON_BUG_FIX

/*
* 2012.04.21 p11546
*  - To show 4G Icon using VZW Test SIM
*     This feature depends on FEATURE_P_VZW_DIABLE_SIM_LOCK (IccCard.State.READY_UNKNOWN)
*  - NetworkController.java
*/
#define FEATURE_P_VZW_DS_TEST_SIM_DATA_ICON

/*
//20120424_yunsik_DATA
During GPS test mode, Do not disconnect even though NO Rx PKT
- DataConnectionTracker.java
- CdmaDataConnectionTacker.java
- GsmDataConnectionTracker.java
*/
#define FEATURE_P_VZW_DS_DO_NOT_RESTART_RADIO_WHEN_NO_RX_PKT_FOR_TESTING

/*
//20120424_yunsik_DATA, SBA porting
searching point 
    //SBA CRs-Fixed: 353696
    //SBA CRs-Fixed: 353897
- DataConnection.java (frameworks\base\telephony\java\com\android\internal\telephony)   : CR 353696
- Ril.h (hardware\ril\include\telephony)   : CR 353696
    ==> Ril.h (hardware\ril\mock-ril\src\cpp)
    ==> Ril.h (hardware\ril\reference-ril)
- Qcril_data_netctrl.c (vendor\qcom\proprietary\qcril\common\data)  : CR 353897
*/
#define FEATURE_P_VZW_DS_QUALCOMM_SBA_CR353696_CR353897

/*
* 2012.04.28 p11546
*  1) Dsi_netctrl: Added param to indicate a partial retry (CRs-Fixed: 355724)
*     - Added a param to dsi_netctrl for clients to indicate a Dual-IP partial  retry attempt. 
*        In QDI, fail the call attempt if a partial retry is being attempted  but the corresponding call object had already been freed
*     - dsi_netctrl/inc/dsi_netctrl.h     
*     - dsi_netctrl/src/dsi_netctrl.c     
*     - dsi_netctrl/src/dsi_netctrl_mni.c 
*     - dsi_netctrl/src/dsi_netctrli.h
*     - qdi/inc/qdi.h
*     - qdi/src/qdi.c 
*
*  2) QCRIL Data: Updates to handle Telephony, QCRIL Data race conditions (CRs-Fixed: 353897, 355724, 353696)
*     - Use PDP_FAIL_PARTIAL_RETRY_FAIL error code when reporting partial retry failure instead of the generic failure code
*     - Block subsequent call attempts with the same RIL parameters if a pending call attempt (in CALL_INACTIVE state) is found
*     - Use the DSI_CALL_INFO_PARTIAL_RETRY parameter to indicate a Dual-IP  partial retry attempt to dsi_netctrl library. 
*        Also, clean-up stale  QMI WDS handle if we receive an unexpected DSI_EVT_NET_IS_CONN event.
*     - common/data/qcril_data_netctrl.c
* 
*  3) Telephony: Add code to recover from special out of sync state (CRs-Fixed: 353696)
*     - If somehow it happened that modem things there is partially succesfulcall, but apps think this is a new call request, 
*       consider partialretry failure as success to get back in sync with modem
*     - /android/internal/telephony/DataConnection.java
*     - this is aleady applied with FEATURE_P_VZW_DS_QUALCOMM_SBA_CR353696_CR353897 
* 
*  4) Telephony: Add partial retry failure code (CRs-Fixed: 353696)
*     - this is aleady applied with FEATURE_P_VZW_DS_QUALCOMM_SBA_CR353696_CR353897 
* 
*  5) Telephony: support dual ip retry error code handling (CRs-Fixed: 353696,353897)
*     - /android/internal/telephony/DataCallState.java 
*
*  6) Telephony: Reset retry count in inactive state (CRs-Fixed: 355482)
*     - /android/internal/telephony/DataConnection.java
*  
*  7) Telephony: Fix setup_data in a loop (CRs-Fixed: 351626)
*     - Pass partial retry flag as an argument to bringUp function callinstead of passing it inside apn (apn can be touched from differentthreads)
*     - When sending EVENT_TRY_SETUP_DATA in onRatChanged method due to having partial failure - set the flag accordingly
*     -  .../android/internal/telephony/DataConnection.java 
*     - .../internal/telephony/DataConnectionTracker.java
*     - .../android/internal/telephony/DataProfile.java
*     - .../java/com/android/internal/telephony/Phone.java
*     - .../telephony/cdma/CdmaDataConnectionTracker.java
*     - .../cdma/MSimCdmaDataConnectionTracker.java
*     - .../telephony/gsm/GsmDataConnectionTracker.java
*     - .../gsm/MSimGsmDataConnectionTracker.java
* 
*/
#define FEATURE_P_VZW_QCOM_SBA_CN836489_042712

/*
//20120503_yunsik_DATA
- Out Of Service is considered as data call disconnect => false setting
- PhoneBase.java (frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_DATA_CALL_NOT_DISCONNECT_WHEN_OOS

/*
* 2012.05.09 p11546
*  In case of single PDN network e.g. 1x or EvDO, mobile upstream is not disconnected even though wifi is connected. 
*  (Problem Reproduce Step : Data on -> USB tethering on -> wifi on -> wifi off -> wifi on) 
*  To prevent this symptom, StarQ doesn't use ConnectivityManager.TYPE_MOBILE_DUN as a tethering upstream and dun APN is not available in CDMA. 
* 
*   - Tethering.java (android\frameworks\base\services\java\com\android\server\connectivity)
*   - CdmaDataProfileTracker.java (android\frameworks\base\telephony\java\com\android\internal\telephony\cdma)
*/
#define FEATURE_P_VZW_DS_TETHERING_1X_EVDO_BUG_FIX

/*
//20120515_yunsik_DATA
- do not show 3G/4G icon (HIPRI, dormant) during wifi connection <- hipri restore timer is 30minutes (ECID porting guide)
- NetworkController.java
- DataConnectionTracker.java (onEnableApn)
- CdmaDataConnectionTracker.java
- GsmDataConnectionTracker.java

//20120525_yunsik_DATA
- this feature removed. becasue ECID app was removed
-  ADR910L - Pan_Int_46045, 
    Description - 
    Application Preload changes:
    1) Add VZ Tones
    2) Remove Caller Name ID

    Here are few important instructions for removing Caller Name ID:

    1) Please make sure APN setting is set properly:
        a. Do not set android.telephony.apn-restore to 30 minutes as instructed by CaNID application integration guide
        b. Following requirement need to be meet:

    [VZW Reqs-Android.docx] INTERNET PDN - 1 min.
*/
//#define FEATURE_P_VZW_DS_DO_NOT_SHOW_HIPRI_ICON_DURING_WIFI

/*
//20120430_yunsik_DATA
//2.9.2 Support to PPP Session/PDN Attachment Management Req was implemented 
//BUT during Data restriction, FOTA could not be connect to vzwadmin PDN.
//So, We should consider that On LTE/eHRPD networks this requirement does not apply to VZWADMIN PDN.
//and we should consider on LTE/eHRPD/HRPD/1X.

-CdmaDataConnectionTracker.java (frameworks\base\telephony\java\com\android\internal\telephony\cdma)
-ConnectivityService.java (frameworks\base\services\java\com\android\server)
-DataConnectionTracker.java (frameworks\base\telephony\java\com\android\internal\telephony)
-GsmDataConnectionTracker.java (frameworks\base\telephony\java\com\android\internal\telephony\gsm)
-MobileDataStateTracker.java (frameworks\base\core\java\android\net)
*/
#define FEATURE_P_VZW_DS_VZWADMIN_NOT_BLOCK_EVEN_THOUGH_DATA_DISABLED

/*
//20120516_yunsik_DATA
- android\vendor\qcom\proprietary\telephony-apps\MultiplePdnTest -> only added vzw800 apn

- FEATURE is not labeling.
- Pantech Multiple PDN Test application with vzw permission
- android\vendor\qcom\proprietary\telephony-apps\PantechMultiplePdnTest
*/
#define FEATURE_P_VZW_DS_PANTECH_MPDP_APK

/*
//20120522_yunsik_DATA
- SBA_M8960AAAAANLYA1046_Pantech_00853164_05172012 applied (only qdi_init, qdi_release)
- dsi_netctrl_mni.c
- qdi.h
- qdi.c
//20120612_yunsik_DATA, remove
*/
//#define FEATURE_P_VZW_DS_M8960AAAAANLYA1046_PANTECH_00853164_05172012

/*
*  2012.06.11 P11546 VZW Request regarding Data Usage (Reqs-Smartphone_UI.docx)
*    warningBytes of default NetworkPolicy should be initialized as WARING_DISABLE to satisfy below VZW req.
*
*    <DEVICE REQUIREMENTS SMARTPHONE UI REQUIREMENTS VERSION 1.12>
*       "6.6 Data Usage 
*    	    Under Data usage settings screen, two options shall be provided to the user
*	    "Limit Mobile data usage"  - OFF by default. When is ON, set it at 5GB
*	    "Alert me about data usage" - OFF by default. Set at 2GB "
*   
*   - LINUX\android\frameworks\base\services\java\com\android\server\net\NetworkPolicyManagerService.java
*   - LINUX\android\frameworks\base\core\res\res\values\strings.xml
*   - LINUX\android\frameworks\base\core\res\res\values-es-rUS\strings.xml
*   - LINUX\android\frameworks\base\core\res\res\drawable-hdpi\data_usage_warning.png
*   - LINUX\android\frameworks\base\core\res\res\drawable-hdpi\data_usage_disabled.png
*/
#define FEATURE_P_VZW_DS_DATA_USAGE

/*==========================================================================
    5. Kernel & init.rc & VPN Feature
===========================================================================*/

/* Kernel Config */
/*
Kernel Config ?�인 방법 //20110621_yunsik_DATA

Apache??경우 LINUX\android\device\qcom\msm7630_fusion\AndroidBoard.mk ?�서
Cheetah??경우 LINUX\android\device\qcom\msm8960\AndroidBoard.mk ?�서
KERNEL_DEFCONFIG ��??�인 ?�면 ?�래?��?같이 kernel config ?�치��??�인?????�다 

= APACHE =======================================
ifeq ($(KERNEL_DEFCONFIG),)
    KERNEL_DEFCONFIG := msm7630-perf_defconfig
endif
==============================================
     APACHE?�서?? LINUX\android\kernel\arch\arm\configs\msm7630-perf_defconfig ??kernel config???�치 ???�다. 

= CHEETAH ======================================
ifeq ($(KERNEL_DEFCONFIG),)
    KERNEL_DEFCONFIG := msm8960_defconfig
endif
==============================================
     CHEETAH?�서 LINUX\android\kernel\arch\arm\configs\msm8960_defconfig ??kernel config???�치 ???�으??
                          ?�제로는 msm8960_cheetah_ws10_defconfig ?��?msm8960_cheetah_ws12_defconfig ��?고쳐???�다. board version ?�라가��??�인 ?�요.

- J-MASAI ?�서???�래?��?같이 ?�정?�었?�며, Apache???�간 ?�리??

CONFIG_NET_KEY=y, CONFIG_INET_ESP=y, CONFIG_INET_IPCOMP=y, 
CONFIG_XFRM=y, CONFIG_XFRM_USER=y, CONFIG_INET_XFRM_MODE_TRANSPORT=y, CONFIG_INET_XFRM_MODE_TUNNEL=y, 
CONFIG_CRYPTO=y, CONFIG_CRYPTO_HMAC=y, CONFIG_CRYPTO_XCBC=y, CONFIG_CRYPTO_NULL=y, CONFIG_CRYPTO_MD5=y,
CONFIG_CRYPTO_SHA1=y, CONFIG_CRYPTO_DES=y, CONFIG_CRYPTO_AES=y, CONFIG_CRYPTO_DEFLATE=y, CONFIG_PPP=y, 
CONFIG_PPP_ASYNC=y, CONFIG_PPP_SYNC_TTY=y, CONFIG_PPP_DEFLATE=y, CONFIG_PPP_BSDCOMP=y, CONFIG_PPP_MPPE=y,
CONFIG_PPPOE=y, CONFIG_PPPOL2TP=y
CONFIG_L2TP=y, CONFIG_L2TP_V3=y, CONFIG_L2TP_IP=y, CONFIG_L2TP_ETH=y, CONFIG_PPPOLAC=y, CONFIG_PPPOPNS=y
*/
#define FEATURE_P_VZW_DS_VPN_FIX

//VpnProfileEditor.java
#define FEATURE_P_VZW_DS_VPN_SPACE_CHECK

/*  20110718, swchoi : IPSec ???�해 ?�래??config 추�?
     CONFIG_INET_AH=y, CONFIG_INET_XFRM_TUNNEL=y
*/
#define FEATURE_P_VZW_DS_IPSEC_CONFIG

/*
//20111101_yunsik_DATA, TUN Driver enable
*/
#define FEATURE_P_VZW_DS_TUN_DRIVER_ENABLE

/*
Info : 
            Since default DHCP Lease Time is 1 hour, VPN, RMNET and android Tethering are disconnected frequently.
            So, Increase the DHCP Lease Time to 7 days by VZW request.
Related Feature :
            
Where : 
            Cust_pantech_modem_data.h (modem_proc\build\cust)
            Dhcp_am_ps_iface.c (modem_proc\modem\datamodem\protocols\inet\src)

            Cust_pantech_linux_data.h (linux\android\pantech\include)
            Showlease.c (linux\android\external\dhcpcd)
            TetherController.cpp (linux\android\system\netd)
            
When : 20110531
Who : Ju.Hojin
*/
#define FEATURE_P_VZW_DS_INCREASE_DHCP_LEASETIME

/*
//20110727_yunsik_DATA

1. Apache : ipv6?�서 MTU가 2000?�로 ?�어?�어 ?��? ?�정??
                  init.rc : write /sys/class/net/rmnet_sdio0~7/mtu 1428
2. Cheetah : init.rc : write /sys/class/net/rmnet0~7/mtu 1428 
              
*/
#define FEATURE_P_VZW_DS_1428_MTU_SETTING


/*
//20110907_yunsik_DATA
1. Apache
   - rmem_max?��?wmem_max ��?기존 262144 ?�서 1730560 ?�로 ?�림
   - 1730560 ?��?LTE window max size?�고, ?�걸 ?�림?�로 ?�해 streaming service(YouTube)?�서 ?�기???�상??보완?????�을 것으��??�각??
2. Cheetah
   - http://www.ipbalance.com/traffic-analysis/%0bthroughput/104-tcp-throughput-calculation-formula.html

   - 80-N7573-1_A_Android_Embedded_Tput_Tuning.pdf ???�라 TCP buffer sizes ?�정
      setprop net.tcp.buffersize.lte 4096,221184,3461120,4096,221184,3461120

   - Also assign TCP buffer thresholds to be the ceiling value of technology maximums   
     write /proc/sys/net/core/rmem_max 3461120 
     write /proc/sys/net/core/wmem_max 3461120
    
*/
#define FEATURE_P_VZW_DS_TCP_BUFFER_SIZES_INCREASE


/*
   2012.02.29 P11546 CTS Fail ??�� ?�정 
   TrafficStats ??UID ��??�이???�집???�한 method가 ?�작?�기 ?�해?�는 CONFIG_UID_STAT=y ��??�정?�어????
- LINUX\android\kernel\arch\arm\configs\msm8960_starq_ws20_defconfig : ?�정 
*/
#define FEATURE_P_VZW_DS_UID_TRAFFIC_STATS_ENABLE

/*
//20120308_yunsik_DATA
busybox install for root, version : 1.19.0
- init_STARQ.rc (android\system\core\rootdir)
- busybox folder all (android\pantech\development\busybox)
*/
#define FEATURE_P_VZW_DS_BUSYBOX_INSTALL

/*
//20120308_yunsik_DATA
iperf install for root, version : 2.0.5 (08 Jul 2010)
- init_STARQ.rc (android\system\core\rootdir)
- iperf folder all (android\pantech\development\iperf)
*/
#define FEATURE_P_VZW_DS_IPERF_INSTALL

/*
 - 20120422 PS4 BTKim : FTP Disconnect
 - FTP disconnection issue. A 
 - init.qcom.rc  (write /proc/sys/net/netfilter/nf_conntrack_tcp_be_liberal 1)
                         ==> /proc/sys/net/ipv4/netfilter/ip_conntrack_tcp_be_liberal copied
*/
#define FEATURE_P_VZW_DS_TCP_BE_LIBERAL_ENABLE

/*
*  2012.06.07 P11546 Android Security Patch
*   [CVE-2012-2811] IPv6 remote denial of service
*       - A remote attacker capable of spoofing large portions of the IPv6 address space can fill the Linux kernel's neighbors table and eventually cause the device to reboot.
*       - LINUX\android\kernel\net\ipv6\route.c  
*
*   [CVE-2012-2808] Incorrect randomization during DNS resolution
*       - During DNS resolution, a random port and a random TXID are generated. 
*          In Android 4.0.4 and prior these values are initialized with insufficient entropy, potentially allowing an attacker to spoof results for DNS queries.
*       - LINUX\android\bionic\libc\netbsd\resolv\res_init.c
*/
#define FEATURE_P_DS_GOOGLE_PATCH_20120607

/*==========================================================================
    6. SDM Feature
===========================================================================*/
/*
Info : 
            VZW_Reqs-LTE_3GPP_Band13_NetworkAcess
            VZW_Reqs-LTE_OTADM
            VZW_Reqs-LTE_Data_Retry
            VZW_Reqs-Data_Requirement(eHRPD)

            1. LTE > ?�나??PDN Connected ?�태?�서 ?�나??PDN Disconnect ?? Detach Request
            2. LTE > ???�상 PDN Connected ?�태?�서 ?�나??PDN Disconnect ?? ?�당 PDN Disconnect Request & LTE Attached ?��?
                - ??경우, ?�당 PDN??IMS PDN??경우, IMS PDN Disconnect ?�후, 곧바��?IMS PDN Re-Connectivity Request
            3. LTE & eHRPD > PDN Enabled/Disabled ?�외???�반?�인 parameter 변��??? ?�당 PDN Disconnect Request ?�후, PDN Re-Connectivity Request

            4. LTE > IMS PDN ?�는 Admin PDN??Enabled/Disabled Setting??Disabled��?바뀌면, Detach & NOT Re-Attach Request / 3G Fall Back(?)
            5. LTE > 기�? PDN??Enabled/Disabled Setting??Disabled��?바뀌면, ?�당 PDN Disconnect Request
            6. eHRPD > IMS PDN ?�는 Admin PDN??Enabled/Disabled Setting??Disabled��?바뀌면, All PDN Disconnect(Connection Close + Session ?��?) & NOT Re-Connection Request
            7. eHRPD > 기�? PDN??Enabled/Disabled Setting??Disabled��?바뀌면, ?�당 PDN Disconnect

Related Feature :
            FEATURE_P_VZW_DATA_EHRPD_SDM
            FEATURE_P_VZW_DATA_LTE_SDM
Where : 
            cust_pantech_protocol_linux.h (android\pantech\include)
            IdevConnMo.java (android\pantech\apps\OtadmExtensions\src\com\innopath\activecare\dev\oem)
            ISkyDataConnection.aidl (android\frameworks\base\telephony\java\com\android\internal\telephony)
            SkyDataConInterfaceManager.java (android\frameworks\base\telephony\java\com\android\internal\telephony)
            
When : 20111026
Who : Ju.Hojin

//20111104_yunsik_DATA
==========================================
From: Youtz, Andrew [mailto:Andrew.Youtz@VerizonWireless.com] 
Sent: Tuesday, November 01, 2011 2:28 AM
To: 'Giseop Won'; Chen, Jennifer; 'EuiSeok Han'; 'Jin Yang'
Cc: 'Kostas Kastamonitis'; '최영??Young H. Choi'; ''?�진??'; Guarino, Bernard
Subject: RE: [UML290] Question regarding OTADM SDM

The device should attach to eHRPD if the eHRPD class 1 APN is disabled. The device should not attach to eHRPD if the eHRPD class 2 APN is disabled.

The device should not attach to LTE if the LTE class 1 APN is disabled. The device should not attach to LTE if the LTE class 2 APN is disabled.

Thanks,
Andy
==========================================

ICS 구현?�항 ?�리 //20120203_yunsik_DATA

IMS Deregi???�용?��? ?�음

<LTE>
VZWIMS 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE DISABLE : IMS de-regi (4sec), cleanUpAllConnections (6sec), Modem Detach(8sec)
VZWADMIN : 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE DISABLE : IMS de-regi (4sec), cleanUpAllConnections (6sec), Modem Detach(8sec)
VZWINTERNET : 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE ENABLE : enableApnType (3sec)
 - APN TYPE DISABLE : disableApnType (3sec)
VZWAPP: 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE ENABLE : nothing (3sec)
 - APN TYPE DISABLE : disableApnType (3sec)
VZW800: 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE ENABLE : nothing (3sec)
 - APN TYPE DISABLE : disableApnType (3sec)

<eHRPD>
VZWIMS 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE DISABLE : IMS de-regi (1sec), disableApnType (3sec)
VZWADMIN : 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE DISABLE : IMS de-regi (4sec), cleanUpAllConnections (6sec), Modem Detach(8sec)
VZWINTERNET : 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE ENABLE : enableApnType (3sec)
 - APN TYPE DISABLE : disableApnType (3sec)
VZWAPP: 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE ENABLE : nothing (3sec)
 - APN TYPE DISABLE : disableApnType (3sec)
VZW800: 
 - parameter change : onCleanUpConnection (3sec)
 - APN TYPE ENABLE : nothing (3sec)
 - APN TYPE DISABLE : disableApnType (3sec)

*/

/*
//20120116_yunsik_DATA, 2011??12??Req.
- SDMInterface.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony) 추�?

//20120203_yunsik_DATA, ?�리
Android.mk (linux\android\vendor\qcom\proprietary\cpmanager\cpmgrif)
AndroidManifest.xml (linux\android\packages\apps\phone)
Cpmgrif.c (linux\android\vendor\qcom\proprietary\cpmanager\cpmgrif)
Cpmgrif.h (linux\android\vendor\qcom\proprietary\cpmanager\cpmgrif)
Cpmgrnative.cpp (linux\android\vendor\qcom\proprietary\cpmanager\cpmgrjni)
Cust_pantech_linux_data.h (linux\android\pantech\include)
DataConnectionTracker.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
GsmDataConnectionTracker.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony\gsm)
IdevConnMo.java (linux\android\pantech\apps\otadmextensions\src\com\innopath\activecare\dev\oem)
ISkyDataConnection.aidl (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
ISkyTelephony.aidl (linux\android\frameworks\base\telephony\java\com\android\internal\telephony\cdma)
Phone.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
SDMInterface.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
SkyDataBroadcastReceiver.java (linux\android\packages\apps\phone\src\com\android\phone)
SkyDataConInterfaceManager.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
SkyDataIntents.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
SkyTelephonyInterfaceManager.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony\cdma)

*/
#define FEATURE_P_VZW_DS_SDM

/*
//20120202_yunsik_DATA
 - erase modem (LTE/eHRPD) profile when android factory reset

Cust_pantech_modem_data.h (modem_proc\build\cust)
Ds_ehrpd_profiles.c (modem_proc\modem\datamodem\3gpp2\ds707\src)
Ds_ehrpd_profiles.h (modem_proc\modem\datamodem\3gpp2\ds707\inc)
Qmi_psdm_protocol.c (modem_proc\core\pantech\qmi\src\psdm)
Cust_pantech_linux_data.h (linux\android\pantech\include)
//AndroidManifest.xml (packages\apps\phone)
//SkyDataBroadcastReceiver.java (packages\apps\phone\src\com\android\phone)
//SkyDataIntents.java (frameworks\base\telephony\java\com\android\internal\telephony)
//Cpmgrif.c (vendor\qcom\proprietary\cpmanager\cpmgrif)

//20120316_yunsik_DATA
changed linux implementation to modem implementation
delete because PSDM ANR
*/
//#define FEATURE_P_VZW_DS_ERASE_MODEM_PROFILE_WHEN_FACTORY_RESET

/*
//20120502_yunsik_DATA
- android\vendor\qcom\proprietary\qmi-framework\qcci\src\Android.mk
- LOCAL_CFLAGS += -DQCCI_OVER_QMUX block
*/
#define FEATURE_P_VZW_DS_QMI_BUILD_OPTION

//------------------------------------------------------------------------------------------------
// 2012.02.14 cho.hyunsoo :: PSDM QMI Service Enable
// Location : /home/p12360/sq/MSM8960ICS/LINUX/android/pantech/frameworks/testmenu_server/psdm *
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_PSDM_QMI_LINUX

//------------------------------------------------------------------------------------------------
// 2012.02.14 cho.hyunsoo :: PSDM QMI �� �̿��� JAVA  JNI Interface (Java)
//  						- cpmgrif�� ��ü�ϱ� ���� interface 
// Location : /home/p12360/sq/MSM8960ICS/LINUX/android/pantech/frameworks/psdmif *
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_USING_PSDM_INTERFACE

//------------------------------------------------------------------------------------------------
// 2012.02.14 cho.hyunsoo :: PSDM QMI �� �̿��� JAVA  JNI Native Method
// Location : /home/p12360/sq/MSM8960ICS/LINUX/android/vendor/qcom/proprietary/psdmif *
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_PSDM_JNI_NATIVE

//------------------------------------------------------------------------------------------------
// 2012.02.14 cho.hyunsoo :: PSDM QMI Service Debug
// Location : /home/p12360/sq/MSM8960ICS/LINUX/android/pantech/frameworks/testmenu_server/psdm *
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_PSDM_QMI_CLINENT_DEBUG

//------------------------------------------------------------------------------------------------
// 2012.03.19 SJM Tethering Null Pointer Exception
// Tethering.java
// ConnectivityService.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_TETHERING_EXCEPTION

//------------------------------------------------------------------------------------------------
// 2012.03.28 SJM Tethering Null Pointer Exception when Add/Remove v6 interface
// Tethering.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_TETHERING_EXCEPTION_V6_INTERFACE

//------------------------------------------------------------------------------------------------
// 2012.03.28 SJM DataConnection Null Pointer Exception when checkAndUpdatePartialProtocolFailure
// IMS Iniating -> Airplane Mode on -> IMS Connected -> Null Pointer Exception in checkAndUpdatePartialProtocolFailure due to mApn is null.
// DataConnection.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_DATACONNECTION_NULL_POINTER

/*
Info : 
            VZW Reqs-LTE_Multimode_PST
              - 2.1.2.1	APN settings
                  * APN Class(ID), APN NI(Name), APN IP Type, APN Bearer, APN Disable, APN Inactivity Timer
Related Feature :
            FEATURE_P_VZW_DS_SDM
            FEATURE_P_VZW_DS_eHRPD_INTERNET_IPV6

            FEATURE_P_VZW_DS_PST_LTE_APN_SETTINGS  // Feature of Modem side
            FEATURE_P_VZW_DS_PST_EHRPD_APN_SETTINGS  // Feature of Modem side
Where : 
            Cust_pantech_linux_data.h (linux\android\pantech\include)
            PstDataApnSettings.java (linux\android\pantech\frameworks\sky_pst\java\com\pantech\service)

When : 20110320
Who : Ju.Hojin
*/
#define FEATURE_P_VZW_DS_PST_ANDROID_APN_SETTINGS


//------------------------------------------------------------------------------------------------
// 2012.03.20 SJM for Data Call State Logging
// cdmaDataConnectionTracker.java
// gsmDataConnectionTracker.java
// DataConnection.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_DATA_CALL_STATE_LOG

//------------------------------------------------------------------------------------------------
// 2012.03.23 SJM for Do not add IMS DNS address to routing table
// ConnectivityService.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_DONOT_ADD_IMS_DNS_ROUTING_TABLE

//------------------------------------------------------------------------------------------------
// 2012.03.23 SJM for Modify DualIpPartialRetry Algorithm 
// GsmDataConnectionTracker.java
// MobileDataStateTracker.java
// NetMgr_kif.c
// NetMgr_Kif.h
// NetMgr_qmi.c
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_DUALIPPARTIALRETRY_CHANGE

/*
//20120324_yunsik_DATA, //20120524_yunsik_DATA, BLOCK
- res_cache.c
- MMS IOT fail, do not query IPv6, when accuired IPv4 address
- but i will handle cache, even though no answer
*/
//#define FEATURE_P_VZW_DS_DNS_CACHE_EVEN_THOUGH_NO_ANSWER

/*
//20120524_yunsik_DATA, with SKPark_Msg
- res_cache.c => remove
- getaddrinfo.c
- res_cache.c
- res_send.c
- MMS IOT fail, do not query IPv6, when accuired IPv4 address
*/
#define FEATURE_P_VZW_DS_DNS_TTL_MMS_ONLY

/*
//20120525_yunsik_DATA, maximum cache time
- Reqs-DataRequirement.docx  1.10.1	CACHING <VZW-REQS-128- 41>
   	- In the event that the source software offers an option for setting the maximum cache time 
   	   (e.g. via a compile-time static variable), the time shall be set to a value of 24 hours. 
- Res_cache.c (bionic\libc\netbsd\resolv)
*/
#define FEATURE_P_VZW_DS_MAXIMUM_CACHE_TIME

/*
//20120328_yunsik_DATA
- UsbManager.java
- Tethering.java 
- add diag port during tethering
*/
#define FEATURE_P_VZW_DS_ADD_DIAG_PORT_WHEN_TETHERING

//------------------------------------------------------------------------------------------------
// 2012.04.05 SJM Data Disconnection after factory reset due to apnchanged event
// GsmDataConnectionTracker.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_APNCHANGED_FACTORY_RESET

//------------------------------------------------------------------------------------------------
// 2012.04.06 SJM DNS Property Changed after application connection. from OSCAR
// ConnectivityService.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_DNS1_CHANGED

//------------------------------------------------------------------------------------------------
// 2012.04.07 chhong block partial retyr call during connecting by partial retry
// ApnContext.java
// GsmDataConnectionTracker.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_MULTIPLE_PARTIAL_RETRY_BLOCKING

//------------------------------------------------------------------------------------------------
// 2012.04.07 SJM reconnect dual ip fail on IPv4
// DataConnection.java
// DataConnectionAC.java
// Phone.java
// GsmDataConnectionTracker.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_RECONNECT_DUALIPFAILURE_ON_IPV4

//------------------------------------------------------------------------------------------------
// 2012.04.10 SJM Ipv6 connection fix in HDR_LTE_ONLY_MODE
// GsmServiceStateTrakcer.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_RAT_CHANGE_GSMSST

//------------------------------------------------------------------------------------------------
// 2012.04.13 SJM Data Call List Update when modem data disconnection
// qcril_data_netctrl.c
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_CALLLIST_UPDATE

//------------------------------------------------------------------------------------------------
// 2012.04.14 SJM QOS Disable due to netmgrd crash
// init_starq.rc
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_QOS_DISALBE

//------------------------------------------------------------------------------------------------
// 2012.04.14 SJM NetworkInfo isAvailable is false when APN State is failed
// GsmDataConnectionTracker.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_APN_FAILED_STATE_BUG

//------------------------------------------------------------------------------------------------
// 2012.04.17 SJM Reconnect when routing add failed.
// ConnectivityService.java
// NetworkStateTracker.java
// MobileDataStateTracker.java
// WifiStateTracker.java
// BluetoothTetheringStateTrakcer.java
// DummyDataStateTracker.java
// EthernetDataTracker.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_ROUTE_ADD_FAIL

//------------------------------------------------------------------------------------------------
// 2012.04.17 SJM enableApnType delay 500ms when state is disconnecting.
// gsmDataConnectionTracker.java
// DataConnectionTracker.java
//------------------------------------------------------------------------------------------------
#define FEATURE_P_VZW_DS_SUPL_ENABLE_BLOCKING


#if 0

/*==========================================================================
    1. Port Bridge & AT COMMAND Feature
===========================================================================*/
/*
//20111107_yunsik_DATA, feature name rename define
<Cheetah : Apache?�서 ?�팅>
 - Dun_kevents.c (vendor\qcom\proprietary\data\port-bridge)
 - Dun_service.c (vendor\qcom\proprietary\data\port-bridge)
 - Port_bridge.c (vendor\qcom\proprietary\data\port-bridge)
 - Port_bridge.h (vendor\qcom\proprietary\data\port-bridge)
 - SkyDunService.java (packages\apps\phone\src\com\android\phone)

  20110121, swchoi
  summary:
    - EF14L??DS1_DS_PORTBRIDGE_EXTERNAL_CONNECTION��?APACHE��??�팅
    - Data manager, Sync Manager, Curi explore��??�결 ??경우, port bridge thread��?종료?�고,
       SkyDunService.java?�서 ��?app??구동?�는 ?�수��??�공??    
*/
#define FEATURE_P_VZW_DS_PORTBRIDGE_EXTERNAL_CONNECTION

  /*
  20110121, swchoi
  summary:
    - EF14L??DS1_DS_Stability_AT_COMMAND_FOR_CDMA��?APACHE��??�팅
    
    - stability test��??�한 구현 ?�항
    - port bridge��?구현?�며, init.qcom.rc?�서 CKPD-Daemon ?�비???�작 부분을 막아?�함 (CKPD-Daemon ?�행??경우)    
  */
#define FEATURE_P_VZW_DS_STABILITY_AT_CMD

  /*
  20110124, swchoi
  summary:
    - FEATURE_P_VZW_DS_PORTBRIDGE_EXTERNAL_CONNECTION ??VZW LTE AT Cmmands Required 추�?
    - +CMER, +CSO, +CSS 추�?
    - port bridge��??�용?�여 구현
    - FEATURE_P_VZW_DS_PORTBRIDGE_EXTERNAL_CONNECTION 가 define ?�어 ?�어???�작??
  */
#define FEATURE_P_VZW_DS_LTE_AT_CMD_FOR_TEST_AUTOMATION





/*==========================================================================
    4. Framework Feature
===========================================================================*/


/*
//20111103_yunsik_DATA
 - eHRPD?�서 Internet PDN��?IPv6 block. (DataRequirement (1.5.5.3 DUAL STACK OPERATION): April 2011 added 부��? 
//20110521_sjm_DATA
 - eHRPD?�서 IPv6 Data Call ?�도 막음.
*/
#define FEATURE_P_VZW_DS_DISABLE_IPV6_EXCEPT_LTE_IMS

/*
  20110724, ckhong
    - vzwapp adn ?�결 ?�도 ??reject ????mms ?�서 ?�결 ?�도??경우 바로 ?�결?�도?�도��?추�?.
   - connectivityservice.java
*/
#define FEATURE_P_VZW_DS_PS_SERVICE_ENABLE_IMMEDIATE

/*
//20111115_yunsik_DATA, from apache
 - Wifi��?MMS ?�도?�에 DNS가 wifi가 ?��????�상???�어??handleDnsConfigurationChange?�서 networkType??usedNetworkType ?�로 변��?
*/
#define FEATURE_P_VZW_DS_SPECIFIC_DNS_BUG_FIX





==> ICS?�서 tethering ?�때 rndis0 ��??�는 것으��??�악 ?�어 ?�선 막음 //20120106_yunsik_DATA
/*==========================================================================
    5. Kernel & init.rc & VPN Feature
===========================================================================*/

/*
//20110624_yunsik_DATA
LTE_3GPP_Band13_NetworkAccess_March2011.pdf
-3.2.4.8 MTU Size
The device shall set the MTU size to 1428 bytes for all PDN connectoins.

1. Apache : USB driver ?�에??MTU��?1428��??�팅?��?��? Kernel ???�정
                  init.rc : write /sys/class/net/usb0/mtu 1428
2. Cheetah : same

*/
#define FEATURE_P_VZW_DS_USB_KERNEL_MTU_SETTING




/*==========================================================================
    6. SDM Feature
===========================================================================*/











  /*
//20110513_yunsik_DATA, modified for GB version
  20110118, swchoi
  summary:
    -. REQS-ANDROID 2.14<VZW-REQS-117-81> Support to DNS address cache Time-To-Live.
    -. ?�드로이?�에?�는 AddressCache Class?�서 
       networkaddress.cache.ttl ��?networkaddress.cache.negative.ttl ??properties��??�용?�여
     TTL???��??관리하?�데, ?�는 ?�드코딩???�어?�다. (10��?
    -. ?�구?�항???�라 ?�제 DNS쿼리��??�해 받아??TTL???��?�하?�록 구현.

    1. bionic ?�서 TTL??추출?�여 JNI��??�하??dalvik?�서 참조가?�하?�록 구현.
    2. InetAddress.java?�서 bionic?�서 가?�온 TTL??addressCache???��?�하��??�하??
        addressCache.put()??ttl??추�??�여 overloading.
    3. addressCache.put() ?�서 ttl??networkaddress.cache.ttl Property??set?�줌.    
      
    AddressCache.java (LINUX\android\dalvik\libcore\luni\src\main\java\java\net)
    getaddrinfo.c (LINUX\android\bionic\libc\netbsd\net)
    InetAddress.java (LINUX\android\dalvik\libcore\luni\src\main\java\java\net)
    java_net_InetAddress.cpp (LINUX\android\dalvik\libcore\luni\src\main\native)
    netdb.h (LINUX\android\bionic\libc\include)    
  */
#define FEATURE_P_VZW_PS_DNS_CACHE_TTL

/*
//20110722_yunsik_DATA, maximum cache time ?�용
- Reqs-DataRequirement.docx ??1.10.1	CACHING <VZW-REQS-128- 41>
   	- In the event that the source software offers an option for setting the maximum cache time 
   	   (e.g. via a compile-time static variable), the time shall be set to a value of 24 hours. 
*/
#define FEATURE_P_VZW_PS_MAXIMUM_CACHE_TIME


  /*
  20110218, swchoi
  summary:
    -. REQS-ANDROID 2.9.1 Network Timer API
    -. New class NetworkTimer extends class Timer
	-. Methods scheduleAtFixedRate() and schedule() overloaded to include parameter int tolerance

    1. 기존 Timer?�작?�서 period ??tolerance��??�해��?
	2. API?�출?�마???�정 ?�식???�용?�여 random값을 ?�해 period???�해��?
	3. ?�식???�용?�는 network dormancy timer???��???VZW??문의?�요.
	4. ?�시��?network dormancy timer??EVDO CTA??30��??�용??
      
    NetworkTimer.java (E-APACHE_MSM_3035\LINUX\android\dalvik\libcore\luni\src\main\java\java\util) 추�?
	Timer.java (E-APACHE_MSM_3035\LINUX\android\dalvik\libcore\luni\src\main\java\java\util) ?�정
  */

#define FEATURE_P_VZW_PS_NET_TIMER_API

  /*
  20110511, swchoi
  summary:
    -. REQS-ANDROID 2.25 DISABLE SIP STACK IN 3G DEVICES 

    " android.net.sip.SipManager.isApiSupported() must return false by removing android.software.sip feature
    " android.net.sip.SipManager.isVoipSupported() must return false by removing android.software.sip.voip
    " The native SIP stack does not have to be removed
      
    common.mk (LINUX\android\device\qcom\common)  : ?�래?�줄 ??��.
    frameworks/base/data/etc/android.software.sip.xml:system/etc/permissions/android.software.sip.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
  */

#define FEATURE_P_VZW_PS_DISABLE_SIP_STACK



 
/*
//20110311_yunsik_DATA
 - Linux -> MDM NV RPC
*/
#ifdef FEATURE_P_VZW_INTERFACE_NV_ACCESS
#define FEATURE_P_VZW_DS_LINUX_TO_MDM_NV_RPC
#endif

#ifdef FEATURE_P_VZW_DS_LINUX_TO_MDM_NV_RPC
/*
//20110616_yunsik_DATA, modified simply
//20110311_yunsik_DATA
 - EVDO Rev change menu (rev.0 & rev.A)
*/
#define FEATURE_P_VZW_DS_EVDO_REVISION_CHANGE_MENU
/*
//20110321_yunsik_DATA
- MDM Reset command using RPC
*/
#define FEATURE_P_VZW_DS_FUSION_SESSION_STATUS_WITH_RESET
#endif



/*
//20110401_yunsik_DATA
- Original Feature name : FEATURE_PACKET_COUNTER
- packet counter
- android ?�서??3G?��?tethering?��?android 쪽에 ?��? ?��?��?lifetime��??��?�한?? (only MSM)
*/
#define FEATURE_P_VZW_DS_PACKET_COUNTER

/*
//20110401_yunsik_DATA
- Original Feature name : FEATURE_PACKET_COUNTER
- packet counter
- android ?�서??3G?��?tethering?��?android 쪽에 ?��? ?��?��?DUN��?관��?
*/
#define FEATURE_P_VZW_DS_PACKET_COUNTER_FOR_DUN


/*
//20110426 ckhong_DATA
- data connection fail cause ???��??처리?��? ?�는 문제 
- fail 발생??cause query ?�도��??�정.
-dataconnection.java ( android\frameworks\base\telephony\java\com\android\internal\telephony\ )
*/
#define FEATURE_P_VZW_DS_PS_FAIL_CAUSE

/*
//20110426 ckhong_DATA
- data connection fail cause ???��??처리 ??event logging ?�수?�서 object 문제��?fatal 발생문제. 
- logging ?��??�도��??�정. 
-dataconnectionfailcause.java ( android\frameworks\base\telephony\java\com\android\internal\telephony\ )
*/
#define FEATURE_P_VZW_DS_PS_FAIL_CAUSE_FATAL_EXCEPTION


/*
//20110503_ckhong_DATA
- dns check 부분에??null_ip ��?0.0.0.0 ??경우 ��?체크?�는 문제, null ??경우(length가 0)??체크?�도��??�정. 
- mmdataconnection.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_PS_NULL_DNS_CHECK


/*
//20110506_ckhong_DATA
- ?�이??로밍 ?�정 sync 문제 버그 fix. 
- Settings.java  ( LINUX\android\packages\apps\Settings\src\com\android\settings)
*/
#define FEATURE_P_VZW_DS_PS_FIX_ROAM_CHECK_UI_BUG

/*
//20110508_yunsik_DATA
- Settings Menu ?�리
*/
#define FEATURE_P_VZW_DS_SETTING_MENU_ARRANGE


/*
//20110629_yunsik_DATA, gateway가 null ??경우가 발생?�여 ?�수코드 ?�용?�고 time??60?�로 ?�림
//20110512_ckhong_DATA
- from EF33(SKT) EF34(KT).
- dhcpclient.c ?�서 ?�고 dataconnection.java ?�서 ?�을 경우 간헐?�으��?property 값이 ?�직 set ?��? ?�아
   java 코드?�서 ?��? 값이 ?�는 경우 발생. ?�수 코드 porting
 - original feature name : FEATURE_SKY_DS_CHANGE_DEFAULT_ROUTE_FOR_MOBILE
*/
#define FEATURE_P_VZW_DS_CHANGE_DEFAULT_ROUTE_FOR_MOBILE

/*
//20110514_ckhong_DATA
- apn enable ?�태?�서 fail발생 ??apn disable ?�키��?reconnect alarm???�해 scanning ?�태��?진행?�고,
   disconnect ?�태???��??noti 발생?��? ?�음.
*/
#define FEATURE_P_VZW_DS_STATE_BUG_FIX


/*
//20110520_ckhong_DATA
- LTE -> 1x -> LTE ��?state 변경시 LTE data ?�결 종료?��? ?�아??data service state 가 in service가 ?�닌 ?�태 
   ��?받아??connectivityservice 로는 disconnect ?�리??문제.
- DataConnectionTracker.java  (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_HANDOVER_BUG_FIX

/*
//20110521_ckhong_DATA
- IPv4 fail ��?profile disable ?�후  IPv6?�상 connected ??경우 IP Profile enable ?�켜??retry가?�하?�록보완코드.
- MmDataConnectionTracker.java  (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony)
*/
#define FEATURE_P_VZW_DS_PROFIEL_REENABLE


/*
//20110527_ckhong_DATA
- preferred data radio tech ��?eHRPD 가 QMI_WDS ?�서 ?�달?��? ?�아 Preferred mode change ??LTE ��?
  ?��??�고, ?�로?�해 pdt_modem_id  ?�러 처리?�는 문제 ?�정.
- qcril_arb.c 
- radio tech unknown ?�면 networkinfo.available ??false ��??�고 ?�런 경우 App. ?�서 ?�이???�결 ��??�어??
- ?�용?????�게?�는 문제 보완. 
- mobiledatastatetracker.java 

*/
#define FEATURE_P_VZW_DS_PS_UNKNOWN_RADIO_TECH


/*
//20110611_ckhong_DATA
- USB Tethering ?�결 ??vzwims / vzwapp /vzwadmin interfac가 ifc ???�위???�으��???interface��??�더��??�도?�는 문제  
- tethering.java , mobiledatastatetracker.java
*/
#define FEATURE_P_VZW_DS_MULTIPLE_APN_TETHERING

/*
//20110611_ckhong_DATA
- ACTION_ANY_DATA_CONNECTION_STATE_CHANGED ��?phoneStateListener 보다 ?�선 처리?�도��?변��?
- PhoneStateListener처리?�서 syncronized ?�후 intent action broadcasting ?��? ?�는 경우 1??발생.
- TelephonyRegistry.java
*/
#define FEATURE_P_VZW_DS_DATA_CON_INTENT_FAST_BROADCASTING


/*
//20110628_yunsik_DATA
- ##*RMNET*#(##*76638*#) ?�로 RMNET ?�도?�에 android??부?�이 ?�료?�기 ?�에 LTE가 먼�? connection???�어버려??
  DUN file ?�때 error 가 발생?�여 Phone process 가 죽는 ?�상??발생??
- 그래??BOOT_COMPLETE가 ???�후?�만 setDunState() ?�수?�서 file write?????�도��??�정
*/
#define FEATURE_P_VZW_DS_BLOCK_DUN_SET_BEFORE_BOOT_COMPLETED

/*
Preferred data tech??1x?��?��?MDM?��?LTE��??�작?�는 경우가 발생?�여 Workaround Code ?�용 
20110701 SJM
*/
#define FEATURE_P_VZW_DS_DATA_RADIO_TECH_FIX


/*
  20110702, ckhong
    - mdm reset 문제 
    -?�일 pdn ???��???�른 IP 버전??disconnecting ?�태?�경??setup ?�도?��? ?�도��?처리. 
   - MMDataConnectionTracker.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony\)      
*/
#define FEATURE_P_VZW_DS_BOLCKING_SETUP_ON_DISCONNECTING

/*
  20110703, ckhong
    - status bar ??LTE ?�서 1xICON 보여지??문제.
    - 3G/4G ?�이???�결 ?�태 ?��?중인??1xRTT ?��? UNKNOWN ?�로 ?�는 경우 ?�전 ?�태��??��??�고, state change ????noti?�도��?처리.
   - telephonyregistry.java
*/
#define FEATURE_P_VZW_DS_DATA_AVOID_1X_ICON_DURING_4G_CONNECTED


/*
  20110805, ckhong
  - Wi-Fi ?�결 ?�태?�서 USB Tethering ?�용??경우 default router 가 Wi-Fi 가 ?�어 
     HIPRI ��??�결??mobile data path ��??�달?��? 못하??문제��?Wi-Fi ?�결 ?�에??Wi-Fi ��?up stream interface ��??�용?�도��?
  - apache ?�서??network management service ?�서 listInterfaces() ?�서 wifi interface name ??가?�오지 못해 Wi-Fi connected ?�면
     wlan0 ��?무조��?return ?�도��??? 
*/
#define FEATURE_P_VZW_DS_USB_TETHERING_BY_WIFI

/*
//20110922_yunsik_DATA
- IPv6 browsing issue fixed
- 303611I ?�용?�에 qualcomm patch ?�용
*/
#define FEATURE_P_VZW_DS_IPV6_CONNECTIVITY_ISSUE_FIX

/*
Info : 
            If vzwinternet(SERVICE_TYPE_DEFAULT) or vzwapp(SERVICE_TYPE_VERIZON) is connected with invalid GW addr. or IPv6 only(IPv4 FAIL), 
            the device execute "tryDisconnectDataCall" to disconnect and reconnect.
            After Disconnected, the device can automatically-"trySetupDataCall" for vzwinternet by "checkAndBringUpDs"
                                                           or manually-"trySetupDataCall" for vzwapp in next vzwapp-connection try.
Related Feature :
            
Where : 
            cust_pantech_protocol_linux.h (linux\android\pantech\include)
            MMDataConnectionTracker.java (linux\android\frameworks\base\telephony\java\com\android\internal\telephony)
            
When : 20111017
Who : Ju.Hojin
*/
#define FEATURE_P_VZW_DS_TYPE_DEFAULT_AND_VERIZON_IP_ERROR_RECOVERY
#endif


/*===========================================================================
    IMS Features
===========================================================================*/
 
/*
20110322, swchoi
summary:
  - IMS관???�플?�서 공통?�로 ?�용???�이???��?�소.
  - ?��?�플?�서 ?�용?�기?�해 frameworks???�함?�킴
  - IMS??부?�과 ?�시???�작???�작?�여?�하기때문에 BootUpListener��??�함?�킴.

Android.mk (LINUX\android\frameworks\base) : 변��?
app_icon.png (LINUX\android\pantech\providers\SkyIMSProvider\res\drawable) : ?�성
strings.xml (LINUX\android\pantech\providers\SkyIMSProvider\res\values) : ?�성
strings.xml (LINUX\android\pantech\providers\SkyIMSProvider\res\values-ko) : ?�성
Android.mk (LINUX\android\pantech\providers\SkyIMSProvider) : ?�성
AndroidManifest.xml (LINUX\android\pantech\providers\SkyIMSProvider) : ?�성
BootUpListener.java (LINUX\android\pantech\providers\SkyIMSProvider\src\com\pantech\provider\skyims) : ?�성
SkyIMS.java (LINUX\android\pantech\frameworks\skyims\java\com\pantech\provider\skyims) : ?�성
SkyIMSProvider.java (LINUX\android\pantech\providers\SkyIMSProvider\src\com\pantech\provider\skyims) : ?�성
*/
#define FEATURE_P_VZW_DS_IMS_PROVIDER

/*
20120128, swchoi
summary:
  - Ecrio IMS on/off 추�?
  - IMS AKA Version 추�?

SkyIMS.java (android\pantech\frameworks\skyims\java\com\pantech\provider\skyims)
SkyIMSProvider.java (android\pantech\providers\SkyIMSProvider\src\com\pantech\provider\skyims)
SkyIMSService.java (android\packages\apps\Phone\src\com\android\phone)
*/
#ifdef FEATURE_P_VZW_DS_IMS_PROVIDER  
#define FEATURE_P_VZW_DS_IMS_PROVIDER_EXPANSION
#endif

/*
  20110404, swchoi
  summary:
    - 부??직후 IMS?�서 ?�용??Contents Provider??값들?? system properties��?변��?
    - IMS Contents Provider 변��???system properties update

AndroidManifest.xml (LINUX\android\packages\apps\Phone) : 변��?
SkyIMSService.java (LINUX\android\packages\apps\Phone\src\com\android\phone) : ?�성

  2010.04.24 add
  - vzwims??IP ?�보 추�?
  - property_perms[] ??ims관??profix추�? "ims."-> AID_RADIO, AID_SYSTEM ?�성추�?  
property_service.c (LINUX\android\system\core\init) : 변��?

  20111006 : cheetah??imsservice??start��?SkyPhoneBroadcastReceiver.java 가 ?�닌 ?�래?�서 ?�작.
                  추후 SkyPhoneBroadcastReceiver ?�일 추�??�면 IMS관???�용??중복?�로 ?�용?��? ?�는지 체크?�요??
IMSStartupReceiver.java (LINUX\android\packages\apps\Phone\src\com\android\phone) : ?�성

  20120101, swchoi ICS?�팅

  2012.01.11 add
  - LinkProperties 기반?�로 코드 ?�정
  
MobileDataStateTracker.java (LINUX\android\frameworks\base\core\java\android\net)
*/
#ifdef FEATURE_P_VZW_DS_IMS_PROVIDER  
#define FEATURE_P_VZW_DS_IMS_SERVICE
#endif  

  /*
  20111012, swchoi
  summary:
    - LTE/eHRPD IMS Connected?�후 PCO Data��?Linux?�으��??�공
    - P-CSCF Addr ��?DNS Addr ???��?�하��?IMS APP???�청 ???�겨��?
    - IPv6 ?��???처리��??�용??    
  
  20120101, swchoi ICS?�팅
  IP버전��?처리 가?�하��??�까지 APN_TYPE_VZWIMS??모두 IPV6��?간주...IPV4???��???�벤?��? ??경우 곤�???
  (DCT??notifyDataConnection() ??IPV???��??처리루틴???�어?�서 TelephonyRegistry?�서 추�??????�음.)
  
cust_pantech_protocol_linux.h (LINUX\android\pantech\include)
SIPInterface.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony) : 추�?
MobileDataStateTracker.java (LINUX\android\frameworks\base\core\java\android\net) : ?�정
?�래??PS4?�서 ?�팅
cpmgrif.c (android\vendor\qcom\proprietary\cpManager\cpmgrif)
cpmgrif.h (android\vendor\qcom\proprietary\cpManager\cpmgrif)
cpmgrnative.cpp (android\vendor\qcom\proprietary\cpManager\cpmgrjni)
ISkyTelephony.aidl (android\frameworks\base\telephony\java\com\android\internal\telephony\cdma)
SkyTelephonyInterfaceManager.java (android\frameworks\base\telephony\java\com\android\internal\telephony\cdma)
  */
#define FEATURE_P_VZW_DS_IMS_SERVER_ADDR

/*
20110403, swchoi
FEATURE_P_VZW_DS_IMS_UICC_EF_LOAD:
  - IMS?�서 ?�용??UICC?�보��?system properties��?set
  - 20110908 : ISIM EF 각각??최소길이 체크 추�?.
20120103, swchoi
  - FEATURE_P_VZW_DS_IMS_PST_UICC ???�쳐 ��?변��?
  - ISIMRecords.java?��??ICS?�서 ?�컴 기본?�로 추�???IsimUiccRecords.java ?�용

SIMRecords.java (LINUX\android\frameworks\base\telephony\java\com\android\internal\telephony\gsm)  : 변��?
IsimUiccRecords.java (android\frameworks\base\telephony\java\com\android\internal\telephony\ims)  : 변��?

20120129, swchoi
FEATURE_P_VZW_DS_IMS_UICC_SET_PUID_DOMAIN:  
  - Domain of Public User ID 값을 ISIM?�서 ?�어?�도��?변��?Read Only)
  - MSISDN Based PUID가 ?�거??Non-provisioned ??경우??default값인 vzims��??�용
  - MSISDN Based PUID가 ?�효??경우 '@'?�의 string???�용

IsimUiccRecords.java (android\frameworks\base\telephony\java\com\android\internal\telephony\ims)
*/
#ifdef FEATURE_P_VZW_DS_IMS_PROVIDER  
#define FEATURE_P_VZW_DS_IMS_UICC_EF_LOAD
#define FEATURE_P_VZW_DS_IMS_UICC_SET_PUID_DOMAIN
#endif


/*
20110403, swchoi
FEATURE_P_VZW_DS_IMS_INTENT:

20120103, swchoi
  - USIM ��?ISIM EF Loading ?�인 ?�후 APP???�용 가?�하?�록 intent broadcast
  - IMS Test Mode ?��?eHRPD Mode ��?SMS Over IP Net Ind ?�의 ?�태��?intent broadcast
  - IMS관??DB Loaded?�었?��????��?? intent broadcast
  - ISIMRecords.java?��??ICS?�서 ?�컴 기본?�로 추�???IsimUiccRecords.java ?�용
  
  SIMRecords.java (android\frameworks\base\telephony\java\com\android\internal\telephony\gsm)
  IsimUiccRecords.java (android\frameworks\base\telephony\java\com\android\internal\telephony\ims)
  SkyIMSService.java (LINUX\android\packages\apps\Phone\src\com\android\phone)
*/
#ifdef FEATURE_P_VZW_DS_IMS_PROVIDER  
#define FEATURE_P_VZW_DS_IMS_INTENT
#endif

  /*
  20120104, swchoi
  summary:
    - qmi ?�용??AKA API모듈 ?�용: PS2?��?�서 ?�쳐��?관��??�주?�요 
    - init.rc?�서 data/radio 그룹변��?��?cmpgrif service??권한 추�?
    - isim��?usim aka?�서 ?�로 ?�른 API��??�출?��?��?sky_qmi_uim_ims_aka ?�수?�의 sim type pram?��??�거?�고 ISIM?�로��??�작?�도��??�정

init_STARQ.rc (android\system\core\rootdir)

qmi_aka 모듈
android\vendor\qcom\proprietary\qmi\inc\sky_qmi_uim_ims_aka.h : 추�?
android\vendor\qcom\proprietary\qmi\src\sky_qmi_uim_ims_aka.c : 추�?
android\vendor\qcom\proprietary\qmi\src\Android.mk
android\vendor\qcom\proprietary\qmi\src\Makefile.am

qmi_aka test app
android\vendor\qcom\proprietary\qmi\sky_isim_aka\*.* :추�?
  */

  /*
  20110329, swchoi
  summary:
    - IMS SDM
      otadmGetIMSDomain()
      otadmSetIMSDomain() 
      :read only -> 관???�보가 ISIM???��?�되?�있???�기불�??? 
      otadmGetIMSSIPT1()
      otadmSetIMSSIPT1()
      otadmGetIMSSIPTf()
      otadmSetIMSSIPTf()
      otadmGetIMSSIPT2()
      otadmSetIMSSIPT2()

  20120108, swchoi
    - ICS?�서 cursor??update관??메서?��? 모두 ??��?�어 ?�정
    - update??cursor.updateXXX ��??�용?��? ?�고 ContentResolver.update��??�용??
    - sms 관??method?�도 ?�께 ?�쳐��?
      
IdevConnMo.java (LINUX\android\pantech\apps\OtadmExtensions\src\com\innopath\activecare\dev\oem)      
  */
#ifdef FEATURE_P_VZW_DS_IMS_PROVIDER  
#define FEATURE_P_VZW_DS_IMS_SDM
#endif


/*
//2012.02.04 swchoi
- 모�??�서 AT command��??�해 IMS TEST MODE ?�정???�용?�는 NV_IMS_CLIENT_ENABLED_I ???�어?��?Ecrio IMS?�서 ?�용
 SkyIMS.java (android\pantech\frameworks\skyims\java\com\pantech\provider\skyims)
 SkyIMSService.java (android\packages\apps\Phone\src\com\android\phone)
 */
#define FEATURE_P_VZW_DS_IMS_TEST_MODE_LINUX

/*  20120302 swchoi : AKAv1 With IPSec
  - FEATURE_P_VZW_DS_IPSEC_CONFIG ?�서 ?�래 config y ��??�팅?�요
    LINUX\android\kernel\arch\arm\configs\msm8960_starq_ws20_defconfig
    CONFIG_XFRM_USER , CONFIG_INET_XFRM_TUNNEL , CONFIG_INET_XFRM_MODE_TRANSPORT
    CONFIG_INET_XFRM_MODE_TUNNEL , CONFIG_INET_AH , CONFIG_INET_ESP
  - ahope??sipsad deamon???��?�서 net_admin 그룹?�성 부??��?sipsa socket???��??system권한부??
    LINUX\android\system\core\rootdir\etc\init.qcom.rc
	=> ??,star Q ??경우 init.qcom_starq.rc ?�서 추�?��??�줘?�한?? 모델 ?�당??마음?�다. 공�????�다.
  - ahope??sipsa ?�이브러리소??��?test code
    LINUX\android\pantech\frameworks\pantech_ipsec\*.*
*/
#define FEATURE_P_VZW_IMS_ECRIO_AKAV1_WITH_IPSEC
/*===========================================================================
    eHRPD Features
===========================================================================*/
#if 0
/*
20110321, SJM
  -eHRPD SDM
*/
#define FEATURE_P_VZW_DATA_EHRPD_SDM
#endif

/*===========================================================================
    LTE Features
===========================================================================*/


/*===========================================================================
    Wi-Fi Features
===========================================================================*/




#endif/* __CUST_PANTECH_LINUX_DATA_H__ */


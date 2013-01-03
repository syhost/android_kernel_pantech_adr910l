#ifndef CUST_PANTECH_H
#define CUST_PANTECH_H

#include "BOARD_REV.h"

#if defined(T_CHEETAH)
    #define CONFIG_PANTECH_CHEETAH_BOARD
#endif

/* Emergency Dload USB */
/* define after merging dload module #define FEATURE_PANTECH_DLOAD_USB*/

/*******************************************************************************
  **  PDL (LK(emergency), bootimage(phoneinfo), KERNEL(idle download))
  *******************************************************************************/
#define FEATURE_PANTECH_PDL_DLOADINFO
#define FEATURE_PANTECH_PDL_DLOAD
#define FEATURE_PANTECH_FLASH_ACCESS
#define FEATURE_PANTECH_DLOAD_USB
#define FEATURE_PANTECH_REBOOT_FOR_IDLE_DL
#define FEATURE_PANTECH_GPT_RECOVERY     //chjeon20120412@LS1 add

/*************************************************************************/
/*                            PANTECH STABILITY		                     */
/*************************************************************************/
#define F_PANTECH_VZW_PS_STABILITY_AT_CMD

#ifdef __BOOTBUILD__
/*
    ONLY BOOT BUILD FEATURE
*/

/*
      !!!!!!!!!!!!!!!!!! MUST BE DEFINED AS FOLLOWS (ONLY MODEM)!!!!!!!!!!!!!!!!!!
      FEATURE_{COMPANY_NAME}_{FUNCTION_NAME}_{ACTION_NAME}
      ex) PMIC function.

      #define FEATURE_PANTECH_PMIC
*/
// TO DO ..

#define FEATURE_PANTECH_BOOT_PMIC

#if defined(FEATURE_PANTECH_BOOT_PMIC)
#define FEATURE_PANTECH_BOOT_PMIC_POWER_ON_PROCESS
#define FEATURE_PANTECH_BOOT_PMIC_POWER_ON_MPP
#endif

#define FEATURE_PANTECH_BOOT_CHARGER
#if defined(FEATURE_PANTECH_BOOT_CHARGER)
#define FEATURE_PANTECH_BOOT_CHARGER_NO_WAIT
#endif

#define FEATURE_PANTECH_ERR_CRASH_LOGGING

/*
 * Caution!!! Enable before FEATURE_PANTECH_ERR_CRASH_LOGGING must enable under feature
 * by tarial 20120111
 */
#ifdef FEATURE_PANTECH_ERR_CRASH_LOGGING
//#define FEATURE_QHSUSB_HDLC_CDCACM   // tarial 20120111 add for USB DUMP for Modem port 
#endif

#define FEATURE_PANTECH_DDR_TINIT3_MODIFY  // 20120131 jylee , Tinit3 

#endif /* __BOOTBUILD__ */

#ifdef __MODEMBUILD__
/* 
    ONLY MODEM BUILD FEATURE
*/

/*
      !!!!!!!!!!!!!!!!!! MUST BE DEFINED AS FOLLOWS (ONLY MODEM)!!!!!!!!!!!!!!!!!!
      FEATURE_{COMPANY_NAME}_{FUNCTION_NAME}_{ACTION_NAME}
      ex) PMIC function.

      #define FEATURE_PANTECH_PMIC
*/

// TO DO ..

#define FEATURE_PANTECH_MODEL                       //chjeon20111031@LS1 add CS tool.

#endif /* __MODEMBUILD__ */


#if !defined(__KERNELBUILD__) && !defined(__MODEMBUILD__)
/*
    ONLY ANROID BUILD FEATURE
*/

/*
      !!!!!!!!!!!!!!!!!! MUST BE DEFINED AS FOLLOWS (ANDROID)!!!!!!!!!!!!!!!!!!
      CONFIG_{COMPANY_NAME}_{FUNCTION_NAME}_{ACTION_NAME}
      ex) PMIC function.
      #define CONFIG_PANTECH_PMIC

      for using BOTH (android & kernel) definition, please read engineer note about chapter 5 Arm Linux Kernel.

      IF YOU ADD FEATURE IN KERNEL , YOU CHECK THE RELEASE ENGINNER NOTE

      __KERNELBUILD__ :  for avoid redefined , this is predefined name in kernel makefile.

*/

/* TO DO define */
#define FEATURE_PANTECH_BMS_TEST   // equals 'CONFIG_PANTECH_BMS_TEST' at \LINUX\android\kernel\arch\arm\mach-msm\cust\Kconfig
/* [LS2_SYS_KIM.DONGSU 20110927] Enable AT command for OEM stability test */
#define FEATURE_PANTECH_STABILITY_AT_COMMAND
#define FEATURE_PANTECH_CMUT_AT_COMMAND
#define CONFIG_PANTECH_ERR_CRASH_LOGGING
#endif /* !defined(__KERNELBUILD__) && !defined(__MODEMBUILD__) */


#if !defined(__KERNELBUILD__)
/*
  MODEM and ANDROID Feature 
*/
#define FEATURE_PANTECH_NV_CMD //chjeon20120417@LS1 add



#endif /* !defined(__KERNELBUILD__) */

#if !defined(__KERNELBUILD__) && !defined(__MODEMBUILD__) && !defined(__BOOTBUILD__)
/* TO DO define */

#define CONFIG_PANTECH

#include "CUST_PANTECH_CAMERA.h"

/*******************************************************************************
**  Display
*******************************************************************************/
#include "CUST_PANTECH_DISPLAY.h"

/*******************************************************************************
**  TDMB
*******************************************************************************/
#include "CUST_PANTECH_TDMB.h"

/*******************************************************************************
**  USER DATA REBUILDING VERSION
*******************************************************************************/
#define FEATURE_SKY_USER_DATA_VER
#define FEATURE_SKY_FAT16_FOR_SBL3
//20111220 jwheo Data Encryption
#define FEATURE_SKY_DATA_ENCRYPTION
//20120117 jwheo SD Card Block Encryption
#if defined(T_OSCAR)
#define FEATURE_SKY_SD_BLOCK_ENCRYPTION
#endif

// 20120516 - App CRC CHECK For Factory Command(LS1)
#define F_PANTECH_APP_CRC_CHECK


#include "CUST_PANTECH_MMP.h"

/****************************************************
** SOUND
****************************************************/
#include "CUST_PANTECH_SOUND.h"

/****************************************************
** MMC(eMMC, MicroSD)
****************************************************/
#if defined(PANTECH_STORAGE_INTERNAL_FAT)
#define FEATURE_SKY_MMC
#endif

/*************************************************************************/
/****************************  PANTECH UIM ********************************/
/*************************************************************************/
#define F_PANTECH_UIM_TESTMENU


/*******************************************************************************
**  FACTORY_COMMAND
*******************************************************************************/
#define FEATURE_PANTECH_FACTORY_COMMAND
#ifdef FEATURE_PANTECH_FACTORY_COMMAND
#define PANTECH_DIAG_MSECTOR
#define FEATURE_PANTECH_CS_AUTO_TAKEOVER
#define FEATURE_PANTECH_BT_FC
#define FEATURE_PANTECH_WLAN_FC
#define F_SKYLCD_FACTORY_PROCESS_CMD
#define FEATURE_PANTECH_MEDIA_FILE_CHECK
#endif

//#define FEATURE_PANTECH_VOLUME_CTL     // P11223_120613 verizon Á¦¿Ü ¿äÃ» 

/*******************************************************************************
**  RAWDATA PARTITION ACCESS, FOR BACKUP
*******************************************************************************/
#define FEATURE_SKY_RAWDATA_ACCESS

#define PANTECH_DIAG_MSECTOR

/*******************************************************************************
**  GOTA
*******************************************************************************/
#include "CUST_PANTECH_GOTA.h"

/*******************************************************************************
**  PMIC
*******************************************************************************/
#define CONFIG_A_PANTECH_PMIC
#if defined(CONFIG_A_PANTECH_PMIC)
#define CONFIG_A_PANTECH_PMIC_SHARED_DATA
#define CONFIG_A_PANTECH_PMIC_HW_REVISION
#define CONFIG_A_PANTECH_PMIC_SILENT_BOOT
#define CONFIG_A_PANTECH_PMIC_RESET_REASON
#define CONFIG_A_PANTECH_PMIC_THERM
#if defined(T_EF44S)
#if(BOARD_VER >= WS20)
#define CONFIG_A_PANTECH_MAX17058
#endif
#elif defined(T_STARQ)
#define CONFIG_A_PANTECH_MAX17058
#elif defined(T_OSCAR)
#define CONFIG_A_PANTECH_MAX17058
#endif
#endif

#define FEATURE_PANTECH_CHARGER
#if defined(FEATURE_PANTECH_CHARGER)
#define FEATURE_PANTECH_CHARGER_OFFLINE
#endif /* FEATURE_PANTECH_CHARGER */

/****************************************************
** POWER ON/OFF REASON COUNT
****************************************************/
#define FEATURE_PANTECH_PWR_ONOFF_REASON_CNT

#if defined(T_STARQ) || defined(T_OSCAR) || defined(T_VEGAPVW) || defined(T_EF45K) || defined(T_EF46L) || defined(T_EF47S)  //chjeon20120224@LS1 add
/*******************************************************************************
 **  WIDEVINE DRM
*******************************************************************************/
#define FEATURE_PANTECH_WIDEVINE_DRM

/*******************************************************************************
 **  PANTECH CERTIFICATION FOR Image_verify
*******************************************************************************/
#define FEATURE_PANTECH_KEY_CERTIFICATION



#endif

/* 120421 LS1-JHM modified : user data backup */
#if defined(T_EF45K) || defined(T_EF46L) || defined(T_EF47S)
#define FEATURE_CS_USERDATA_BACKUP
#endif


/****************************************************
** DRM
****************************************************/
#include "CUST_PANTECH_DRM.h"

#endif /*!defined(__KERNELBUILD__) && !defined(__MODEMBUILD__) && !defined(__BOOTBUILD__)*/


/*******************************************************************************
**  WLAN
*******************************************************************************/
/* WLAN Common Feature */
#define FEATURE_PANTECH_WLAN
#define FEATURE_PANTECH_WLAN_PROCESS_CMD
#define FEATURE_PANTECH_WLAN_TESTMENU
#define FEATURE_PANTECH_WLAN_RAWDATA_ACCESS  
#define FEATURE_PANTECH_WLAN_MAC_ADDR_BACKUP
#define FEATURE_PANTECH_WLAN_QCOM_PATCH
#define FEATURE_PANTECH_WLAN_FOUR_MAC_ADDRESS // 20120204 thkim_wifi This feauture is applied to MSM8960-WCN3660 MODEL
#define FEATURE_PANTECH_WLAN_TRP_TIS // 2012-04-09, Pantech only, ymlee_p11019, to config & test TRP TIS
#define FEATURE_PANTECH_WLAN_TRP_TIS_ROHDE // 20120303 thkim_wifi for trp/tis for Rohde&Schwarz
#define FEATURE_PANTECH_5G_DPD_ENABLE_AUTO_TEST // 2012-04-02, Pantech only, ymlee_p11019, On Auto test mode 5G DPD enabled

/* WLAN Model Feature */
#if defined(T_EF45K) || defined(T_EF46L) || defined(T_EF47S)
#define FEATURE_PANTECH_WLAN_SCAN
#endif 

#define FEATURE_PANTECH_WLAN_FOUR_MAC_ADDRESS

#define FEATURE_PANTECH_WLAN_TRP_TIS_ROHDE
#define FEATURE_PANTECH_5G_DPD_ENABLE_AUTO_TEST

/*************************************************************************/
/*                               PST                                     */
/*************************************************************************/
#define F_PANTECH_PST
#define F_PANTECH_PST_ROOT_PROCESS

/*******************************************************************************
**  SENSOR
*******************************************************************************/
#define FEATURE_PANTECH_DSPS
#define FEATURE_PANTECH_GSIFF_DELAY

/*******************************************************************************
 **  PANTECH ROOTING CHECK		//lsi@ls1
*******************************************************************************/
#define F_PANTECH_OEM_ROOTING

/*******************************************************************************
 **  PANTECH SECURE BOOT		//lsi@ls1
*******************************************************************************/
#if defined(T_STARQ) || defined(T_VEGAPVW)
#define F_PANTECH_SECBOOT
#endif

/*******************************************************************************
**  PM
*******************************************************************************/
#define PANTECH_CHARGER_MONITOR_TEST //20120229_khlee_pm : for chargerMonitor test(#8378522#)
#define FEATURE_PANTECH_BATTERY_DUMMY
/*******************************************************************************
** USB 
*******************************************************************************/
#if defined(T_STARQ) || defined(T_VEGAPVW)
#define FEATURE_ANDROID_PANTECH_USB_CDFREE
#endif

#define FEATURE_ANDROID_PANTECH_USB_PROPERTY_SETTING
#define FEATURE_ANDROID_PANTECH_USB_TESTMENU
#define FEATURE_HSUSB_SET_SIGNALING_PARAM
#define FEATURE_PANTECH_USB_CABLE_CONNECT
/*T_EF47S, T_EF46L, T_EF45K, T_CHEETAH, T_VEGAVZW    */
#if defined(T_EF46L) ||  defined(T_EF47S) || defined(T_EF45K)
#if(BOARD_VER >= TP10)
    #define FEATURE_ANDROID_PANTECH_USB_OTG_MODE
#endif
#endif
#define FEATURE_PANTECH_MSG_ONOFF // QXDM MSG ONOFF
/****************************************************
** MTC
****************************************************/
#if defined(T_EF45K) || defined(T_EF46L) || defined(T_EF47S)
//#define CONFIG_FEATURE_PANTECH_MDS_MTC   /* MTC */
//#define CONFIG_FEATURE_PANTECH_MAT      /* MAT */
//#define CONFIG_FEATURE_DIAG_LARGE_PACKET
#endif

/****************************************************
** RF
****************************************************/
#if defined(T_EF46L) || defined(T_EF47S) || defined(T_EF45K)
#define FEATURE_RF_TUNABLE_ANT_TEST //EF46L tunable ant test bin
#endif

/****************************************************
** CPRM
****************************************************/
#if defined(T_VEGAPKDDI)
#define FEATURE_CPRM_INTERFACE //Vega Premia KDDI
#endif
/*******************************************************************************
** Release/Debug User/Eng mode 
*******************************************************************************/
#if defined(FEATURE_AARM_RELEASE_MODE)
#define FEATURE_SW_RESET_RELEASE_MODE // use in release mode
#endif

#if defined(T_STARQ)
#define F_PANTECH_UTS_POWER_UP // 20120302 LS1_hskim add for UTS
#endif
/*******************************************************************************
** Android Patternlock Reset
*******************************************************************************/
#define FEATURE_PANTECH_PATTERN_UNLOCK
#if defined(T_STARQ) || defined(T_OSCAR) || defined(T_VEGAPVW)
	#define FEATURE_PANTECH_PATTERN_MIN
#endif

/*
  20110923 hyko
  cust_pantech_protocol_linux ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
*/
#include "cust_pantech_protocol_linux.h"

/*
//20120119_yunsik_DATA
FEATURE_P_VZW_DS_CUST_INCLUDE
*/
#include "cust_pantech_linux_data.h"

/*************************************************************************/
/*                            PANTECH UTS(220 ~ 239(                     */
/*************************************************************************/
#define F_PANTECH_UTS_HWID
#define F_PANTECH_UTS_DISP_CAPTURE
#define FEATURE_P_VZW_UTS_SMS

#define F_PANTECH_PROC_MODE_RESET

/*************************************************************************/
/*                            FACTORY INIT MODEM                         */
/*************************************************************************/
#define F_PANTECH_FACTORY_INIT_MODEM /* 20120220 LS1_hskim */

#endif/* CUST_PANTECH_H */

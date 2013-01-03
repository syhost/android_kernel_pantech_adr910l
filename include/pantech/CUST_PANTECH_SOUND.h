#ifndef __CUST_PANTECH_SOUND_H__
#define __CUST_PANTECH_SOUND_H__

#define FEATURE_SND_MODIFICATION // SangwonLee 110916

/* 
  2012/02/20 
  QualComm Patch feature
*/
#define FEATURE_PANTECH_SND_QCOM_CR

/* 
  2012/03/12
  Feature must be applied to all models
*/
#define FEATURE_PANTECH_SND

/*
 2011/10/24 [P11157]
  ������ �ؿܸ����� �����ϱ� ���� FEATURE�� ����
*/
#if defined(T_EF45K) || defined(T_EF46L) || defined(T_EF47S)
#define FEATURE_PANTECH_SND_DOMESTIC
#define FEATURE_SKY_QSOUND_QFX
#define FEATURE_SKYSND_LPA  // for QSound LPA
#elif defined(T_CHEETAH) || defined(T_STARQ) || defined(T_RACERJ) || defined(T_VEGAPVW) || defined(T_VEGAPKDDI)
#define FEATURE_PANTECH_SND_ABROAD
#elif defined(T_CSFB) || defined(T_SVLTE) // temp
#define FEATURE_PANTECH_SND_ABROAD  
#elif defined(T_OSCAR)
#define FEATURE_PANTECH_SND_ABROAD
#define FEATURE_DOLBY_AUDIOEFFECT_DM
#define DOLBY_AUDIOEFFECT_DM
#define FEATURE_SKYSND_LPA
#define FEATURE_OSCAR_POWER_SAVE_SND /*For Power Save TF*/
#define FEATURE_OSCAR_BT_GROUPPING /*For NAC certification*/
#define FEATURE_OSCAR_MIC_LOOPBACK /*For Test Mode, 1, 2st loopback*/
#else
    #error "FEATURE_PANTECH_SND ? DOMESTIC or ABROAD"
#endif

#if 0 //defined(T_EF47S) || defined(T_EF46L)
#define FEATURE_PANTECH_LTE_VT
#endif

#if defined(T_STARQ)
#define FEATURE_PANTECH_SND_STARQ

#define FEATURE_PANTECH_SND_ELECTOVOX
#define FEATURE_PANTECH_SND_BT_ECNR
#define FEATURE_PANTECH_SND_NR_DEV

#define FEATURE_OSCAR_BT_GROUPPING
#elif defined (T_VEGAPVW)
#define FEATURE_PANTECH_SND_VEGAPVW
#endif

#endif /* __CUST_PANTECH_SOUND_H__ */
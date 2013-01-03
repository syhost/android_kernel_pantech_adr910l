/************************************************************************************************
**
**    PANTECH AUDIO
**
**    FILE
**        pantech_audio.h
**
**    DESCRIPTION
**        This file contains pantech audio defines
**
**    Copyright (c) 2012 by PANTECH Incorporated.  All Rights Reserved.
*************************************************************************************************/

/************************************************************************************************
** Definition
*************************************************************************************************/
/* Default Register Value */ 

#define PANTECH_AUDIO_IOCTL_MAGIC		'z'
#define PANTECH_AUDIO_NR_OFF				_IOW(PANTECH_AUDIO_IOCTL_MAGIC, 0, unsigned)
#define PANTECH_AUDIO_NR_ON				_IOW(PANTECH_AUDIO_IOCTL_MAGIC, 1, unsigned)
#define PANTECH_AUDIO_NR_CTL				_IOW(PANTECH_AUDIO_IOCTL_MAGIC, 2, unsigned)
/*
#define PANTECH_AUDIO_NR_HANDSET_ON		_IOW(PANTECH_AUDIO_IOCTL_MAGIC, 2, unsigned)
#define PANTECH_AUDIO_NR_SPEAKER_ON		_IOW(PANTECH_AUDIO_IOCTL_MAGIC, 3, unsigned)
#define PANTECH_AUDIO_NR_HEADSET_ON		_IOW(PANTECH_AUDIO_IOCTL_MAGIC, 4, unsigned)
*/

/*=========================================================================*/


#ifndef __TIMER_H__
#define __TIMER_H__

#include "main.h"
#define CH57xNETTIMEPERIOD 10
void Timer0Init(UINT32 time);
void CH57xNET_TimeIsr(UINT8 time);

#endif

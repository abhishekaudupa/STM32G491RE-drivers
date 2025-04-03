#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "config.h"

extern volatile uint32_t sysTick;

void enable_systick();

#endif

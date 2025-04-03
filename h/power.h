#ifndef __POWER_H__
#define __POWER_H__

#include "config.h"

typedef enum {
    VCore_Range_1_Boost,
    VCore_Range_1_Normal,
    VCore_Range_2,
} VCore_Power_Mode;

/* public interface */
VCore_Power_Mode get_vcore_power_mode();

#endif

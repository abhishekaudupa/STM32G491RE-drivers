#ifndef __POWER_H__
#define __POWER_H__

#include "config.h"

LOCAL void 	enable_power_interface();
LOCAL void 	disable_power_interface();

uint32_t 	get_vos();
uint32_t	get_r1_mode();

#endif

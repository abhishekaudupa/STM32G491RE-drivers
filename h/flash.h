#ifndef __FLASH_H__
#define __FLASH_H__

#include "config.h"

LOCAL void 	enable_flash_interface_registers();
LOCAL void	disable_flash_interface_registers();

void 		set_flash_read_latency(uint32_t wait_states);

#endif

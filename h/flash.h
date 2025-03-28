#ifndef __FLASH_H__
#define __FLASH_H__

#include "config.h"

LOCAL void 	enable_flash_interface_registers();
LOCAL void	disable_flash_interface_registers();
LOCAL void 	update_flash_read_latency_r1(const uint32_t ahb_speed);
LOCAL void 	set_flash_read_latency(uint32_t wait_states);

void 		update_flash_read_latency(const uint32_t ahb_speed);

#endif

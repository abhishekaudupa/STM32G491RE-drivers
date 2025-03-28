#ifndef __FLASH_H__
#define __FLASH_H__

#include "config.h"

#define FLASH_LATENCY_0 	0x0U
#define FLASH_LATENCY_1 	0x1U
#define FLASH_LATENCY_2 	0x2U
#define FLASH_LATENCY_3 	0x3U
#define FLASH_LATENCY_4 	0x4U
#define FLASH_LATENCY_5 	0x5U
#define FLASH_LATENCY_6 	0x6U
#define FLASH_LATENCY_7 	0x7U
#define FLASH_LATENCY_8 	0x8U
#define FLASH_LATENCY_9 	0x9U
#define FLASH_LATENCY_10 	0xAU
#define FLASH_LATENCY_11 	0xBU
#define FLASH_LATENCY_12 	0xCU
#define FLASH_LATENCY_13 	0xDU
#define FLASH_LATENCY_14 	0xEU
#define FLASH_LATENCY_15 	0xFU

/* internal helper functions */
LOCAL void 	enable_flash_interface();
LOCAL void	disable_flash_interface();
LOCAL void 	update_flash_read_latency_r1(const uint32_t ahb_speed);
LOCAL void 	set_flash_read_latency(uint32_t wait_states);

/* public interface */
void 		update_flash_read_latency(const uint32_t ahb_speed);
 
#endif

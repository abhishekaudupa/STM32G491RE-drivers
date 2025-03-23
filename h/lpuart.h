#ifndef __LPUART_H__
#define __LPUART_H__

#include <stdint.h>
#include "config.h"

//#define LPUART1_POLL_MODE

LOCAL uint32_t 	get_lpuart1_clock_speed();
LOCAL uint32_t 	get_lpuart1_presc();
LOCAL void 	set_lpuart1_presc(const uint32_t presc);
LOCAL void 	set_lpuart1_word_len(const uint8_t len);

void 		init_lpuart1(const uint32_t baud_rate);
void 		tx_lpuart1(const char *data);
void 		tx_single_lpuart1(const char *const data);

#ifdef LPUART1_POLL_MODE
char* 		rx_poll_lpuart1(char *const buf);
#endif

#endif

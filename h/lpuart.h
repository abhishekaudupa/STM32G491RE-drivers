#ifndef __LPUART_H__
#define __LPUART_H__

#include "config.h"

//#define LPUART1_POLL_MODE

/* public interface */
void 		init_lpuart1(const uint32_t baud_rate);
void 		tx_lpuart1(const char *data);
void 		tx_single_lpuart1(const char *const data);

#ifdef LPUART1_POLL_MODE
char* 		rx_poll_lpuart1(char *const buf);
#endif

#endif

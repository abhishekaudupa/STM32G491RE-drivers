#ifndef __PRINT_H__
#define __PRINT_H__

#include "config.h"

/* internal helper functions */
LOCAL void 	swap(char *const c1, char *const c2);
LOCAL void 	reverse_num(char *const buf, const uint8_t len);
LOCAL uint8_t 	stuff_int(int val, char *buf);
LOCAL uint8_t 	stuff_inth(uint32_t val, char *const buf);

/* public interface */
void 		my_printf(char *fmt, ...);

#endif

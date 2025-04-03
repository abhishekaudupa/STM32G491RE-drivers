#include <stdarg.h>
#include "lpuart.h"
#include "print.h"

#define PRINT_BUF_MAX 512

/* internal helper functions */
LOCAL void 	swap(char *const c1, char *const c2);
LOCAL void 	reverse_num(char *const buf, const uint8_t len);
LOCAL uint8_t 	stuff_int(int val, char *buf);
LOCAL uint8_t 	stuff_inth(uint32_t val, char *const buf);

void my_printf(char *fmt, ...) {
    char buf[PRINT_BUF_MAX];
    va_list args;
    va_start(args, fmt);

    uint8_t index;
    index = 0;
    while(*fmt) {
	if(*fmt == '%') {
	    fmt++;
	    if(*fmt == 'd') {
		int val = va_arg(args, int);
		index += stuff_int(val, buf + index);
	    }

	    if(*fmt == 'x') {
		uint32_t val = va_arg(args, uint32_t);
		index += stuff_inth(val, buf + index);
	    }

	    if(*fmt == 'c') {
		int val = va_arg(args, int);
		buf[index++] = val;
	    }

	    if(*fmt == 's') {
		char *val = va_arg(args, char*);
		while(*val)
		    buf[index++] = *val++;
	    }
	} else if(*fmt == '\n') {
	    buf[index++] = *fmt;
	    buf[index++] = '\r';
	} else {
	    buf[index++] = *fmt;
	}
	fmt++;
    }

    buf[index++] = 0;
    tx_lpuart1(buf);
    va_end(args);
}

LOCAL void swap(char *const c1, char *const c2) {
    char swp = *c1;
    *c1 = *c2;
    *c2 = swp;
}

LOCAL void reverse_num(char *const buf, const uint8_t len) {
    uint8_t i = 0;
    uint8_t j = len - 1;
    while(j > i) {
	swap(buf + i, buf + j);
	i++;
	j--;
    }
}

LOCAL uint8_t stuff_int(int val, char *buf) {
    uint8_t index = 0;
    uint8_t negative_flag = 0;
    if(!val) {
	*buf = '0';
	return 1;
    }
    if(val < 0) {
	val = ~val + 1U;
	negative_flag = 1;
	*buf++ = '-';
    }
    while(val) {
	buf[index++] = '0' + val % 10;
	val /= 10;
    }
    reverse_num(buf, index);
    return index + negative_flag;
}

LOCAL uint8_t stuff_inth(uint32_t val, char *const buf) {
    uint8_t index = 0;
    if(!val) {
	*buf = '0';
	return 1;
    }
    while(val) {
	buf[index] = '0' + val % 16;
	if(buf[index] > '9')
	    buf[index] = 'a' + buf[index] - '9' - 1;
	index++;
	val /= 16;
    }
    reverse_num(buf, index);
    return index;
}

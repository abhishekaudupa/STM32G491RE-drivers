#include <stddef.h>
#include "stm32g491xx.h"
#include "startup.h"
#include "nucleoG491RE.h"
#include "flash.h"
#include "clock.h"
#include "lpuart.h"
#include "print.h"
#include "delay.h"
#include "power.h"
#include "deviceid.h"

void flash_dump() {
    uint8_t *fb = (uint8_t*)FLASH_BASE;

    uint32_t i = 0;
    uint8_t c = 0;
    while(fb <= (uint8_t*)0x0807FFFF) {
	if(i % 16 == 0)
	    my_printf("\n 0x%x   ", fb);
	if(*fb > 0xF)
	    my_printf("%x ", *fb);
	else
	    my_printf("0%x ", *fb);
	if(*fb == 0xff)
	    c++;
	else
	    c = 0;
	if(c == 10) {
	    my_printf("\nDone.\n");
	    return;
	}
	fb++;
	i++;
    }
}

void main(void) {
    set_sysclk_pll_100MHz();
    init_lpuart1(115200);
    my_printf("lpuart initialized. sysclk is %d MHz\n", get_sysclk_speed() / 1000000);

    flash_dump();

#ifdef LPUART1_POLL_MODE
    char buf[256];
    uint8_t ri = 0;
    uint8_t ti = 0;

    while(1) {
	if(rx_poll_lpuart1(buf + ri) != NULL) {
	    tx_single_lpuart1(buf + ti);
	    ri++;
	    ti++;
	}
    }
#endif
}

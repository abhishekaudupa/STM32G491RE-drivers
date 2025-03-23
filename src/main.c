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

void main(void) {
    set_sysclk_pll_100MHz();
    init_lpuart1(115200);
    my_printf("lpuart initialized. sysclk is %d MHz\n", get_sysclk_speed() / 1000000);

    my_printf("VOS = %d\n", get_vos());
    my_printf("R1 Mode = %d\n", get_r1_mode());

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

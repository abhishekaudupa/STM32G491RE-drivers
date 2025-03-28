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

void main(void) {
    enable_user_led();
    enable_systick();

    init_lpuart1(115200);
    my_printf("lpuart initialized. sysclk is %d MHz\n", get_sysclk_speed() / 1000000);
    
    set_sysclk_pll(PLL_HSE24_144);
    init_lpuart1(115200);
    my_printf("lpuart initialized. sysclk is %d MHz\n", get_sysclk_speed() / 1000000);
    
    set_sysclk_hsi();
    user_led_on();
    init_lpuart1(115200);
    my_printf("lpuart initialized. sysclk is %d MHz\n", get_sysclk_speed() / 1000000);


    set_sysclk_pll(PLL_HSE24_96);
    init_lpuart1(115200);
    my_printf("lpuart initialized. sysclk is %d MHz\n", get_sysclk_speed() / 1000000);

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

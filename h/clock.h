#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "config.h"

#define __MHz(n)	(n * 1000000)
#define __kHz(n)	(n * 1000)

typedef enum {
    SYSCLK_HSI16	= 1U,
    SYSCLK_HSE		= 2U,
    SYSCLK_PLL		= 3U
} Sysclk_Source;

LOCAL void 	set_sysclk(Sysclk_Source sysclk_src);
LOCAL void 	hse_on();
LOCAL void 	hsi_on();
LOCAL void 	enable_pll();
LOCAL void 	disable_pll();
LOCAL uint32_t 	get_pllr_speed();
LOCAL uint32_t 	get_pll_src_speed();
LOCAL void 	update_sysclk();
LOCAL void 	update_hclk();
LOCAL void 	update_pclk1();
LOCAL void 	update_pclk2();
LOCAL uint32_t 	get_pllm();
LOCAL uint32_t 	get_plln();
LOCAL uint32_t 	get_pllr();
LOCAL void 	set_pllm();
LOCAL void 	set_plln();
LOCAL void 	set_pllr();
LOCAL void 	set_ahb_presc(const uint16_t ahb_presc);

uint32_t 	get_sysclk_speed();
uint32_t 	get_hclk_speed();
uint32_t 	get_pclk1_speed();
uint32_t 	get_pclk2_speed();
void 		update_clocks();
void 		set_sysclk_hse();
void 		set_sysclk_pll_100MHz();

#endif

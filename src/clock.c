#include "clock.h"
#include "flash.h"
#include "power.h"

uint32_t sysclk, hclk, pclk1, pclk2;
SysClk_Source sysclk_source;

/* internal helper functions */
LOCAL void 		set_sysclk(SysClk_Source sysclk_src);
LOCAL void 		hse_on();
LOCAL void 		hsi_on();
LOCAL void 		disable_pll();
LOCAL void 		enable_pll();
LOCAL void 		update_sysclk();
LOCAL void 		update_hclk();
LOCAL void 		update_pclk1();
LOCAL void 		update_pclk2();
LOCAL void 		update_pclk(uint32_t *const pclk, const uint8_t ppre);
LOCAL uint32_t 		get_pllr_speed();
LOCAL uint32_t 		get_pll_src_speed();
LOCAL uint32_t 		get_pllm();
LOCAL uint32_t 		get_plln();
LOCAL uint32_t 		get_pllr();
LOCAL void 		set_pllm(const uint32_t m);
LOCAL void 		set_plln(const uint32_t n);
LOCAL void 		set_pllr(const uint32_t r);
LOCAL void 		set_ahb_presc(const uint16_t ahb_presc);
LOCAL void 		configure_pll(PLLClk_Source pll_src, const uint32_t m, const uint32_t n, const uint32_t r);
LOCAL void 		transition(const uint32_t target_ahb_speed, SysClk_Source sysclk_src);
LOCAL void 		transition_internal(SysClk_Source sysclk_src);
LOCAL SysClk_Source 	get_sysclk_source();

void update_clocks() {
    /* 
     * do not change the order of these calls. 
     * hclk depends on sysclk and pclk depends 
     * on hclk. 
     */
    update_sysclk();
    update_hclk();
    update_pclk1();
    update_pclk2();
}

uint32_t get_sysclk_speed() {
    return sysclk;
}

uint32_t get_hclk_speed() {
    return hclk;
}

uint32_t get_pclk1_speed() {
    return pclk1;
}

uint32_t get_pclk2_speed() {
    return pclk2;
}

void set_sysclk_hsi() {
    hsi_on();
    transition(__MHz(16), SysClk_HSI16);
    update_clocks();
}

void set_sysclk_hse() {
    hse_on();
    transition(__MHz(24), SysClk_HSE);
    update_clocks();
}

void set_sysclk_pll(PLLClk_Source pll_src, 
	const uint32_t m, 
	const uint32_t n, 
	const uint32_t r, 
	const uint32_t target_ahb_speed) {

    /* 
     * The parameter 'target_ahb_speed' could be calculated using the 
     * PLL input clock speed, m, n and r. Refer datasheet to see how. 
     * Yet, I'm passing it into this function as a parameter to avoid 
     * the arithmetic overhead involved and also because I have 
     * predefined macros that directly provide these numbers without 
     * any overhead.
     * This function is called using predefined macros as function input.
     * example: set_sysclk_pll(PLL_HSI_48). 
     * 'PLL_HSI_48' expands to 'PLLClk_HSI, PLLM_2, PLLN_12, PLLR_2, 48000000U'
     * and these in turn provide the correct values of these arguments,
     * precluding the headache of having to calculate these numbers 
     * programmatically.
     */

    if(get_sysclk_source() == SysClk_PLL) {
	/* 
	 * we cannot configure the PLL while it is being 
	 * used as the system clock source. So we're gonna
	 * temporarily switch to HSI16 as the system clock.
	 */
	set_sysclk_hsi();
    }

    configure_pll(pll_src, m, n, r);

    /* enable PLL R output */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    /* switch the system clock to PLL */
    transition(target_ahb_speed, SysClk_PLL);

    update_clocks();
}

LOCAL void set_sysclk(SysClk_Source sysclk_src) {
    /* Set the system clock as required */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | (sysclk_src << RCC_CFGR_SW_Pos);

    /* ensure it has taken effect */
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (uint32_t)(sysclk_src << RCC_CFGR_SWS_Pos));

    /* update status */
    sysclk_source = sysclk_src;
}

LOCAL void hse_on() {
    /* switch on HSE */
    RCC->CR |= RCC_CR_HSEON;

    /* wait for HSE to become stable */
    while(!(RCC->CR & RCC_CR_HSERDY));
}

LOCAL void hsi_on() {
    /* switch on HSI */
    RCC->CR |= RCC_CR_HSION;

    /* wait for HSI to become stable */
    while(!(RCC->CR & RCC_CR_HSIRDY));
}

LOCAL void disable_pll() {
    /* disable PLL */
    RCC->CR &= ~RCC_CR_PLLON;

    /* wait for PLLRDY to be cleared */
    while(RCC->CR & RCC_CR_PLLRDY_Msk);
}

LOCAL void enable_pll() {
    /* enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* wait for PLLRDY to be set */
    while(!(RCC->CR & RCC_CR_PLLRDY_Msk));
}

LOCAL void update_sysclk() {
    switch(READ_REG_BITS(RCC->CFGR, RCC_CFGR_SWS)) {
	case 1U:		/* HSI16 - 16 MHz */
	    sysclk = __MHz(16);
	    break;

	case 2U:		/* HSE - 24 MHz */
	    sysclk = __MHz(24);
	    break;

	case 3U:		/* PLLR - Various */
	    sysclk = get_pllr_speed();
	    break;
    }
}

LOCAL void update_hclk() {
    uint8_t hpre = READ_REG_BITS(RCC->CFGR, RCC_CFGR_HPRE);
    switch(hpre) {
	case 0U:
	    hclk = sysclk;
	    break;

	case 8U:
	case 9U:
	case 10U:
	case 11U:
	    hclk = (sysclk >> (hpre - 7));
	    break;

	case 12U:
	case 13U:
	case 14U:
	case 15U:
	    hclk = (sysclk >> (hpre - 6));
	    break;
    }
}

LOCAL void update_pclk1() {
    uint8_t ppre1 = READ_REG_BITS(RCC->CFGR, RCC_CFGR_PPRE1);
    update_pclk(&pclk1, ppre1);
}

LOCAL void update_pclk2() {
    uint8_t ppre2 = READ_REG_BITS(RCC->CFGR, RCC_CFGR_PPRE2);
    update_pclk(&pclk2, ppre2);
}

LOCAL void update_pclk(uint32_t *const pclk, const uint8_t ppre) {
    switch(ppre) {
	case 0U:
	    *pclk = hclk;
	    return;

	case 4U:
	case 5U:
	case 6U:
	case 7U:
	    *pclk = (hclk >> (ppre - 3));
	    return;
    }
}

LOCAL uint32_t get_pllr_speed() {

    uint32_t input = get_pll_src_speed();

    /* source clock division by M */
    input /= get_pllm();

    /* VCO loopback multiplication by N */
    input *= get_plln();

    /* final division by R */
    return (input / get_pllr());
}

LOCAL uint32_t get_pll_src_speed() {
    switch(READ_REG_BITS(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC)) {
	case 2U:		/* HSI16 - 16 MHz */
	    return __MHz(16);

	case 3U:		/* HSE - 24 MHz */
	    return __MHz(24);
    }

    /* avoid compiler warning */
    return 0;
}

LOCAL uint32_t get_pllm() {
    return READ_REG_BITS(RCC->PLLCFGR, RCC_PLLCFGR_PLLM) + 1;
}

LOCAL uint32_t get_plln() {
    return READ_REG_BITS(RCC->PLLCFGR, RCC_PLLCFGR_PLLN);
}

LOCAL uint32_t get_pllr() {
    return (READ_REG_BITS(RCC->PLLCFGR, RCC_PLLCFGR_PLLR) + 1) * 2;
}

LOCAL void set_pllm(const uint32_t m) {
    /* clear PLLM */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;

    /* set PLLM */
    RCC->PLLCFGR |= m << RCC_PLLCFGR_PLLM_Pos;
}

LOCAL void set_plln(const uint32_t n) {
    /* clear PLLN */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;

    /* set PLLN */
    RCC->PLLCFGR |= n << RCC_PLLCFGR_PLLN_Pos;
}

LOCAL void set_pllr(const uint32_t r) {
    /* clear PLLR */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;

    /* set PLLR */
    RCC->PLLCFGR |= r << RCC_PLLCFGR_PLLR_Pos;
}

LOCAL void set_ahb_presc(const uint16_t ahb_presc) {

    /* HPRE bits */
    uint32_t hpre = ahb_presc << RCC_CFGR_HPRE_Pos;

    /* clear HPRE bits */
    RCC->CFGR &= ~RCC_CFGR_HPRE;

    /* write HPRE bits */
    RCC->CFGR |= hpre;

    /* ensure it has taken */
    while((RCC->CFGR & hpre) != hpre);

    update_hclk();
}

LOCAL void configure_pll(PLLClk_Source pll_src, const uint32_t m, const uint32_t n, const uint32_t r) {

    disable_pll();

    /* clear PLL M,N,R and clock source bits */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM | ~RCC_PLLCFGR_PLLN | ~RCC_PLLCFGR_PLLR | ~RCC_PLLCFGR_PLLSRC | ~RCC_PLLCFGR_PLLREN;

    /* set M, N and R */
    set_pllm(m);
    set_plln(n);
    set_pllr(r);

    /* select clock source */
    switch(pll_src) {
	case PLLClk_HSE:
	    hse_on();
	    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	    break;

	case PLLClk_HSI:
	    hsi_on();
	    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
	    break;
    }

    enable_pll();
}

LOCAL void transition(const uint32_t target_ahb_speed, SysClk_Source sysclk_src) {
    /*
     * If system clock speed increases after this, we must first 
     * set the appropriate flash read latency and then switch the 
     * clock. If system clock speed decreases we must switch the 
     * clock and then set the appropriate flash read latency. 
     */
    if(target_ahb_speed >= get_sysclk_speed()) {
	update_flash_read_latency(target_ahb_speed);
	transition_internal(sysclk_src);
    } else {
	transition_internal(sysclk_src);
	update_flash_read_latency(target_ahb_speed);
    }
}

LOCAL void transition_internal(SysClk_Source sysclk_src) {

    /* This is a little dance that the datasheet requires us 
     * to do if the system clock speed crosses the threshold 
     * of 80MHz from below or above. But I've chosen to do 
     * this dance regardless; it's just safe. */

    /* set AHB prescaler to divide by 2 - dance move */
    set_ahb_presc(HPRE_2);

    set_sysclk(sysclk_src);

    /* wait for atleast 1us - dance move */
    for(volatile int i = 0; i < 100; ++i);

    /* set AHB prescaler to no division - dance move */
    set_ahb_presc(HPRE_1);
}

LOCAL SysClk_Source get_sysclk_source() {
    return sysclk_source;
}

#include "stm32g491xx.h"
#include "flash.h"
#include "clock.h"
#include "power.h"

uint32_t sysclk, hclk, pclk1, pclk2;

#define __MHz(n)	(n * 1000000)
#define __kHz(n)	(n * 1000)

void update_clocks() {
    /* do not change the order of these calls */
    // TODO: force reset or something if one of these is 0.
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

void set_sysclk_hse() {
    hse_on();

    /* set flash latency to 0 wait states */
    set_flash_read_latency(0);

    set_sysclk(SYSCLK_HSE);

    update_clocks();
}

void set_sysclk_pll_100MHz() {

    hse_on();
    disable_pll();

    /* clear PLL M,N,R and clock source bits */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM | ~RCC_PLLCFGR_PLLN | ~RCC_PLLCFGR_PLLR | ~RCC_PLLCFGR_PLLSRC | ~RCC_PLLCFGR_PLLREN;

    /* M = 2, N = 50, R = 6 */
    set_pllm(2);
    set_plln(50);
    set_pllr(6);

    /* PLL clock source is HSE 24MHz */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

    enable_pll();

    /* enable PLL R output */
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    /* set flash latency to 3 wait states */
    set_flash_read_latency(3);

    /* set AHB prescaler to divide by 2 */
    //RCC->CFGR &= ~RCC_CFGR_HPRE;
    //RCC->CFGR |= (8U << RCC_CFGR_HPRE_Pos);
    set_ahb_presc(2);

    set_sysclk(SYSCLK_PLL);

    /* wait for atleast 1us */
    for(volatile int i = 0; i < 100; ++i);

    /* set AHB prescaler to no division */
    //RCC->CFGR &= ~RCC_CFGR_HPRE;
    //RCC->CFGR |= (0U << RCC_CFGR_HPRE_Pos);
    set_ahb_presc(1);

    update_clocks();
}

LOCAL void set_sysclk(Sysclk_Source sysclk_src) {
    /* Set the system clock as required */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | (sysclk_src << RCC_CFGR_SW_Pos);

    /* ensure it has taken effect */
    while(RCC->CFGR & RCC_CFGR_SWS_Msk != sysclk_src << RCC_CFGR_SWS_Pos);
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
    // TODO: force reset or something if sysclk is 0.
    switch(READ_REG_BITS(RCC->CFGR, RCC_CFGR_SWS)) {
	case 1:		/* HSI16 - 16 MHz */
	    sysclk = __MHz(16);
	    break;

	case 2:		/* HSE - 24 MHz */
	    sysclk = __MHz(24);
	    break;

	case 3:		/* PLLR - Various */
	    sysclk = get_pllr_speed();
	    break;

	default:
	    break;
    }
}

LOCAL void update_hclk() {
    // TODO: force reset or something if hclk is 0.
    uint8_t temp = READ_REG_BITS(RCC->CFGR, RCC_CFGR_HPRE);
    switch(temp) {
	case 0:
	    hclk = sysclk;
	    break;

	case 8:
	case 9:
	case 10:
	case 11:
	    hclk = sysclk / (2 << (temp - 7));
	    break;

	case 12:
	case 13:
	case 14:
	case 15:
	    hclk = sysclk / (2 << (temp - 6));
	    break;

	default:
	    hclk = 0;
	    break;
    }
}

LOCAL void update_pclk1() {
    // TODO: force reset or something if pclk1 is 0.
    uint8_t temp = READ_REG_BITS(RCC->CFGR, RCC_CFGR_PPRE1);
    switch(temp) {
	case 0:
	    pclk1 = hclk;
	    break;

	case 4:
	case 5:
	case 6:
	case 7:
	    pclk1 = hclk / (2 << (temp - 3));
	    break;

	default:
	    pclk1 = 0;
	    break;
    }
}

LOCAL void update_pclk2() {
    // TODO: force reset or something if pclk2 is 0.
    uint8_t temp = READ_REG_BITS(RCC->CFGR, RCC_CFGR_PPRE2);
    switch(temp) {
	case 0:
	    pclk2 = hclk;
	    break;

	case 4:
	case 5:
	case 6:
	case 7:
	    pclk2 = hclk / (2 << (temp - 3));
	    break;

	default:
	    pclk2 = 0;
	    break;
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
	case 2:		/* HSI16 - 16 MHz */
	    return __MHz(16);

	case 3:		/* HSE - 24 MHz */
	    return __MHz(24);

	default:
	    return 0;
    }
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

LOCAL void set_pllm(uint32_t m) {
    /* value to be set is 1 less than m */
    m--;

    /* m should only be 4 bits */
    m &= 0xF;

    /* clear PLLM */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;

    /* set PLLM */
    RCC->PLLCFGR |= m << RCC_PLLCFGR_PLLM_Pos;
}

LOCAL void set_plln(uint32_t n) {
    /* n should only be 7 bits */
    n &= 0x7F;
    
    /* clear PLLN */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;

    /* set PLLN */
    RCC->PLLCFGR |= n << RCC_PLLCFGR_PLLN_Pos;
}

LOCAL void set_pllr(uint32_t r) {
    /* value to be set is 1 less than half of r */
    r = r/2 - 1;

    /* r should only be 2 bits */
    r &= 0x3;

    /* clear PLLR */
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;

    /* set PLLR */
    RCC->PLLCFGR |= r << RCC_PLLCFGR_PLLR_Pos;
}

LOCAL void set_ahb_presc(const uint16_t ahb_presc) {

    uint32_t hpre;
    
    /* clear HPRE bits */
    RCC->CFGR &= ~RCC_CFGR_HPRE;

    /* determine HPRE bits */
    switch(ahb_presc) {
	case 1U:
	    hpre = 0x0U << RCC_CFGR_HPRE_Pos;
	    break;

	case 2U:
	    hpre = 0x8U << RCC_CFGR_HPRE_Pos;
	    break;

	case 4U:
	    hpre = 0x9U << RCC_CFGR_HPRE_Pos;
	    break;

	case 8U:
	    hpre = 0xAU << RCC_CFGR_HPRE_Pos;
	    break;

	case 16U:
	    hpre = 0xBU << RCC_CFGR_HPRE_Pos;
	    break;

	case 64U:
	    hpre = 0xCU << RCC_CFGR_HPRE_Pos;
	    break;

	case 128U:
	    hpre = 0xDU << RCC_CFGR_HPRE_Pos;
	    break;

	case 256U:
	    hpre = 0xEU << RCC_CFGR_HPRE_Pos;
	    break;

	case 512U:
	    hpre = 0xFU << RCC_CFGR_HPRE_Pos;
	    break;

	default:
	    /* TODO: system reset or somethng? */
	    hpre = 0;
	    break;
    }

    /* write HPRE bits */
    RCC->CFGR |= hpre;

    /* ensure it has taken */
    while((RCC->CFGR & hpre) != hpre);
}

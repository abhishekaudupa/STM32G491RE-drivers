#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "config.h"

#define __MHz(n)	(n * 1000000U)
#define __kHz(n)	(n * 1000U)

/* 
 * PLL output speeds supported.
 * only those clock speeds divisible by 512. 
 * cuz, max AHB prescaling value is 512 and 
 * I'd like to have a nice integer value when 
 * the system clock speed is divided by any 
 * possible prescaler number <= 512, including 
 * prescalers for AHB and APBs.
 *
 * The format is:
 * PLL Input Source, M, N, R, Target AHB Speed in Hz.
 * Refer datasheet to know what M, N and R are.
 */
#define PLL_HSI_16	PLLClk_HSI, 2U,   8U, 4U,  16000000U
#define PLL_HSI_24	PLLClk_HSI, 2U,  12U, 4U,  24000000U
#define PLL_HSI_32	PLLClk_HSI, 2U,   8U, 2U,  32000000U
#define PLL_HSI_40	PLLClk_HSI, 2U,  10U, 2U,  40000000U
#define PLL_HSI_48	PLLClk_HSI, 2U,  12U, 2U,  48000000U
#define PLL_HSI_56	PLLClk_HSI, 2U,  14U, 2U,  56000000U
#define PLL_HSI_64	PLLClk_HSI, 2U,  16U, 2U,  64000000U
#define PLL_HSI_72	PLLClk_HSI, 2U,  18U, 2U,  72000000U
#define PLL_HSI_80	PLLClk_HSI, 2U,  20U, 2U,  80000000U
#define PLL_HSI_88	PLLClk_HSI, 2U,  22U, 2U,  88000000U
#define PLL_HSI_96	PLLClk_HSI, 2U,  24U, 2U,  96000000U
#define PLL_HSI_104	PLLClk_HSI, 2U,  26U, 2U, 104000000U
#define PLL_HSI_112	PLLClk_HSI, 2U,  28U, 2U, 112000000U
#define PLL_HSI_120	PLLClk_HSI, 2U,  30U, 2U, 120000000U
#define PLL_HSI_128	PLLClk_HSI, 2U,  32U, 2U, 128000000U
#define PLL_HSI_129	PLLClk_HSI, 5U,  81U, 2U, 129600000U
#define PLL_HSI_131	PLLClk_HSI, 5U,  82U, 2U, 131200000U
#define PLL_HSI_136	PLLClk_HSI, 2U,  34U, 2U, 136000000U
#define PLL_HSI_137	PLLClk_HSI, 5U,  86U, 2U, 137600000U
#define PLL_HSI_139	PLLClk_HSI, 5U,  87U, 2U, 139200000U
#define PLL_HSI_144	PLLClk_HSI, 2U,  36U, 2U, 144000000U
#define PLL_HSI_145	PLLClk_HSI, 5U,  91U, 2U, 145600000U
#define PLL_HSI_147	PLLClk_HSI, 5U,  92U, 2U, 147200000U
#define PLL_HSI_152	PLLClk_HSI, 2U,  38U, 2U, 152000000U
#define PLL_HSI_153	PLLClk_HSI, 5U,  96U, 2U, 153600000U
#define PLL_HSI_155	PLLClk_HSI, 5U,  97U, 2U, 155200000U
#define PLL_HSI_160	PLLClk_HSI, 2U,  40U, 2U, 160000000U
#define PLL_HSI_161	PLLClk_HSI, 5U, 101U, 2U, 161600000U
#define PLL_HSI_163	PLLClk_HSI, 5U, 102U, 2U, 163200000U
#define PLL_HSI_168	PLLClk_HSI, 2U,  42U, 2U, 168000000U
#define PLL_HSI_169	PLLClk_HSI, 5U, 106U, 2U, 169600000U
#define PLL_HSE24_16	PLLClk_HSE, 2U,   8U, 6U,  16000000U
#define PLL_HSE24_24	PLLClk_HSE, 2U,   8U, 4U,  24000000U
#define PLL_HSE24_32	PLLClk_HSE, 2U,  16U, 6U,  32000000U
#define PLL_HSE24_40	PLLClk_HSE, 2U,  20U, 6U,  40000000U
#define PLL_HSE24_48	PLLClk_HSE, 2U,   8U, 2U,  48000000U
#define PLL_HSE24_56	PLLClk_HSE, 2U,  28U, 6U,  56000000U
#define PLL_HSE24_64	PLLClk_HSE, 2U,  32U, 6U,  64000000U
#define PLL_HSE24_72	PLLClk_HSE, 2U,  12U, 2U,  72000000U
#define PLL_HSE24_80	PLLClk_HSE, 2U,  40U, 6U,  80000000U
#define PLL_HSE24_88	PLLClk_HSE, 2U,  44U, 6U,  88000000U
#define PLL_HSE24_96	PLLClk_HSE, 2U,  16U, 2U,  96000000U
#define PLL_HSE24_104	PLLClk_HSE, 2U,  52U, 6U, 104000000U
#define PLL_HSE24_112	PLLClk_HSE, 2U,  56U, 6U, 112000000U
#define PLL_HSE24_120	PLLClk_HSE, 2U,  20U, 2U, 120000000U
#define PLL_HSE24_128	PLLClk_HSE, 2U,  64U, 6U, 128000000U
#define PLL_HSE24_136	PLLClk_HSE, 2U,  68U, 6U, 136000000U
#define PLL_HSE24_144	PLLClk_HSE, 2U,  24U, 2U, 144000000U
#define PLL_HSE24_152	PLLClk_HSE, 2U,  76U, 6U, 152000000U
#define PLL_HSE24_160	PLLClk_HSE, 2U,  80U, 6U, 160000000U
#define PLL_HSE24_168	PLLClk_HSE, 2U,  28U, 2U, 168000000U

/* HPRE divisors */
#define HPRE_1		0x0U
#define HPRE_2		0x8U
#define HPRE_4		0x9U
#define HPRE_8		0xAU
#define HPRE_16		0xBU
#define HPRE_64		0xCU
#define HPRE_128	0xDU
#define HPRE_256	0xEU
#define HPRE_512	0xFU

/* PCLK divisors */
#define PPRE_1		0x0U
#define PPRE_2		0x4U
#define PPRE_4		0x5U
#define PPRE_8		0x6U
#define PPRE_16		0x7U

typedef enum {
    SysClk_HSI16	= 1U,
    SysClk_HSE		= 2U,
    SysClk_PLL		= 3U
} SysClk_Source;

typedef enum {
    PLLClk_HSI,
    PLLClk_HSE
} PLLClk_Source;

/* internal helper functions */
LOCAL void 		set_sysclk(SysClk_Source sysclk_src);
LOCAL void 		hse_on();
LOCAL void 		hsi_on();
LOCAL void 		enable_pll();
LOCAL void 		disable_pll();
LOCAL uint32_t 		get_pllr_speed();
LOCAL uint32_t 		get_pll_src_speed();
LOCAL void 		update_sysclk();
LOCAL void 		update_hclk();
LOCAL void 		update_pclk1();
LOCAL void 		update_pclk2();
LOCAL void 		update_pclk(uint32_t *const pclk, const uint8_t ppre);
LOCAL uint32_t 		get_pllm();
LOCAL uint32_t 		get_plln();
LOCAL uint32_t 		get_pllr();
LOCAL void 		set_pllm();
LOCAL void 		set_plln();
LOCAL void 		set_pllr();
LOCAL void 		set_ahb_presc(const uint16_t ahb_presc);
LOCAL void 		configure_pll(PLLClk_Source pll_src, const uint32_t m, const uint32_t n, const uint32_t r);
LOCAL void 		transition(const uint32_t target_ahb_speed, SysClk_Source sysclk_src);
LOCAL void 		transition_internal(SysClk_Source sysclk_src);
LOCAL SysClk_Source	get_sysclk_source();

/* public interface */
uint32_t 		get_sysclk_speed();
uint32_t 		get_hclk_speed();
uint32_t 		get_pclk1_speed();
uint32_t 		get_pclk2_speed();
void 			update_clocks();
void 			set_sysclk_hsi();
void 			set_sysclk_hse();
void 			set_sysclk_pll(PLLClk_Source pll_src, const uint32_t m, const uint32_t n, const uint32_t r, const uint32_t target_ahb_speed);

#endif

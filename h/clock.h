#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "config.h"

#define __MHz(n)	(n * 1000000U)
#define __kHz(n)	(n * 1000U)

/* 
 * PLL output speeds supported are those clock speeds divisible by 512. 
 * Max AHB prescaling value is 512 and I'd like to have a nice integer 
 * value when the system clock speed is divided by any possible prescaler 
 * number <= 512, including prescalers for AHB and APBs, all of which 
 * are powers of 2. 
 *
 * These #defines act as input to the set_sysclk_pll() functions and 
 * consequently to the configure_pll() function. Do not call 
 * set_sysclk_pll() using your own arguments.
 */
#define PLL_HSI_24	PLLClk_HSI, PLLM_2, PLLN_12,  PLLR_4,  24000000U
#define PLL_HSI_32	PLLClk_HSI, PLLM_2, PLLN_8,   PLLR_2,  32000000U
#define PLL_HSI_40	PLLClk_HSI, PLLM_2, PLLN_10,  PLLR_2,  40000000U
#define PLL_HSI_48	PLLClk_HSI, PLLM_2, PLLN_12,  PLLR_2,  48000000U
#define PLL_HSI_56	PLLClk_HSI, PLLM_2, PLLN_14,  PLLR_2,  56000000U
#define PLL_HSI_64	PLLClk_HSI, PLLM_2, PLLN_16,  PLLR_2,  64000000U
#define PLL_HSI_72	PLLClk_HSI, PLLM_2, PLLN_18,  PLLR_2,  72000000U
#define PLL_HSI_80	PLLClk_HSI, PLLM_2, PLLN_20,  PLLR_2,  80000000U
#define PLL_HSI_88	PLLClk_HSI, PLLM_2, PLLN_22,  PLLR_2,  88000000U
#define PLL_HSI_96	PLLClk_HSI, PLLM_2, PLLN_24,  PLLR_2,  96000000U
#define PLL_HSI_104	PLLClk_HSI, PLLM_2, PLLN_26,  PLLR_2, 104000000U
#define PLL_HSI_112	PLLClk_HSI, PLLM_2, PLLN_28,  PLLR_2, 112000000U
#define PLL_HSI_120	PLLClk_HSI, PLLM_2, PLLN_30,  PLLR_2, 120000000U
#define PLL_HSI_128	PLLClk_HSI, PLLM_2, PLLN_32,  PLLR_2, 128000000U
#define PLL_HSI_129	PLLClk_HSI, PLLM_5, PLLN_81,  PLLR_2, 129600000U
#define PLL_HSI_131	PLLClk_HSI, PLLM_5, PLLN_82,  PLLR_2, 131200000U
#define PLL_HSI_136	PLLClk_HSI, PLLM_2, PLLN_34,  PLLR_2, 136000000U
#define PLL_HSI_137	PLLClk_HSI, PLLM_5, PLLN_86,  PLLR_2, 137600000U
#define PLL_HSI_139	PLLClk_HSI, PLLM_5, PLLN_87,  PLLR_2, 139200000U
#define PLL_HSI_144	PLLClk_HSI, PLLM_2, PLLN_36,  PLLR_2, 144000000U
#define PLL_HSI_145	PLLClk_HSI, PLLM_5, PLLN_91,  PLLR_2, 145600000U
#define PLL_HSI_147	PLLClk_HSI, PLLM_5, PLLN_92,  PLLR_2, 147200000U
#define PLL_HSI_152	PLLClk_HSI, PLLM_2, PLLN_38,  PLLR_2, 152000000U
#define PLL_HSI_153	PLLClk_HSI, PLLM_5, PLLN_96,  PLLR_2, 153600000U
#define PLL_HSI_155	PLLClk_HSI, PLLM_5, PLLN_97,  PLLR_2, 155200000U
#define PLL_HSI_160	PLLClk_HSI, PLLM_2, PLLN_40,  PLLR_2, 160000000U
#define PLL_HSI_161	PLLClk_HSI, PLLM_5, PLLN_101, PLLR_2, 161600000U
#define PLL_HSI_163	PLLClk_HSI, PLLM_5, PLLN_102, PLLR_2, 163200000U
#define PLL_HSI_168	PLLClk_HSI, PLLM_2, PLLN_42,  PLLR_2, 168000000U
#define PLL_HSI_169	PLLClk_HSI, PLLM_5, PLLN_106, PLLR_2, 169600000U

#define PLL_HSE_16	PLLClk_HSE, PLLM_2, PLLN_8,   PLLR_6,  16000000U
#define PLL_HSE_32	PLLClk_HSE, PLLM_2, PLLN_16,  PLLR_6,  32000000U
#define PLL_HSE_40	PLLClk_HSE, PLLM_2, PLLN_20,  PLLR_6,  40000000U
#define PLL_HSE_48	PLLClk_HSE, PLLM_2, PLLN_8,   PLLR_2,  48000000U
#define PLL_HSE_56	PLLClk_HSE, PLLM_2, PLLN_28,  PLLR_6,  56000000U
#define PLL_HSE_64	PLLClk_HSE, PLLM_2, PLLN_32,  PLLR_6,  64000000U
#define PLL_HSE_72	PLLClk_HSE, PLLM_2, PLLN_12,  PLLR_2,  72000000U
#define PLL_HSE_80	PLLClk_HSE, PLLM_2, PLLN_40,  PLLR_6,  80000000U
#define PLL_HSE_88	PLLClk_HSE, PLLM_2, PLLN_44,  PLLR_6,  88000000U
#define PLL_HSE_96	PLLClk_HSE, PLLM_2, PLLN_16,  PLLR_2,  96000000U
#define PLL_HSE_104	PLLClk_HSE, PLLM_2, PLLN_52,  PLLR_6, 104000000U
#define PLL_HSE_112	PLLClk_HSE, PLLM_2, PLLN_56,  PLLR_6, 112000000U
#define PLL_HSE_120	PLLClk_HSE, PLLM_2, PLLN_20,  PLLR_2, 120000000U
#define PLL_HSE_128	PLLClk_HSE, PLLM_2, PLLN_64,  PLLR_6, 128000000U
#define PLL_HSE_136	PLLClk_HSE, PLLM_2, PLLN_68,  PLLR_6, 136000000U
#define PLL_HSE_144	PLLClk_HSE, PLLM_2, PLLN_24,  PLLR_2, 144000000U
#define PLL_HSE_152	PLLClk_HSE, PLLM_2, PLLN_76,  PLLR_6, 152000000U
#define PLL_HSE_160	PLLClk_HSE, PLLM_2, PLLN_80,  PLLR_6, 160000000U
#define PLL_HSE_168	PLLClk_HSE, PLLM_2, PLLN_28,  PLLR_2, 168000000U

/* PLL M divisors */
#define PLLM_1		0x0U
#define PLLM_2		0x1U
#define PLLM_3		0x2U
#define PLLM_4		0x3U
#define PLLM_5		0x4U
#define PLLM_6		0x5U
#define PLLM_7		0x6U
#define PLLM_8		0x7U
#define PLLM_9		0x8U
#define PLLM_10		0x9U
#define PLLM_11		0xAU
#define PLLM_12		0xBU
#define PLLM_13		0xCU
#define PLLM_14		0xDU
#define PLLM_15		0xEU
#define PLLM_16		0xFU

/* PLL N multipliers */
#define PLLN_8		0x08U
#define PLLN_9		0x09U
#define PLLN_10		0x0AU
#define PLLN_11		0x0BU
#define PLLN_12		0x0CU
#define PLLN_13		0x0DU
#define PLLN_14		0x0EU
#define PLLN_15		0x0FU
#define PLLN_16		0x10U
#define PLLN_17		0x11U
#define PLLN_18		0x12U
#define PLLN_19		0x13U
#define PLLN_20		0x14U
#define PLLN_21		0x15U
#define PLLN_22		0x16U
#define PLLN_23		0x17U
#define PLLN_24		0x18U
#define PLLN_25		0x19U
#define PLLN_26		0x1AU
#define PLLN_27		0x1BU
#define PLLN_28		0x1CU
#define PLLN_29		0x1DU
#define PLLN_30		0x1EU
#define PLLN_31		0x1FU
#define PLLN_32		0x20U
#define PLLN_33		0x21U
#define PLLN_34		0x22U
#define PLLN_35		0x23U
#define PLLN_36		0x24U
#define PLLN_37		0x25U
#define PLLN_38		0x26U
#define PLLN_39		0x27U
#define PLLN_40		0x28U
#define PLLN_41		0x29U
#define PLLN_42		0x2AU
#define PLLN_43		0x2BU
#define PLLN_44		0x2CU
#define PLLN_45		0x2DU
#define PLLN_46		0x2EU
#define PLLN_47		0x2FU
#define PLLN_48		0x30U
#define PLLN_49		0x31U
#define PLLN_50		0x32U
#define PLLN_51		0x33U
#define PLLN_52		0x34U
#define PLLN_53		0x35U
#define PLLN_54		0x36U
#define PLLN_55		0x37U
#define PLLN_56		0x38U
#define PLLN_57		0x39U
#define PLLN_58		0x3AU
#define PLLN_59		0x3BU
#define PLLN_60		0x3CU
#define PLLN_61		0x3DU
#define PLLN_62		0x3EU
#define PLLN_63		0x3FU
#define PLLN_64		0x40U
#define PLLN_65		0x41U
#define PLLN_66		0x42U
#define PLLN_67		0x43U
#define PLLN_68		0x44U
#define PLLN_69		0x45U
#define PLLN_70		0x46U
#define PLLN_71		0x47U
#define PLLN_72		0x48U
#define PLLN_73		0x49U
#define PLLN_74		0x4AU
#define PLLN_75		0x4BU
#define PLLN_76		0x4CU
#define PLLN_77		0x4DU
#define PLLN_78		0x4EU
#define PLLN_79		0x4FU
#define PLLN_80		0x50U
#define PLLN_81		0x51U
#define PLLN_82		0x52U
#define PLLN_83		0x53U
#define PLLN_84		0x54U
#define PLLN_85		0x55U
#define PLLN_86		0x56U
#define PLLN_87		0x57U
#define PLLN_88		0x58U
#define PLLN_89		0x59U
#define PLLN_90		0x5AU
#define PLLN_91		0x5BU
#define PLLN_92		0x5CU
#define PLLN_93		0x5DU
#define PLLN_94		0x5EU
#define PLLN_95		0x5FU
#define PLLN_96		0x60U
#define PLLN_97		0x61U
#define PLLN_98		0x62U
#define PLLN_99		0x63U
#define PLLN_100	0x64U
#define PLLN_101	0x65U
#define PLLN_102	0x66U
#define PLLN_103	0x67U
#define PLLN_104	0x68U
#define PLLN_105	0x69U
#define PLLN_106	0x6AU
#define PLLN_107	0x6BU
#define PLLN_108	0x6CU
#define PLLN_109	0x6DU
#define PLLN_110	0x6EU
#define PLLN_111	0x6FU
#define PLLN_112	0x70U
#define PLLN_113	0x71U
#define PLLN_114	0x72U
#define PLLN_115	0x73U
#define PLLN_116	0x74U
#define PLLN_117	0x75U
#define PLLN_118	0x76U
#define PLLN_119	0x77U
#define PLLN_120	0x78U
#define PLLN_121	0x79U
#define PLLN_122	0x7AU
#define PLLN_123	0x7BU
#define PLLN_124	0x7CU
#define PLLN_125	0x7DU
#define PLLN_126	0x7EU
#define PLLN_127	0x7FU

/* PLL R divisors */
#define PLLR_2		0x0U
#define PLLR_4		0x1U
#define PLLR_6		0x2U
#define PLLR_8		0x3U

/* HCLK divisors */
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

/* public interface */
void 			update_clocks();
uint32_t 		get_sysclk_speed();
uint32_t 		get_hclk_speed();
uint32_t 		get_pclk1_speed();
uint32_t 		get_pclk2_speed();
void 			set_sysclk_hsi();
void 			set_sysclk_hse();
void 			set_sysclk_pll(PLLClk_Source pll_src, const uint32_t m, const uint32_t n, const uint32_t r, const uint32_t target_ahb_speed);

#endif

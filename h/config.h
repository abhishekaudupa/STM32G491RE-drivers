#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32g491xx.h"
#include "nucleoG491RE.h"
#include <stdint.h>
#include <stddef.h>

#define LOCAL static inline

#define READ_REG_BITS(REG, BITS)	((REG & BITS) >> BITS##_Pos)

#define __WAIT_2_CLOCKS__		__attribute__((unused))volatile uint32_t dummy = RCC->AHB1ENR; \
											 dummy = RCC->AHB1ENR

#endif

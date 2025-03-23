#include "stm32g491xx.h"
#include "config.h"
#include "power.h"

uint32_t get_vos() {
    return READ_REG_BITS(PWR->CR1, PWR_CR1_VOS);
}

uint32_t get_r1_mode() {
    return READ_REG_BITS(PWR->CR5, PWR_CR5_R1MODE);
}

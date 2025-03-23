#include "stm32g491xx.h"
#include "delay.h"
#include "clock.h"

void delay(const uint32_t ms) {
    uint32_t now = sysTick;
    while(now + ms > sysTick);
}

void enable_systick() {

    /* enable global interrupt */
    __enable_irq();

    /* clock source is AHB clock, interrupt on, switch on systick */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    /* load value for 1ms ticks */
    SysTick->LOAD = get_hclk_speed() / 1000U;
}

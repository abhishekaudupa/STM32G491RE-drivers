#include "systick.h"
#include "clock.h"

volatile uint32_t sysTick;

void systick_handler() {
    sysTick++;
}

void enable_systick() {

    /* enable global interrupt */
    __enable_irq();

    /* clock source is AHB clock, interrupt on, switch on systick */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    /* load value for 1ms ticks */
    SysTick->LOAD = get_hclk_speed() / 1000U;
}

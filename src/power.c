#include "stm32g491xx.h"
#include "config.h"
#include "power.h"

uint32_t get_vos() {

    enable_power_interface();

    /* acquire VOS 
     * 1 - Range 1
     * 2 - Range 2
     */
    uint32_t vos = READ_REG_BITS(PWR->CR1, PWR_CR1_VOS);

    disable_power_interface();

    return vos;
}

uint32_t get_r1_mode() {

    enable_power_interface();

    /* acquire R1 mode
     * 0 - Boost Mode
     * 1 - Normal Mode
     */
    uint32_t r1_mode = READ_REG_BITS(PWR->CR5, PWR_CR5_R1MODE);

    disable_power_interface();

    return r1_mode;
}

LOCAL void enable_power_interface() {
    /* enable power interface clock on APB1 */
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
}

LOCAL void disable_power_interface() {
    /* disable power interface clock on APB1 */
    RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;
}

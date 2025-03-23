#include "stm32g491xx.h"
#include "flash.h"

void set_flash_read_latency(uint32_t wait_states) {

    enable_flash_interface_registers();

    /* clear flash latency bits */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;

    /* bits other than LATENCY should be cleared */
    wait_states &= FLASH_ACR_LATENCY;

    /* set flash latency bits */
    FLASH->ACR |= wait_states;

    /* ensure it has taken effect */
    while((FLASH->ACR & wait_states) != wait_states);
    
    disable_flash_interface_registers();
}

LOCAL void enable_flash_interface_registers() {
    /* set flash enable bit */
    RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;
}

LOCAL void disable_flash_interface_registers() {
    /* clear flash enable bit */
    RCC->AHB1ENR &= ~RCC_AHB1ENR_FLASHEN;
}

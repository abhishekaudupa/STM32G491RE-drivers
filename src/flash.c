#include "stm32g491xx.h"
#include "flash.h"
#include "power.h"
#include "clock.h"

void update_flash_read_latency(const uint32_t ahb_speed) {
    switch(get_vos()) {
	case 1:		/* Range 1 */
	    update_flash_read_latency_r1(ahb_speed);
	    break;

	case 2:		/* Range 2*/
	    if(ahb_speed <= __MHz(12)) {
		set_flash_read_latency(0);
		break;
	    }

	    if(ahb_speed <= __MHz(24)) {
		set_flash_read_latency(1);
		break;
	    }

	    if(ahb_speed <= __MHz(26)) {
		set_flash_read_latency(2);
		break;
	    }
    }
}

LOCAL void update_flash_read_latency_r1(const uint32_t ahb_speed) {
    uint32_t ahb_ref;

    /* Refer RM0440 Rev 8 Table 19 */
    switch(get_r1_mode()) {
	case 0:		/* Boost Mode */
	    ahb_ref = __MHz(34);
	    break;

	case 1:		/* Normal Mode */
	    ahb_ref = __MHz(30);
	    break;
    }

    for(uint32_t i = 1; i <= 5; ++i)
	if(ahb_speed <= (ahb_ref * i)) {
	    set_flash_read_latency(i - 1);
	    break;
	}
}

LOCAL void set_flash_read_latency(uint32_t wait_states) {

    enable_flash_interface();

    /* clear flash latency bits */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;

    /* bits other than LATENCY should be cleared */
    wait_states &= FLASH_ACR_LATENCY;

    /* set flash latency bits */
    FLASH->ACR |= wait_states;

    /* ensure it has taken effect */
    while((FLASH->ACR & wait_states) != wait_states);

    disable_flash_interface();
}

LOCAL void enable_flash_interface() {
    /* set flash enable bit */
    RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;
}

LOCAL void disable_flash_interface() {
    /* clear flash enable bit */
    RCC->AHB1ENR &= ~RCC_AHB1ENR_FLASHEN;
}

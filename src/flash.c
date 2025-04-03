#include "flash.h"
#include "clock.h"

/* internal helper functions */
LOCAL void 	enable_flash_interface();
LOCAL void	disable_flash_interface();
LOCAL void 	update_flash_read_latency_r1(const uint32_t target_ahb_speed, VCore_Power_Mode r1_mode);
LOCAL void 	update_flash_read_latency_r2(const uint32_t target_ahb_speed);
LOCAL void 	set_flash_read_latency(const uint32_t wait_states);

void update_flash_read_latency(const uint32_t target_ahb_speed) {

    /* Refer RM0440 Rev 8 Table 19 */
    switch(get_vcore_power_mode()) {
	case VCore_Range_1_Normal:
	    update_flash_read_latency_r1(target_ahb_speed, VCore_Range_1_Normal);
	    return;

	case VCore_Range_1_Boost:
	    update_flash_read_latency_r1(target_ahb_speed, VCore_Range_1_Boost);
	    return;

	case VCore_Range_2:
	    update_flash_read_latency_r2(target_ahb_speed);
	    return;
    }
}

LOCAL void update_flash_read_latency_r1(const uint32_t target_ahb_speed, VCore_Power_Mode r1_mode) {
    uint32_t ahb_ref;

    /* Refer RM0440 Rev 8 Table 19 */
    switch(r1_mode) {
	case VCore_Range_1_Boost:
	    ahb_ref = __MHz(34);
	    break;

	case VCore_Range_1_Normal:
	    ahb_ref = __MHz(30);
	    break;

	case VCore_Range_2:
	    /* doesn't happen. Here just to avoid compiler warning */
	    break;
    }

    for(uint32_t i = 1; i <= 5; ++i)
	if(target_ahb_speed <= (ahb_ref * i)) {
	    set_flash_read_latency(i - 1);
	    return;
	}
}

LOCAL void update_flash_read_latency_r2(const uint32_t target_ahb_speed) {
    if(target_ahb_speed <= __MHz(12)) {
	set_flash_read_latency(FLASH_LATENCY_0);
	return;
    }

    if(target_ahb_speed <= __MHz(24)) {
	set_flash_read_latency(FLASH_LATENCY_1);
	return;
    }

    if(target_ahb_speed <= __MHz(26)) {
	set_flash_read_latency(FLASH_LATENCY_2);
	return;
    }
}

LOCAL void set_flash_read_latency(const uint32_t wait_states) {

    enable_flash_interface();

    /* clear flash latency bits */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;

    /* set flash latency bits */
    FLASH->ACR |= wait_states;

    /* ensure it has taken effect */
    while((FLASH->ACR & wait_states) != wait_states);

    disable_flash_interface();
}

LOCAL void enable_flash_interface() {
    /* set flash enable bit */
    RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;

    /* wait for 2 clock cycle */
    __attribute__((unused))volatile uint32_t dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;
}

LOCAL void disable_flash_interface() {
    /* 
     * clear flash enable bit. 
     * Can be disabled only when 
     * the flash is in power down
     * mode. 
     */
    RCC->AHB1ENR &= ~RCC_AHB1ENR_FLASHEN;
}

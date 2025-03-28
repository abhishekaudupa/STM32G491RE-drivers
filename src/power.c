#include "stm32g491xx.h"
#include "config.h"
#include "power.h"

VCore_Power_Mode get_vcore_power_mode() {
    VCore_Power_Mode mode;
    
    enable_power_interface();
    
    switch(READ_REG_BITS(PWR->CR1, PWR_CR1_VOS)) {
	case 1:		/* Range 1 */
	    mode = READ_REG_BITS(PWR->CR5, PWR_CR5_R1MODE)? VCore_Range_1_Normal: VCore_Range_1_Boost;
	    break;

	case 2:		/* Range 2 */
	    mode = VCore_Range_2;
	    break;
    }
    
    disable_power_interface(); 
    return mode;
}

LOCAL void enable_power_interface() {
    /* enable power interface clock on APB1 */
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
}

LOCAL void disable_power_interface() {
    /* disable power interface clock on APB1 */
    RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;
}

#include "stm32g491xx.h"

/* public interface */
void enable_user_led();
void disable_user_led();
void user_led_on();
void user_led_off();
void user_led_toggle();
void user_led_blink(const uint32_t ms);
void enable_user_button();
void disable_user_button();

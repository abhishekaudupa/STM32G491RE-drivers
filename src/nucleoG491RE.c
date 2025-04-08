#include "nucleoG491RE.h"
#include "delay.h"
#include "gpio.h"

void enable_user_led() {
    /* enable clock to GPIOA */
    enable_gpio_interface(GPIO_PA);

    /* set LED_PIN in output mode */
    set_gpio_mode(GPIO_PA, GPIO_Pin_5, GPIO_Mode_Output);    
}

void disable_user_led() {
    user_led_off();
    
    /* reset LED_PIN: analog mode */
    set_gpio_mode(GPIO_PA, GPIO_Pin_5, GPIO_Mode_Analog);    
}

void user_led_on() {
    GPIOA->ODR |= GPIO_ODR_OD5;
}

void user_led_off() {
    GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void user_led_toggle() {
    GPIOA->ODR ^= GPIO_ODR_OD5;
}

void user_led_blink(const uint32_t ms) {
    while(1) {
	user_led_toggle();
	delay(ms);
    }
}

void enable_user_button() {
    /* enable clock to GPIOC */
    enable_gpio_interface(GPIO_PC);

    /* set user button in input mode */
    set_gpio_mode(GPIO_PC, GPIO_Pin_13, GPIO_Mode_Input);    
}

uint8_t is_user_button_pressed() {
    return (GPIOC->IDR & GPIO_IDR_ID13)? 1U: 0U;
}

void disable_user_button() {
    /* reset user button: analog mode */
    set_gpio_mode(GPIO_PC, GPIO_Pin_13, GPIO_Mode_Analog);    
}

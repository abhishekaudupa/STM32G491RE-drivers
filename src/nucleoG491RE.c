#include "stm32g491xx.h"
#include "nucleoG491RE.h"
#include "delay.h"

void enable_user_led() {
    /* enable clock to GPIOA */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* set LED_PIN in output mode */
    GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;	
    GPIOA->MODER |= GPIO_MODER_MODE5_0;		
}

void disable_user_led() {
    user_led_off();

    /* disable clock to GPIOA */
    RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOAEN;
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
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    /* set user button in input mode */
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;
}

void disable_user_button() {
    /* disable clock to GPIOC */
    RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOCEN;
}

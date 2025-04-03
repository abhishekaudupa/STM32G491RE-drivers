#include "gpio.h"

LOCAL GPIO_TypeDef* get_gpio_base_address(GPIO_Port gpio_port);

void set_gpio_af(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_AF gpio_af) {
    uint8_t af_bit_pos = 0;
    uint8_t af_index = 0;

    /* acquire the GPIO base address */
    GPIO_TypeDef *GPIOx = get_gpio_base_address(gpio_port);

    /* get the AFR register index and the bit position */
    if(gpio_pin <= 7) {
	af_bit_pos = (gpio_pin << 2);
	af_index = 0;
    } else {
	af_bit_pos = ((gpio_pin - 8) << 2);
	af_index = 1;
    }

    /* clear AFSEL bits */
    GPIOx->AFR[af_index] &= ~(0xFU << af_bit_pos);

    /* set AFSEL bits according to input */
    GPIOx->AFR[af_index] |= (gpio_af << af_bit_pos);
}

void set_gpio_output_resistor(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Resistor gpio_res) {
    /* acquire the GPIO base address */
    GPIO_TypeDef *GPIOx = get_gpio_base_address(gpio_port);

    /* clear the resistor bits */
    GPIOx->PUPDR &= ~(0x3U << (gpio_pin << 1));

    /* set the appropriate resistor setting */
    GPIOx->PUPDR |= (gpio_res << (gpio_pin << 1));
}

void set_gpio_output_speed(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Speed gpio_speed) {
    /* acquire the GPIO base address */
    GPIO_TypeDef *GPIOx = get_gpio_base_address(gpio_port);

    /* clear the speed bits */
    GPIOx->OSPEEDR &= ~(0x3U << (gpio_pin << 1));

    /* set speed bits according to the input */
    GPIOx->OSPEEDR |= (gpio_speed << (gpio_pin << 1));
}

void set_gpio_output_mode(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Output_Mode output_mode) {
    /* acquire the GPIO base address */
    GPIO_TypeDef *GPIOx = get_gpio_base_address(gpio_port);

    /* set the mode on the appropriate pin */
    if(output_mode == GPIO_Output_Push_Pull)
	GPIOx->OTYPER &= ~(0x1U << gpio_pin);
    else
	GPIOx->OTYPER |= (0x1U << gpio_pin);
}

void set_gpio_mode(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Mode gpio_mode) {
    /* acquire the GPIO base address */
    GPIO_TypeDef *GPIOx = get_gpio_base_address(gpio_port);

    /* clear the mode bits */
    GPIOx->MODER &= ~(0x3U << (gpio_pin << 1));

    /* set bits according to selected mode */
    GPIOx->MODER |= (gpio_mode << (gpio_pin << 1));
}

void enable_gpio_interface(GPIO_Port gpio_port) {
    /* clock the appropriate GPIO registers on AHB2 */
    RCC->AHB2ENR |= (0x1U << (RCC_AHB2ENR_GPIOAEN_Pos + gpio_port));

    /* 
     * Wait for at least two clock cycles for the clock to take effect. 
     * If not, wait for a billion cycles debugging.
     */
    __attribute__((unused))volatile uint32_t dummy = RCC->AHB2ENR;
    dummy = RCC->AHB2ENR;
}

void disable_gpio_interface(GPIO_Port gpio_port) {
    /* unclock the appropriate GPIO registers on AHB2 */
    RCC->AHB2ENR &= ~(0x1U << (RCC_AHB2ENR_GPIOAEN_Pos + gpio_port));
}

LOCAL GPIO_TypeDef* get_gpio_base_address(GPIO_Port gpio_port) {
    return (GPIO_TypeDef*)(GPIOA_BASE + gpio_port * 0x400U);
}

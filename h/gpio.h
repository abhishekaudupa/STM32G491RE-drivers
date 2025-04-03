#ifndef __GPIO_H__
#define __GPIO_H__

#include "config.h"

/* values are the offset of the corresponding GPIOAEN bit from bit 0 */
typedef enum {
    GPIO_PA	= 0x0U,
    GPIO_PB	= 0x1U,
    GPIO_PC	= 0x2U,
    GPIO_PD	= 0x3U,
    GPIO_PE	= 0x4U,
    GPIO_PF	= 0x5U,
    GPIO_PG	= 0x6U
} GPIO_Port;

/* values are bit settings for corresponding modes */
typedef enum {
    GPIO_Mode_Input		= 0x0U,
    GPIO_Mode_Output		= 0x1U,
    GPIO_Mode_Alternate		= 0x2U,
    GPIO_Mode_Analog		= 0x3U
} GPIO_Mode;

/* pin bits from bit 0 */
typedef enum {
    GPIO_Pin_0	= 0x00U,
    GPIO_Pin_1	= 0x01U,
    GPIO_Pin_2	= 0x02U,
    GPIO_Pin_3	= 0x03U,
    GPIO_Pin_4	= 0x04U,
    GPIO_Pin_5	= 0x05U,
    GPIO_Pin_6	= 0x06U,
    GPIO_Pin_7	= 0x07U,
    GPIO_Pin_8	= 0x08U,
    GPIO_Pin_9	= 0x09U,
    GPIO_Pin_10	= 0x0AU,
    GPIO_Pin_11	= 0x0BU,
    GPIO_Pin_12	= 0x0CU,
    GPIO_Pin_13	= 0x0DU,
    GPIO_Pin_14	= 0x0EU,
    GPIO_Pin_15	= 0x0FU
} GPIO_Pin;

/* GPIO pin output modes */
typedef enum {
    GPIO_Output_Push_Pull,
    GPIO_Output_Open_Drain
} GPIO_Output_Mode;

/* values are bit settings for corresponding speeds */
typedef enum {
    GPIO_Speed_Low	= 0x0U,
    GPIO_Speed_Med	= 0x1U,
    GPIO_Speed_High	= 0x2U,
    GPIO_Speed_VHigh	= 0x3U
} GPIO_Speed;

/* values are bit settings for corresponding resistor */
typedef enum {
    GPIO_Res_None	= 0x0U,
    GPIO_Res_Pull_Up	= 0x1U,
    GPIO_Res_Pull_Down	= 0x2U
} GPIO_Resistor;

/* Alternate function bit pattern */
typedef enum {
    GPIO_AF0	= 0x0U,
    GPIO_AF1	= 0x1U,
    GPIO_AF2	= 0x2U,
    GPIO_AF3	= 0x3U,
    GPIO_AF4	= 0x4U,
    GPIO_AF5	= 0x5U,
    GPIO_AF6	= 0x6U,
    GPIO_AF7	= 0x7U,
    GPIO_AF8	= 0x8U,
    GPIO_AF9	= 0x9U,
    GPIO_AF10	= 0xAU,
    GPIO_AF11	= 0xBU,
    GPIO_AF12	= 0xCU,
    GPIO_AF13	= 0xDU,
    GPIO_AF14	= 0xEU,
    GPIO_AF15	= 0xFU
} GPIO_AF;

void set_gpio_af(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_AF gpio_af);
void set_gpio_output_resistor(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Resistor gpio_res);
void set_gpio_output_speed(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Speed gpio_speed);
void set_gpio_output_mode(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Output_Mode output_mode);
void set_gpio_mode(GPIO_Port gpio_port, GPIO_Pin gpio_pin, GPIO_Mode gpio_mode);
void enable_gpio_interface(GPIO_Port gpio_port);
void disable_gpio_interface(GPIO_Port gpio_port);

#endif

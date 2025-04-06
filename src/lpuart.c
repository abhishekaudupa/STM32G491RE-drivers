#include "lpuart.h"
#include "clock.h"
#include "gpio.h"

#define LPUART1_FIFO_MODE

#ifdef LPUART1_FIFO_MODE
#define LPUART1_TX_READY (LPUART1->ISR & USART_ISR_TXE_TXFNF)
#define LPUART1_RX_READY (LPUART1->ISR & USART_ISR_RXNE_RXFNE)
#else
#define LPUART1_TX_READY (LPUART1->ISR & USART_ISR_TXE)
#define LPUART1_RX_READY (LPUART1->ISR & USART_ISR_RXNE)
#endif

/* internal helper functions */
LOCAL uint32_t 	get_lpuart1_clock_speed();
LOCAL uint32_t 	get_lpuart1_presc();
LOCAL void 	set_lpuart1_presc(const uint32_t presc);
LOCAL void 	set_lpuart1_word_len(const uint8_t len);

#ifndef LPUART1_POLL_MODE
uint8_t internal_buf[256];
uint8_t insert_index, remove_index;

void lpuart1_IRQ_handler() {

    /* RDR not empty or RXFIFO not empty */
    if(LPUART1_RX_READY) {
	internal_buf[insert_index] = LPUART1->RDR;
	switch(internal_buf[insert_index]) {
	    case 13:	/* Line feed */
		internal_buf[insert_index++] = '\n';
		internal_buf[insert_index++] = '\r';
		break;
	    case 127:	/* Backspace */
		internal_buf[insert_index++] = '\b';
		internal_buf[insert_index++] = ' ';
		internal_buf[insert_index++] = '\b';
		break;
	    default:
		insert_index++;
		break;
	}

	/* enable TDR empty/TXFIFO not full interrupt */
	LPUART1->CR1 |= USART_CR1_TXEIE_TXFNFIE;
    }

    /* TDR empty or TXFIFO not full */
    if(LPUART1_TX_READY) {
	if(remove_index != insert_index)
	    LPUART1->TDR = internal_buf[remove_index++];
	else
	    LPUART1->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
    }
}
#endif

void init_lpuart1(const uint32_t baud_rate) {

    /* clock the lpuart1 */
    RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;

    /* wait for two clock cycles */
    __attribute__((unused))volatile uint32_t dummy = RCC->APB1ENR2;
    dummy = RCC->APB1ENR2;

    /* disable lpuart1 and its interrupts */
    LPUART1->CR1 &= ~USART_CR1_UE;

#ifndef LPUART1_POLL_MODE
    NVIC_DisableIRQ(LPUART1_IRQn);
#endif

    /* clock GPIOA */
    enable_gpio_interface(GPIO_PA);

    /* set PA2 as TX (AF12) */
    set_gpio_mode(GPIO_PA, GPIO_Pin_2, GPIO_Mode_Alternate);
    set_gpio_af(GPIO_PA, GPIO_Pin_2, GPIO_AF12);

    /* set PA3 as RX (AF12) */
    set_gpio_mode(GPIO_PA, GPIO_Pin_3, GPIO_Mode_Alternate);
    set_gpio_af(GPIO_PA, GPIO_Pin_3, GPIO_AF12);

    /* set lpuart1 baud rate */
    LPUART1->BRR = (256U * (get_lpuart1_clock_speed() / baud_rate)) & 0x000FFFFF;

#ifdef LPUART1_FIFO_MODE
    /* enable fifo mode */
    LPUART1->CR1 |= USART_CR1_FIFOEN;
#endif

#ifndef LPUART1_POLL_MODE
    /* buffer indices reset */
    insert_index = remove_index = 0;

    /* enable interrupts */
    __enable_irq();
    NVIC_EnableIRQ(LPUART1_IRQn);

    /* enable rxfifo not empty interrupt */
    LPUART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
#endif

    /* enable lpuart1 */
    LPUART1->CR1 |= USART_CR1_UE;

    /* enable transmitter and receiver */
    LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);
}

void tx_lpuart1(const char *data) {
    /* write data to TDR when it's ready */
    while(*data) 
	if(LPUART1_TX_READY)
	    LPUART1->TDR = *data++;

    /* wait for the transmission to be complete */
    while(!(LPUART1->ISR & USART_ISR_TC));
}

void tx_single_lpuart1(const char *const data) {
    /* write data to TDR when it's ready */
    if(LPUART1_TX_READY) {
	LPUART1->TDR = *data;

	/* wait for the transmission to be complete */
	while(!(LPUART1->ISR & USART_ISR_TC));
    }
}

#ifdef LPUART1_POLL_MODE
char* rx_poll_lpuart1(char *const buf) {
    /* if RDR or RXFIFO is empty, return NULL */
    if(!LPUART1_RX_READY)
	return NULL;

    /* return the buffer after filling it with data */
    *buf = LPUART1->RDR;
    return buf;
}
#endif

LOCAL uint32_t get_lpuart1_clock_speed() {
    uint32_t lpuart1_clk = 0;
    uint32_t presc = 0;
    switch((RCC->CCIPR & RCC_CCIPR_LPUART1SEL) >> RCC_CCIPR_LPUART1SEL_Pos) {

	case 0:		/* PCLK1 - Various */
	    lpuart1_clk = get_pclk1_speed();
	    break;

	case 1:		/* SYSCLK - Various */
	    lpuart1_clk = get_sysclk_speed();
	    break;

	case 2:		/* HSI16 - 16 MHz */
	    lpuart1_clk = 16000000;
	    break;

	case 3:		/* LSE - 32.768 kHz */
	    lpuart1_clk = 32768;
	    break;

	default:
	    /* TODO: do a system reset of something in this case? */
	    return 0;
    }

    presc = get_lpuart1_presc();
    return presc? (lpuart1_clk / presc): 0;
}

LOCAL uint32_t get_lpuart1_presc() {
    uint32_t presc = LPUART1->PRESC & USART_PRESC_PRESCALER;
    switch(presc) {
	case 0U:
	    return 1;

	case 1U:
	case 2U:
	case 3U:
	case 4U:
	case 5U:
	case 6U:
	    /* multiply by 2 */
	    return (presc << 1);

	case 7U:
	case 8U:
	case 9U:
	case 10U:
	case 11U:
	    return (1 << (presc - 3));

	default:
	    return 0;
    }
}

LOCAL void set_lpuart1_presc(const uint32_t presc) {
    switch(presc) {
	case 1U:
	    LPUART1->PRESC = 0U;
	    break;

	case 2U:
	case 4U:
	case 6U:
	case 8U:
	case 10U:
	case 12U:
	    /* divide by 2 */
	    LPUART1->PRESC = presc >> 1;
	    break;

	case 16U:
	    LPUART1->PRESC = 7U;
	    break;

	case 32U:
	    LPUART1->PRESC = 8U;
	    break;

	case 64U:
	    LPUART1->PRESC = 9U;
	    break;

	case 128U:
	    LPUART1->PRESC = 10U;
	    break;

	case 256U:
	    LPUART1->PRESC = 11U;
	    break;

	default:
	    LPUART1->PRESC = 11U;
	    break;
    }
}

LOCAL void set_lpuart1_word_len(const uint8_t len) {
    /* clear M bits */
    LPUART1->CR1 &= ~USART_CR1_M;

    switch(len) {
	case 7U:
	    /* M0 = 0, M1 = 1 i.e M = 2 */
	    LPUART1->CR1 |= 1 << USART_CR1_M1_Pos;
	    break;
	case 8U:
	    /* M0 = 0, M1 = 0 i.e M = 0 */
	    break;
	case 9U:
	    /* M0 = 1, M1 = 0 i.e M = 1 */
	    LPUART1->CR1 |= 1 << USART_CR1_M0_Pos;
	    break;

	default:
	    /* 8-bit word length - default */
	    break;
    }
}

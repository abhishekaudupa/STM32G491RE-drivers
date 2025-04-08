#include "spi.h"
#include "gpio.h"

void 		enable_spi_interface(SPI_Channel spi_chan);
void 		disable_spi_interface(SPI_Channel spi_chan);
SPI_TypeDef*	get_spi_baseaddr(SPI_Channel spi_chan);
void 		set_spi_mode(SPI_TypeDef *const SPIx, SPI_Mode spi_mode);
void 		set_spi_comm_mode(SPI_TypeDef *const SPIx, SPI_Comm_Mode spi_comm_mode);
void 		set_spi_datasize(SPI_TypeDef *const SPIx, SPI_Datasize spi_datasize);
void 		set_spi_baud_rate(SPI_TypeDef *const SPIx, SPI_Baudrate_Divisor spi_br_div);
void 		set_spi_clock_pol(SPI_TypeDef *const SPIx, SPI_Clock_Pol clk_pol);
void 		set_spi_clock_pha(SPI_TypeDef *const SPIx, SPI_Clock_Pha clk_pha);
void 		set_spi_frame_format(SPI_TypeDef *const SPIx, SPI_Frame_Format frm_fmt);
void 		set_spi_full_duplex(SPI_TypeDef *const SPIx);
void 		set_spi_half_duplex(SPI_TypeDef *const SPIx);
void 		set_spi_simplex_transmit(SPI_TypeDef *const SPIx);
void 		set_spi_simplex_receive(SPI_TypeDef *const SPIx);
void 		set_spi_single_master_mode(SPI_TypeDef *const SPIx);
void 		set_spi_multi_master_mode(SPI_TypeDef *const SPIx);
void 		set_spi_slave_mode(SPI_TypeDef *const SPIx);
void 		set_spi_gpio_af(SPI_Channel spi_chan);

volatile uint16_t *rbuf, *tbuf;
volatile uint16_t *rwi;
uint16_t tbuf_size, ti, dummy;

void spi1_IRQ_handler() {
    if(SPI1->SR & SPI_SR_TXE) {
	if(SPI1->CR2 & SPI_CR2_TXEIE) {
	    if(tbuf_size) {
		uint16_t val = tbuf[ti - tbuf_size];
		tbuf_size--;
		val |= tbuf[ti - tbuf_size] << 8;
		tbuf_size--;
		SPI1->DR = val;
	    } else {
		SPI1->CR2 &= ~SPI_CR2_TXEIE;
	    } 
	}	
    }

    if(SPI1->CR2 & SPI_CR2_RXNEIE) {
	if(SPI1->SR & SPI_SR_RXNE) {
	    rbuf[*rwi] = SPI1->DR;
	    (*rwi)++;
	}
    }
}

void spi2_IRQ_handler() {
}

void spi3_IRQ_handler() {
}

void init_spi(SPI_Channel spi_chan, 
	SPI_Mode spi_mode, 
	SPI_Clock_Pha spi_cpha, 
	SPI_Clock_Pol spi_cpol, 
	SPI_Comm_Mode spi_comm_mode, 
	SPI_Frame_Format spi_fmt, 
	SPI_Datasize spi_datasize) {

    /* acquire SPI register base address */
    SPI_TypeDef *SPIx = get_spi_baseaddr(spi_chan);

    /* clock the SPI */
    enable_spi_interface(spi_chan);

    /* multiplex the relevant GPIO pins to SPI functionality */
    set_spi_gpio_af(spi_chan);

    /* clock polarity setting */
    set_spi_clock_pol(SPIx, spi_cpol);

    /* clock phase setting */
    set_spi_clock_pha(SPIx, spi_cpha);

    /* full/half-duplex/simplex */
    set_spi_comm_mode(SPIx, spi_comm_mode);

    /* msb vs lsb first */
    set_spi_frame_format(SPIx, spi_fmt);

    /* master vs. slave mode.*/
    set_spi_mode(SPIx, spi_mode);

    /* transaction data length in bits */
    set_spi_datasize(SPIx, spi_datasize);
}

void spi_start(SPI_Channel spi_chan) {

    /* acquire SPI register base address */
    SPI_TypeDef *SPIx = get_spi_baseaddr(spi_chan);

    /* enable interrupts */
    __enable_irq();
    NVIC_EnableIRQ(SPI1_IRQn);
    SPIx->CR2 |= SPI_CR2_RXNEIE;
    SPIx->CR2 |= SPI_CR2_TXEIE;

    /* fire-up the SPI */
    SPIx->CR1 |= SPI_CR1_SPE;
}

void spi_config_rxbuffer(volatile void *rxbuf, volatile uint16_t *rxbuf_windex) {
    rbuf = rxbuf;
    rwi = rxbuf_windex;
}

void spi_config_txbuffer(volatile void *txbuf, const uint16_t txbuf_size) {
    tbuf = txbuf;
    tbuf_size = ti = txbuf_size;
}

SPI_TypeDef* get_spi_baseaddr(SPI_Channel spi_chan) {
    switch(spi_chan) {
	case SPI_Channel_1:
	    return SPI1;

	case SPI_Channel_2:
	    return SPI2;

	case SPI_Channel_3:
	    return SPI3;
    }
    return NULL;
}

void set_spi_mode(SPI_TypeDef *const SPIx, SPI_Mode spi_mode) {
    switch(spi_mode) {
	case SPI_Single_Control:
	    set_spi_single_master_mode(SPIx);
	    return;

	case SPI_Multi_Control:
	    set_spi_multi_master_mode(SPIx);
	    return;

	case SPI_Peripheral:
	    set_spi_slave_mode(SPIx);
	    return;
    }
}

void set_spi_comm_mode(SPI_TypeDef *const SPIx, SPI_Comm_Mode spi_comm_mode) {
    switch(spi_comm_mode) {
	case SPI_Comm_Full_Duplex:
	    set_spi_full_duplex(SPIx);
	    return;

	case SPI_Comm_Half_Duplex:
	    set_spi_half_duplex(SPIx);
	    return;

	case SPI_Comm_Simplex_Receive:
	    set_spi_simplex_receive(SPIx);
	    return;

	case SPI_Comm_Simplex_Transmit:
	    set_spi_simplex_transmit(SPIx);
	    return;
    }
}

void enable_spi_interface(SPI_Channel spi_chan) {

    switch(spi_chan) {
	case SPI_Channel_1:
	    /* enable SPI1 bus clock */
	    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	    break;

	case SPI_Channel_2:
	    /* enable SPI2 bus clock */
	    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
	    break;

	case SPI_Channel_3:
	    /* enable SPI3 bus clock */
	    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
	    break;
    }

    /* wait for 2 clock cycles. */
    __WAIT_2_CLOCKS();
}

void disable_spi_interface(SPI_Channel spi_chan) {
    switch(spi_chan) {
	case SPI_Channel_1:
	    /* disable SPI1 bus clock */
	    RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
	    break;

	case SPI_Channel_2:
	    /* disable SPI2 bus clock */
	    RCC->APB1ENR1 &= ~RCC_APB1ENR1_SPI2EN;
	    break;

	case SPI_Channel_3:
	    /* disable SPI3 bus clock */
	    RCC->APB1ENR1 &= ~RCC_APB1ENR1_SPI3EN;
	    break;
    }
}

void set_spi_datasize(SPI_TypeDef *const SPIx, SPI_Datasize spi_datasize) {

    /* clear datasize bits */
    SPIx->CR2 &= ~SPI_CR2_DS;

    /* set the new datasize bits */
    SPIx->CR2 |= (spi_datasize << SPI_CR2_DS_Pos);

    /* 
     * rxfifo interrupt threshold setting. If datasize 
     * is less than 16 bits, receive interrupt when 
     * rxfifo is 1/4th or more full. Otherwise receive 
     * interrupt when rxfifo is 1/2 or more full. 
     */
    if(spi_datasize < SPI_DATASIZE_16) 
	SPIx->CR2 |= SPI_CR2_FRXTH;
    else
	SPIx->CR2 &= ~SPI_CR2_FRXTH;
}

void set_spi_baud_rate(SPI_TypeDef *const SPIx, SPI_Baudrate_Divisor spi_br_div) {

    /* clear baudrate bits */
    SPIx->CR1 &= ~SPI_CR1_BR;

    /* set new baudrate bits */
    SPIx->CR1 |= (spi_br_div << SPI_CR1_BR_Pos);
}

void set_spi_clock_pol(SPI_TypeDef *const SPIx, SPI_Clock_Pol clk_pol) {
    switch(clk_pol) {
	case SPI_CPOL_Idle_Hi:
	    SPIx->CR1 |= SPI_CR1_CPOL;
	    return;

	case SPI_CPOL_Idle_Lo:
	    SPIx->CR1 &= ~SPI_CR1_CPOL;
	    return;
    }
}

void set_spi_clock_pha(SPI_TypeDef *const SPIx, SPI_Clock_Pha clk_pha) {
    switch(clk_pha) {
	/* data transaction begins on the first active edge of the clock */
	case SPI_CPHA_Imm:
	    SPIx->CR1 &= ~SPI_CR1_CPHA;
	    return;

	    /* data transaction begins on the second active edge of the clock */
	case SPI_CPHA_Del:
	    SPIx->CR1 |= SPI_CR1_CPHA;
	    return;
    }
}

void set_spi_frame_format(SPI_TypeDef *const SPIx, SPI_Frame_Format frm_fmt) {
    switch(frm_fmt) {
	case SPI_MSB_First:
	    SPIx->CR1 &= ~SPI_CR1_LSBFIRST;
	    return;

	case SPI_LSB_First:
	    SPIx->CR1 |= SPI_CR1_LSBFIRST;
	    return;
    }
}

void set_spi_full_duplex(SPI_TypeDef *const SPIx) {
    /* clear bidirection data bit */
    SPIx->CR1 &= ~SPI_CR1_BIDIMODE;
}

void set_spi_half_duplex(SPI_TypeDef *const SPIx) {
    /* set bidirection data bit */
    SPIx->CR1 |= SPI_CR1_BIDIMODE;
}

void set_spi_simplex_transmit(SPI_TypeDef *const SPIx) {
    /* setting same as full duplex. Ignore the unnecessary pin: MISO or MOSI */
    set_spi_full_duplex(SPIx);
}

void set_spi_simplex_receive(SPI_TypeDef *const SPIx) {
    /* clear bidirection data bit */
    SPIx->CR1 &= ~SPI_CR1_BIDIMODE;

    /* set rxonly bit */
    SPIx->CR1 |= SPI_CR1_RXONLY;
}

void set_spi_single_master_mode(SPI_TypeDef *const SPIx) {

    /* set the master mode bit */
    SPIx->CR1 |= SPI_CR1_MSTR;

    /* hardware slave select management */
    SPIx->CR1 &= ~SPI_CR1_SSM;

    /* slave select output enabled */
    SPIx->CR2 |= SPI_CR2_SSOE;
}

void set_spi_multi_master_mode(SPI_TypeDef *const SPIx) {

    /* set the master mode bit */
    SPIx->CR1 |= SPI_CR1_MSTR;

    /* hardware slave select management */
    SPIx->CR1 &= ~SPI_CR1_SSM;

    /* slave select output disabled */
    SPIx->CR2 &= ~SPI_CR2_SSOE;
}

void set_spi_slave_mode(SPI_TypeDef *const SPIx) {

    /* clear the master mode bit */
    SPIx->CR1 &= ~SPI_CR1_MSTR;

}

void set_spi_gpio_af(SPI_Channel spi_chan) {
    switch(spi_chan) {
	case SPI_Channel_1:
	    /* PA4, PA15 af nss,  af5 */
	    /* PA5, PB3  is clk,  af5 */
	    /* PA6, PB4  is miso, af5 */
	    /* PA7, PB5  is mosi, af5 */

	    init_gpio_as_af(GPIO_PA, GPIO_Pin_4, GPIO_AF5);
	    init_gpio_as_af(GPIO_PA, GPIO_Pin_5, GPIO_AF5);
	    init_gpio_as_af(GPIO_PA, GPIO_Pin_6, GPIO_AF5);
	    init_gpio_as_af(GPIO_PA, GPIO_Pin_7, GPIO_AF5);

	    return;

	case SPI_Channel_2:
	    /* PB12, PF0  af nss,  af5 */
	    /* PB13, PF1  is clk,  af5 */
	    /* PA10, PB14 is miso, af5 */
	    /* PA11, PB15 is mosi, af5 */

	    /* PD15 af nss,  af6 */
	    return;

	case SPI_Channel_3:
	    /* PA4, PA15 af nss,  af6 */
	    /* PB3, PC10 is clk,  af6 */
	    /* PB4, PC11 is miso, af6 */
	    /* PB5, PC12 is mosi, af6 */
	    return;
    }
}

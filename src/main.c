#include "flash.h"
#include "clock.h"
#include "lpuart.h"
#include "print.h"
#include "delay.h"
#include "power.h"
#include "systick.h"
#include "gpio.h"
#include "spi.h"

int main(void) {

    set_sysclk_pll(PLL_HSE_32);
    enable_systick();
    init_lpuart1(115200);
    my_printf("lpuart initialized. sysclk is %d MHz, ahbclk is %d MHz, pclk1 = %d MHz and pclk2 = %d MHz\n", 
	    get_sysclk_speed() / 1000000, 
	    get_hclk_speed() / 1000000,
	    get_pclk1_speed() / 1000000,
	    get_pclk2_speed() / 1000000);

    uint8_t rxbuf[256], txbuf[256];
    volatile uint8_t rxbuf_windex, rxbuf_rindex;

    rxbuf_rindex = rxbuf_windex = 0;

    for(int i = 0; i < 256; ++i)
	txbuf[i] = i + 1;

    spi_config_rxbuffer(rxbuf, &rxbuf_windex);
    spi_config_txbuffer(txbuf, 100U);
    init_spi(SPI_Channel_1, SPI_Peripheral, SPI_CPHA_Imm, SPI_CPOL_Idle_Lo, SPI_Comm_Full_Duplex, SPI_MSB_First, SPI_DATASIZE_8);

    spi_start(SPI_Channel_1);

    while(1) {
	if(rxbuf_windex != rxbuf_rindex) {
	    my_printf("Read: %d[w:%d]\n", rxbuf[rxbuf_rindex++], rxbuf_windex);
	}
    }
    return 0;
}

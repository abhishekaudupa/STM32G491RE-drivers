#ifndef __SPI_H__
#define __SPI_H__

#include "config.h"

typedef enum {
    SPI_Channel_1,
    SPI_Channel_2,
    SPI_Channel_3
} SPI_Channel;

typedef enum {
    SPI_Single_Control,
    SPI_Multi_Control,
    SPI_Peripheral
} SPI_Mode;

typedef enum {
    SPI_Comm_Full_Duplex,
    SPI_Comm_Half_Duplex,
    SPI_Comm_Simplex_Receive,
    SPI_Comm_Simplex_Transmit
} SPI_Comm_Mode;

typedef enum {
    SPI_CPOL_Idle_Lo,
    SPI_CPOL_Idle_Hi
} SPI_Clock_Pol;

typedef enum {
    SPI_CPHA_Imm,
    SPI_CPHA_Del
} SPI_Clock_Pha;

typedef enum {
    SPI_MSB_First,
    SPI_LSB_First
} SPI_Frame_Format;

typedef enum {
    SPI_DATASIZE_4 	= 0x3U,
    SPI_DATASIZE_5 	= 0x4U,
    SPI_DATASIZE_6 	= 0x5U,
    SPI_DATASIZE_7 	= 0x6U,
    SPI_DATASIZE_8 	= 0x7U,
    SPI_DATASIZE_9 	= 0x8U,
    SPI_DATASIZE_10 	= 0x9U,
    SPI_DATASIZE_11 	= 0xAU,
    SPI_DATASIZE_12 	= 0xBU,
    SPI_DATASIZE_13 	= 0xCU,
    SPI_DATASIZE_14 	= 0xDU,
    SPI_DATASIZE_15 	= 0xEU,
    SPI_DATASIZE_16 	= 0xFU
} SPI_Datasize;

typedef enum {
    SPI_BAUD_D2		= 0x0U,
    SPI_BAUD_D4		= 0x1U,
    SPI_BAUD_D8		= 0x2U,
    SPI_BAUD_D16	= 0x3U,
    SPI_BAUD_D32	= 0x4U,
    SPI_BAUD_D64	= 0x5U,
    SPI_BAUD_D128	= 0x6U,
    SPI_BAUD_D256	= 0x7U
} SPI_Baudrate_Divisor;

void init_spi(SPI_Channel spi_chan, 
	SPI_Mode spi_mode, 
	SPI_Clock_Pha spi_cpha, 
	SPI_Clock_Pol spi_cpol, 
	SPI_Comm_Mode spi_comm_mode, 
	SPI_Frame_Format spi_fmt, 
	SPI_Datasize spi_datasize);
void spi_start(SPI_Channel spi_chan);
void spi_config_rxbuffer(volatile void *rxbuf, volatile uint16_t *rxbuf_windex);
void spi_config_txbuffer(volatile void *txbuf, const uint16_t txbuf_size);

#endif

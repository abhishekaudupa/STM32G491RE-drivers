#include <stdio.h>
#include <stdint.h>

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define __MHz(n) (n*1000000)
#define __kHz(n) (n*1000)
#define HSE 0
#define HSI 1

void pll(uint32_t freq, uint8_t src) {
    uint8_t m[] = {1,2,3,4,5,6,7,8,9};
    int ct = 1;
    uint32_t freqs[200] = {0};
    uint8_t mfs[200] = { 0 };
    uint8_t nfs[200] = { 0 };
    uint8_t rfs[200] = { 0 };

    for(int i = 0; i < ARR_SIZE(m); ++i) {
	if(freq % m[i] == 0) {
	    uint32_t vco = (freq / m[i]);
	    if(vco > 2660000 && vco < 16000000)
		for(int n = 8; n < 128; ++n) {
		    for(int r = 2; r <= 8; r +=2) {
			if((vco * n) % r == 0) {
			    uint32_t pllr = vco * n / r;
			    if(pllr <= 170000000 && pllr >= 16000000) {
				uint32_t pllrmhz = pllr / 1000000;
				if(!freqs[pllrmhz]) {
				    freqs[pllrmhz] = pllr;
				    mfs[pllrmhz] = m[i];
				    nfs[pllrmhz] = n;
				    rfs[pllrmhz] = r;
				}
			    }
			}
		    }
		}
	}
    }

    ct = 1;
    for(int i = 0; i < 200; ++i) {
	if(!freqs[i])
	    continue;
	if(src == HSE && freqs[i] == 24000000)
	    continue;
	if(src == HSI && freqs[i] == 16000000)
	    continue;
	if(freqs[i] % 512 == 0) {
	    //printf("   %2d: PLLR = %3u MHz(%9u), m = %u, n = %3u, r = %u\n", ct++, (freqs[i] / 1000000), freqs[i], mfs[i], nfs[i], rfs[i]);
	    if(src == HSE)
		printf("#define PLL_HSE_");
	    else
		printf("#define PLL_HSI_");

	    printf("%u\t", freqs[i]/1000000);
	    if(src == HSE)
		printf("PLLClk_HSE, ");
	    else
		printf("PLLClk_HSI, ");
	    printf("PLLM_%u, PLLN_%u, ", mfs[i], nfs[i]);
	    if(nfs[i] < 10)
		printf("  ");
	    else if(nfs[i] < 100)
		printf(" ");
	    printf("PLLR_%u, %9uU\n", rfs[i], freqs[i]);
	}
    }
}

void plln_muls() {
    for(int i = 8; i <= 99; ++i) {
	printf("#define PLLN_%d\t\t0x%02XU\n", i, i);
    }
    for(int i = 100; i <= 127; ++i) {
	printf("#define PLLN_%d\t0x%02XU\n", i, i);
    }
}

void pllm_divs() {
    for(int i = 1; i <= 16; ++i)
	printf("#define PLLM_%d\t\t0x%XU\n", i, i - 1);
}

void pllr_divs() {
    for(int i = 2; i <= 8; i += 2)
	printf("#define PLLR_%d\t\t0x%XU\n", i, i/2-1);
}

void spi_datasz() {
    for(int i = 4; i <= 16; ++i)
	printf("#define SPI_DATASIZE_%d\t0x%XU\n", i, i - 1);
}

void spi_baud() {
    int j = 0;
    for(int i = 2; i <= 256; i *= 2)
	printf("#define SPI_BAUD_D%d\t0x%XU\n", i, j++);
}

int main(int argc, char **argv) {
    for(int i = 0; i < 2; ++i) {
	uint32_t freq;
	switch(i) {
	    case 0:
		freq = 16000000;
		pll(freq, HSI);
		break;
	    case 1:
		freq = 24000000;
		pll(freq, HSE);
	    default:
		break;
	}
    }
    printf("\n/* PLL M divisors */\n");
    pllm_divs();
    
    printf("\n/* PLL N multipliers */\n");
    plln_muls();
    
    printf("\n/* PLL R divisors */\n");
    pllr_divs();

    printf("\n/* SPI datasize bits */\n");
    spi_datasz();
    
    printf("\n/* SPI baudrate bits */\n");
    spi_baud();
}

#include <stdio.h>
#include <stdint.h>

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define __MHz(n) (n*1000000)
#define __kHz(n) (n*1000)

void pll(uint32_t freq) {
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
				    //printf("Inserting %u at %u\n", pllr, pllrmhz);
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
	if(freqs[i] % 512 == 0) {
	    printf("   %2d: PLLR = %3u MHz, m = %u, n = %3u, r = %u\n", ct++, (freqs[i] / 1000000), mfs[i], nfs[i], rfs[i]);
	}
    }
}

int main(int argc, char **argv) {
    for(int i = 0; i < 2; ++i) {
	uint32_t freq;
	switch(i) {
	    case 0:
		printf("Source HSI, 16MHz\n");
		freq = 16000000;
		pll(freq);
		break;
	    case 1:
		printf("Source HSE, 24MHz\n");
		freq = 24000000;
		pll(freq);
	    default:
		break;
	}
    }
}


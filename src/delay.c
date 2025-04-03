#include "delay.h"
#include "systick.h"

void delay(const uint32_t ms) {
    uint32_t now = sysTick;
    while(now + ms > sysTick);
}

#include <stdint.h>
#include "stm32g491xx.h"

void get_device_uid(const uint32_t *uid_buf) {
    uid_buf = (uint32_t*)UID_BASE;
}

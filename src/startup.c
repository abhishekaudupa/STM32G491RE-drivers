#include "stm32g491xx.h"
#include "startup.h"
#include "nucleoG491RE.h"
#include "clock.h"

#define __weak			__attribute__ ((weak, alias("default_handler")))

#define SRAM_END		(SRAM1_BASE + SRAM1_SIZE_MAX)
#define STACK_PTR_INIT_ADDR	(SRAM_END)

#define STM32G4_INTR_COUNT	(102)
#define STM32_EXCP_COUNT	(15)
#define IVT_SIZE		(STM32G4_INTR_COUNT + STM32_EXCP_COUNT + 1)	/* +1 for stack pointer address */

volatile uint32_t sysTick;
uint32_t dbg;

/* STM32 standard exception handlers */
void reset_handler(void);
void systick_handler(void);
__weak void nmi_handler(void);
__weak void hard_fault_handler(void);
__weak void memory_management_fault_handler(void);
__weak void bus_fault_handler(void);
__weak void usage_fault_handler(void);
__weak void svcall_handler(void);
__weak void pendsv_handler(void);
__weak void lpuart1_IRQ_handler(void);

void main(void);

/* vector table as seen in PM0214 Rev10 Page#40 Table 17 */
uint32_t ivt[IVT_SIZE] __attribute__((section(".isr_vector"))) = {
    STACK_PTR_INIT_ADDR,
    (uint32_t)&reset_handler,
    (uint32_t)&nmi_handler,
    (uint32_t)&hard_fault_handler,
    (uint32_t)&memory_management_fault_handler,
    (uint32_t)&bus_fault_handler,
    (uint32_t)&usage_fault_handler,
    0,
    0,
    0,
    0,
    (uint32_t)&svcall_handler,
    0,
    0,
    (uint32_t)&pendsv_handler,
    (uint32_t)&systick_handler,

    /* IRQ 0-9 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 10-19 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,

    /* IRQ 20-29 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 30-39 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 40-49 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 50-59 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 60-69 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 70-79 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 80-89 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 90-99 */
    0,
    (uint32_t)&lpuart1_IRQ_handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    
    /* IRQ 100-101 */
    0,
    0
};

/* these are defined in linker script */
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _sidata;

/* This is our main boot function */
void reset_handler(void) {

    __disable_irq();
    sysTick = 0;

    /* calculate data size */ 
    uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;

    /* flash data section start address */
    uint8_t *flash_data = (uint8_t*)&_sidata;

    /* SRAM data section start address */
    uint8_t *sram_data = (uint8_t*)&_sdata;

    /* copy data from flash to SRAM */
    for(uint32_t i = 0; i < data_size; ++i)
	sram_data[i] = flash_data[i];

    /* Calculate bss size */
    uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;

    /* bss section start address */
    uint8_t *bss = (uint8_t*)&_sbss;

    /* Zero out BSS section in SRAM */
    for(uint32_t i = 0; i < bss_size; ++i) {
	bss[i] = 0;
    }

    set_sysclk_hse();
    update_clocks();
    main();
    for(;;) (void)0;
}

void systick_handler() {
    sysTick++;
}

void default_handler() {}


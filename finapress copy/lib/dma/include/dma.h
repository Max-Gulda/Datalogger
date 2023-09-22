#include "gd32vf103.h"

uint16_t sample_buffer[8];

int timer_config(uint32_t sample_rate);
void adc_config(void);
void dma_config();
void DMA0_Channel0_IRQHandler(void);
#pragma once

#include <inttypes.h>

uint64_t clock_get_nsec_from_isr(void);
uint64_t clock_get_nsec(void);
void systick_delay_us(uint32_t us);
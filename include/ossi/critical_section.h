#pragma once
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Worst-case storage type for a mutex across platforms.
 *
 * This type is chosen to be large enough to accommodate the mutex implementation
 * on any supported platform. Static assertions are used later to ensure that the
 * actual platform-specific mutex types fit within this storage type.
 */
#define SI_CRITICAL_STORAGE_BYTES 88u
#define SI_CRITICAL_STORAGE_ALIGN 4u

typedef struct critical_section_s {
    bool isinit;
    unsigned char storage[SI_CRITICAL_STORAGE_BYTES] __attribute__((aligned(SI_CRITICAL_STORAGE_ALIGN)));
} critical_section_t;

void si_init_critical(critical_section_t *me);
bool si_enter_critical(critical_section_t *me, uint32_t timeout_ticks);
void si_leave_critical(critical_section_t *me);
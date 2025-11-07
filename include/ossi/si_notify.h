#pragma once
#include <stdbool.h>

#include <libossi.h>

#ifdef LIBOSSI_FREERTOS
#include <FreeRTOS.h>
#include <task.h>

typedef struct si_notify_data_s {
    TaskHandle_t handle;
    uint32_t index;
} si_notify_data_t;
#endif

#ifdef LIBOSSI_POSIX
typedef struct si_notify_s {
    uint8_t not_implemented_yet;
} si_notify_t;
#endif

typedef struct si_notify_s {
    bool isinit;
    si_notify_data_t data;
} si_notify_t;

extern void si_notify_init(si_notify_t *me, uint32_t index, void *handle);
extern uint32_t si_notify_wait(si_notify_t *me, uint32_t wait_ms);
extern void si_notify_isr(si_notify_t *me);
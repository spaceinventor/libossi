#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/* FreeRTOS version */
#include <FreeRTOS.h>
#include <task.h>

#include "ossi/si_notify.h"

void si_notify_init(si_notify_t *me, uint32_t index, void *handle) {

    if (!me->isinit) {
        me->data.handle = handle;
        me->data.index = index;
        me->isinit = true;
    }
}

uint32_t si_notify_wait(si_notify_t *me, uint32_t wait_ms) {

    uint32_t value = 0;
    TickType_t ticks_to_wait = portMAX_DELAY;
    if (wait_ms < UINT32_MAX) {
        ticks_to_wait = pdMS_TO_TICKS(wait_ms);
    }
    if (me->isinit) {
        value = ulTaskGenericNotifyTake(me->data.index, true, ticks_to_wait);
    }
    return value;
}

void si_notify_isr(si_notify_t *me) {

    BaseType_t woken = pdFALSE;
    if (me->isinit) {
        vTaskGenericNotifyGiveFromISR(me->data.handle, me->data.index, &woken);
        portYIELD_FROM_ISR(woken);
    }
}
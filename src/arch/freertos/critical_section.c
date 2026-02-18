#include <ossi/critical_section.h>
#include <FreeRTOS.h>
#include <semphr.h>

typedef struct {
    SemaphoreHandle_t hsem;
    StaticSemaphore_t mutex;
} critical_section_impl_t;

_Static_assert(sizeof(critical_section_impl_t) <= SI_CRITICAL_STORAGE_BYTES, "critical_section_t storage too small");
_Static_assert(_Alignof(critical_section_impl_t) <= SI_CRITICAL_STORAGE_ALIGN, "critical_section_t storage alignment too small");

static inline critical_section_impl_t *impl(critical_section_t *me) {
    return (critical_section_impl_t *)(void *)me->storage;
}

void si_init_critical(critical_section_t *me) {

    if (me->isinit) {
        return;
    }
    impl(me)->hsem = xSemaphoreCreateMutexStatic(&impl(me)->mutex);
    me->isinit = true;
}

bool si_enter_critical(critical_section_t *me, uint32_t timeout_ms) {
    if (!me->isinit) {
        return false;
    }
    return xSemaphoreTake(impl(me)->hsem, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

void si_leave_critical(critical_section_t *me) {

    if (!me->isinit) {
        return;
    }
    (void)xSemaphoreGive(impl(me)->hsem);
}

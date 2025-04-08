#include <stdint.h>

#include "message_queue.h"

void message_queue_create(message_queue_t *me, uint32_t item_size, uint32_t length, void *storage) {

    return;
}

int message_queue_send(message_queue_t *me, void *item) {

    return 0;
}

int message_queue_send_isr(message_queue_t *me, void *item) {

    return 0;
}

int message_queue_receive(message_queue_t *me, void *item) {

    return 0;
}

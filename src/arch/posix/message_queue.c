#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>

#include "ossi/message_queue.h"

char cleanup_buffer[OS_QUEUE_SIZE];

static void message_queue_cleanup(void) {    
    snprintf(cleanup_buffer, OS_QUEUE_SIZE - 1, "/ossi_%d", getpid());
    mq_unlink(cleanup_buffer);
}

void message_queue_create(message_queue_t *me, uint32_t item_size, uint32_t length, void *storage) {
    struct mq_attr attr;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = item_size;
    attr.mq_flags   = 0;

    snprintf((char *)me->obj, sizeof(me->obj) - 1, "/ossi_%d", getpid());
    mqd_t mqd = mq_open ((const char *)me->obj, O_RDWR|O_CREAT, mode, &attr);
    if (mqd == -1)
        {
        perror("message_queue_create: mq_open() failure");
        exit(0);
    };
    atexit(message_queue_cleanup);
    /* Bit nasty, this. Using the handle to store the queue id */
    memcpy(&me->handle, &mqd, sizeof(mqd));
}

int message_queue_send(message_queue_t *me, void *item) {    
    struct mq_attr attr;
    mq_getattr((mqd_t)(intptr_t)me->handle, &attr);
    mq_send ((mqd_t)(intptr_t)me->handle, (char *)item, attr.mq_msgsize, 10);
    return 0;
}

int message_queue_send_isr(message_queue_t *me, void *item) {
    struct mq_attr attr;
    mq_getattr((mqd_t)(intptr_t)me->handle, &attr);
    mq_send ((mqd_t)(intptr_t)me->handle, (char *)item, attr.mq_msgsize, 10);
    return 0;
}

int message_queue_receive(message_queue_t *me, void *item) {
    unsigned int priority = 0;
    struct mq_attr attr;
    mq_getattr((mqd_t)(intptr_t)me->handle, &attr);
    ssize_t res = mq_receive ((mqd_t)(intptr_t)me->handle, (char *)item, attr.mq_msgsize, &priority);
    if (res == -1) {
        perror("message_queue_receive: mq_receive() failure");
        exit(0);
    } 
    return 0;
}

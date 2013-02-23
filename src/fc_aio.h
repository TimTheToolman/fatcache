#ifndef _FC_AIO_H_
#define _FC_AIO_H_

#include <libaio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/signalfd.h>
#include <sys/eventfd.h>
#include <fc_core.h>
#include <fc_event.h>


io_context_t aio_ctx;

int aio_fd;

typedef void (*aio_callback_t)(void *data);

int aio_init(struct context *ctx);

int aio_write(int fd, void *buffer, size_t length, off_t offset, aio_callback_t cb);

int aio_read(int fd, void *buffer, size_t length, off_t offset, aio_callback_t cb);

void aio_events(struct epoll_event *ev, struct context *ctx);


#endif

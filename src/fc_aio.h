#ifndef _FC_AIO_H_
#define _FC_AIO_H_

#include <libaio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/signalfd.h>
#include <sys/eventfd.h>

#include <fc_core.h>

typedef void (*aio_callback_t)(void *data);

int aio_init();

int aio_write(int fd, void *buffer, size_t length, off_t offset, aio_callback_t cb);

int aio_read(int fd, void *buffer, size_t length, off_t offset, aio_callback_t cb);

void aio_events(uint64_t events);


#endif

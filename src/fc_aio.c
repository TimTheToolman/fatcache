#include <fc_aio.h>

io_context_t aio_ctx;
int aio_fd;

int aio_init(struct context *ctx){
	
	struct io_event *event;

	aio_fd = eventfd(0, EFD_NONBLOCK);
	event = fc_calloc(5, sizeof(*ctx->aio_event));

	
	ctx->aio_fd = aio_fd;
	ctx->aio_event = event;

	return 0;
}

int aio_write(int fd, void *buffer, size_t length, off_t offset, aio_callback_t cb){
	struct iocb iocb;
	struct iocb *piocb = &iocb;
	int resp;
	
	io_prep_pwrite(&iocb, fd, buffer, length, offset);
	iocb.data = cb;
	io_set_eventfd(&iocb, aio_fd);
	

	resp = io_submit(aio_ctx,1, &piocb);
	
	return resp;
}

int aio_read(int fd, void *buffer, size_t length, off_t offset, aio_callback_t cb){
	struct iocb iocb;
        struct iocb *piocb = &iocb;
        int resp;

        io_prep_pread(&iocb, fd, buffer, length, offset);
        iocb.data = cb;
	io_set_eventfd(&iocb, aio_fd);

        resp = io_submit(aio_ctx,1, &piocb);
        return resp;
}

void aio_events(struct epoll_event *ev, struct context *ctx){
	uint64_t io_events;
	size_t length;
	int nevents, i;
	aio_callback_t cb;	

	length = read(ev->data.fd, &io_events, sizeof(uint64_t));
	
	if(length != sizeof(uint64_t)){
		return;
	}
	
	nevents = io_getevents(aio_ctx,io_events,io_events, ctx->aio_event, NULL);
	for(i=0; i<nevents; i++){
		/* make sure the write or read is complete */
		cb = ctx->aio_event[i].data;
		cb(ctx->aio_event[i].obj->u.c.buf);
		
		free(ctx->aio_event[i].obj->u.c.buf);
		free(ctx->aio_event[i].obj);
	}
}

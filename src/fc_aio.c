#include <fc_aio.h>

io_context_t aio_ctx;
int aio_fd;
unsigned ret_io_events;

int aio_init(int ctx_fd){
	ctx_fd = eventfd(0, EFD_NONBLOCK);
	aio_fd = ctx_fd;

	return 1;
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

void aio_events(uint64_t events){


}

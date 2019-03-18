#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <string>
#include <assert>
#include <stdlib.h>
#include <pthread.h>

#define _DEFAULT_RINGBUF_SIZE_ 8096

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

class RingBuff {
	public:
		RingBuff(uint32_t buf_size = _DEFAULT_RINGBUF_SIZE_);
		~RingBuff();

		getData(void* buff, uint32_t size);
		putData(void* buff, uint32_t size);
		uint32_t getReaableSize();
		uint32_t getAvaliableSize();

	private:
		uint8_t *buf;
		uint32_t bufSize;
		int head;
		int tail;

};

RingBuff::RingBuff(uint32_t buf_size)
{
	buf = nullptr;
	*buf = new uint8_t(buf_size);
	assert(buf);
	bufsize = buf_size;
	int head = 0;
	int tail = -1;
}

RingBuff:RingBuff()
{
	delete [] buf;
	buf = nullptr;
}



#endif

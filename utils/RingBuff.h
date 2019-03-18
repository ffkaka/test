#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define _DEFAULT_RINGBUF_SIZE_ 8096
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

class RingBuff {
	public:
		RingBuff(uint32_t buf_size = _DEFAULT_RINGBUF_SIZE_);
		~RingBuff();

		uint32_t getData(void* buff, uint32_t size);
		uint32_t putData(void* buff, uint32_t size);
		uint32_t getReadableSize();
		uint32_t getWritableSize();

	private:
		uint8_t *buf;
		uint32_t bufSize;
		int head;
		int tail;

};

RingBuff::RingBuff(uint32_t buf_size)
{
	buf = NULL;
	*buf = new uint8_t[buf_size];
	assert(buf);
	bufSize = buf_size;
	int head = 0;
	int tail = -1;
}

RingBuff::~RingBuff()
{
	delete [] buf;
	buf = NULL;
}

uint32_t RingBuff::getData(void* buff, uint32_t size)
{
	auto ibuf = (uint8_t*)buff;
	if (head < tail) {
		if ( bufSize - tail > size ) {
			memcpy(&buf[tail], ibuf, size);
			tail += size;
			return size;
		}
		else {
			auto rest = bufSize - tail;
			memcpy(ibuf, &buf[tail], rest);
			memcpy(ibuf + rest, &buf[0], size - rest);
			tail = size - rest;
			return size;
		}
	}
	else {
		memcpy(ibuf, &buf[tail], size);
		tail += size;
	}
	return size;
}

uint32_t RingBuff::putData(void* buff, uint32_t size) {
	auto ibuf = (uint8_t*)buff;
	if (head < tail) {
		uint32_t avaliable = tail - head;
		if (avaliable >= size) {
			memcpy(&buf[head], ibuf, size);
			head +=size; 
			return size;
		}
		else {
			memcpy(&buf[head], ibuf, avaliable);
			head += avaliable; 
			return avaliable;
		}
	}
	else {
		auto rest = bufSize - head;
		if (rest >= size) {
			memcpy(&buf[head], ibuf, size);
			head += size;
			return size;
		}
		else {
			memcpy(&buf[head], ibuf, rest);
			memcpy(&buf[0], ibuf+rest, size - rest);
			head = size - rest;
			return size;
		}
	}
	return size;
}


uint32_t RingBuff::getReadableSize() {
	if (head < tail) {
		return (bufSize - tail) + head;
	}
	else return head - tail;
}

uint32_t RingBuff::getWritableSize() {
	if (head < tail) {
		return tail - head - 1;
	}
	else {
		return (bufSize - head) + tail - 1;
	}
}

#endif

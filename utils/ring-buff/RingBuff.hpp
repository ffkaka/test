#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/****************************************************
 * This is a Ringbuffer samples which can be used for multi-threaded application
 * write operation will overwrite even there is not read datas. 
 * ffkaka
 ***************************************************/

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

#define _DEFAULT_RINGBUF_SIZE_ 1024 * 1024 * 2 // 2MB
//typedef unsigned int uint32_t;
//typedef unsigned char uint8_t;

class RingBuff {
	public:
		RingBuff(uint32_t buf_size = _DEFAULT_RINGBUF_SIZE_);
		~RingBuff();

		/*
		 * discrition : will copy data in ring buffer as the amount of the size. 
		 * params in [buff] : The pointer which will be copied with the ring buffer contents.
		 * params in [size] : The size which will be copied to the buff
		 * return : The total copied size
		 */
		uint32_t getData(void* buff, uint32_t size);

		/*
		 * discrition : will copy the buff dat into the ring buffer as of size. 
		 * This will write data even though there are data which are not poped yet(overwrite by default)
		 * params in [buff] : The pointer which will point the contents would be copied
		 * params in [size] : The size which will be copied to the ring buffer
		 * return : The total copied size
		 */
		void putData(void* buff, uint32_t size);

		/*
		 * discrition : will return the size of contents in the ring buffer
		 * return : The size can put the data
		 */
		uint32_t getReadableSize();

		/*
		 * discrition : will return the size of the empty space in ring buffer
		 * return : The empty space in the ring buffer
		 */
		uint32_t getWritableSize();

	private:
		uint8_t *buf;
		uint32_t bufSize;
		int head; //write to head
		int tail; //read from tail
		pthread_mutex_t mtx;
		
		void moveHead(uint32_t size);
		void moveTail(uint32_t size);

};

RingBuff::RingBuff(uint32_t buf_size)
{
	buf = NULL;
	buf = new uint8_t[buf_size]();
	assert(buf);
	bufSize = buf_size;
	head = 0;
	tail = -1;
	pthread_mutex_init(&mtx, NULL);
}

RingBuff::~RingBuff()
{
	delete [] buf;
	buf = NULL;
}

uint32_t RingBuff::getData(void* buff, uint32_t size)
{
	pthread_mutex_lock(&mtx);
	uint32_t ret = 0;
	tail = (tail == -1)? 0 : tail;
	auto ibuf = (uint8_t*)buff;
	if (head < tail) {
		if ( bufSize - tail > size ) {
			memcpy(&buf[tail], ibuf, size);
			tail += size;
			ret = size;
		}
		else {
			auto rest = bufSize - tail;
			memcpy(ibuf, &buf[tail], rest);
			memcpy(ibuf + rest, &buf[0], size - rest);
			tail = size - rest;
			ret = size;
		}
	}
	else {
		memcpy(ibuf, &buf[tail], size);
		tail += size;
	}
	pthread_mutex_unlock(&mtx);
	return ret;
}

// This will overwrite if the rest size is smaller then the input buf size
// So the tail position will be moved to 
void RingBuff::putData(void* buff, uint32_t size) {
	if (size >= bufSize) {
		printf("!!! Input Data is too larget. will not push data !!!\n");
		return;
	}

	pthread_mutex_lock(&mtx);
	tail = (tail == -1)? 0 : tail;
	uint32_t ret = 0;
	auto ibuf = (uint8_t*)buff;
	if (head < tail) {
		uint32_t avaliable = tail - head;
		if (avaliable >= size) {
			memcpy(&buf[head], ibuf, size);
			moveHead(size); 
		}
		else {
			memcpy(&buf[head], ibuf, avaliable);
			moveHead(avaliable); 
			memcpy(&buf[head], ibuf+avaliable, size - avaliable);
			moveHead(size - avaliable);
		}
	}
	else {
		auto rest = bufSize - head;
		if (rest >= size) {
			memcpy(&buf[head], ibuf, size);
			moveHead(size); 
		}
		else {
			memcpy(&buf[head], ibuf, rest);
			moveHead(rest);
			memcpy(&buf[head], ibuf+rest, size - rest);
			moveHead(size - rest);
			if (tail < size - rest) {
				printf("File over wrote\n");
				moveTail(head - tail + 1);
			}
		}
	}
	pthread_mutex_unlock(&mtx);
}


uint32_t RingBuff::getReadableSize() {
	pthread_mutex_lock(&mtx);
	uint32_t ret = 0;
	if (head < tail) {
		ret = (bufSize - tail) + head;
	}
	else ret = head - tail;
	pthread_mutex_unlock(&mtx);
	return ret;
}

uint32_t RingBuff::getWritableSize() {
	pthread_mutex_lock(&mtx);
	uint32_t ret = 0;
	if (head < tail) {
		ret = tail - head - 1;
	}
	else {
		ret = (bufSize - head) + tail - 1;
	}
	pthread_mutex_unlock(&mtx);
	return ret;
}

void RingBuff::moveHead(uint32_t dist) {
	if (head + dist >= bufSize) {
		head = dist - (bufSize - head);
	}
	else head += dist;
}

void RingBuff::moveTail(uint32_t dist) {
	if (tail + dist >= bufSize) {
		tail = dist - (bufSize - tail);
	}
	else tail += dist;
}

#endif

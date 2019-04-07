/****************************************************
 * Simple API Instance using pulse simple API
 * This will record a mic input from pulse audio 
 * ffkaka
 ***************************************************/

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <locale.h>

// For get the pid in user space
#include <sys/types.h>
#include <sys/syscall.h>

#include <pulse/error.h>
#include "pulse-input-handler.h"
#include "RingBuff.hpp"

//#define INPUT_UNIT_SIZE 768

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

RingBuff gBuff;

int getFormatSize(pa_sample_format_t format)
{
	int ret = 1;
	switch (format)
	{
		case PA_SAMPLE_U8:
		case PA_SAMPLE_ALAW:
		case PA_SAMPLE_ULAW:
			ret = 1;
			break;
		case PA_SAMPLE_S16LE:
		case PA_SAMPLE_S16BE:
			ret = 2;
			break;
		case PA_SAMPLE_S24LE:
		case PA_SAMPLE_S24BE:
			ret = 3;
			break;
		case PA_SAMPLE_FLOAT32LE:
		case PA_SAMPLE_FLOAT32BE:
		case PA_SAMPLE_S32LE:
		case PA_SAMPLE_S32BE:
		case PA_SAMPLE_S24_32LE:
		case PA_SAMPLE_S24_32BE:
			ret = 4;
			break;
		default :
			break;
	}
	return ret;
}

PulseInputHandler::PulseInputHandler(int rate, in_read_format_t format, int channels) {
	sample_spec = pa_xnew0(pa_sample_spec, 1);
	sample_spec->format = PA_SAMPLE_S32LE;
	sample_spec->rate = 48000;
	sample_spec->channels = (uint8_t)1;
	readThread = 0; needRun = false;
	userRate = rate;
	userFormat = format;
	pthread_mutex_init(&mutex, nullptr);
	pthread_cond_init(&cond, nullptr);
}

PulseInputHandler::~PulseInputHandler() {
	if (needRun) {
		needRun = false;
		void* ret = nullptr;
		pthread_join(readThread, &ret);
	}
	if (stream) {
		freeStream();
	}
	pa_xfree(sample_spec);
	if (channel_map) pa_xfree(channel_map);
	if (stream_name) pa_xfree(stream_name);
	if (client_name) pa_xfree(client_name);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

void PulseInputHandler::PS_CreateStream(const char* devName, const char* streamName) {
	createStream(devName, streamName);
}

void PulseInputHandler::PS_FreeStream() {
	freeStream();
}

void PulseInputHandler::PS_readDataSync(void* buf, size_t bytes) {
	readDataSync(buf, bytes);
}

void PulseInputHandler::createStream(const char* devName, const char *streamName) {
	stream_name = pa_xstrdup( streamName? streamName : "test-input");

	int error;
	stream = pa_simple_new(NULL,
			stream_name,
			PA_STREAM_RECORD,
			devName,
			stream_name,
			sample_spec,
			channel_map,
			NULL,
			&error);
	if (!stream) {
		printf("[%u:%lu] pa_simple_new failed: %s\n",
				getpid(), syscall(__NR_gettid), pa_strerror(error));
		return;
	}

	pthread_mutex_lock(&mutex);
	needRun = true;
	error = pthread_create(&readThread, NULL, readThreadFunc, this);
	if (error < 0) {
		printf("[%u:%lu] fail to create thread: \n", getpid(), syscall(__NR_gettid));
		pthread_mutex_unlock(&mutex);
		return;
	}
	pthread_cond_wait(&cond, &mutex);
	pthread_mutex_unlock(&mutex);
}

void* PulseInputHandler::readThreadFunc(void* data) {
	assert(data);
	auto pthis = static_cast<PulseInputHandler*>(data);
	
	pthread_mutex_lock(&pthis->mutex);
	pthread_cond_signal(&pthis->cond);
	pthread_mutex_unlock(&pthis->mutex);

	int error = 0;
	printf("[%u:%lu] Thread Started\n", getpid(), syscall(__NR_gettid));
	uint32_t tenMsSize = pthis->sample_spec->rate / 100;
	while (true) {
		uint8_t buf[tenMsSize * getFormatSize(pthis->sample_spec->format) * pthis->sample_spec->channels] = {0,};
		pthread_mutex_lock(&pthis->mutex);
		if (!pthis->needRun) { 
			pthread_mutex_unlock(&pthis->mutex);
			break;
		}
		if (pa_simple_read(pthis->stream, buf, sizeof(buf), &error) < 0) {
			printf("[%u:%lu] failt to read simple data (%s)\n",
					getpid(), syscall(__NR_gettid), pa_strerror(error));
		}
		else {
			gBuff.putData(buf, sizeof(buf));
			if (gBuff.getReadableSize() >= pthis->userReadSize) {
				pthread_cond_signal(&pthis->cond);
			}
		}
		pthread_mutex_unlock(&pthis->mutex);
		usleep(5000);
	}
	printf("[%u:%lu] Thread Exit\n", getpid(), syscall(__NR_gettid));
	return nullptr;
}


void PulseInputHandler::putData(uint8_t* buf, size_t size) {
}

void PulseInputHandler::freeStream() {
	if (stream) {
		if (readThread && needRun) {
			needRun = false;
			void *ret = nullptr;
			pthread_join(readThread, &ret);  // This will unref readThread;
			readThread = 0;
		}
		pa_simple_free(stream);
		stream = nullptr;
	}
}

void PulseInputHandler::readDataSync(void *buf, size_t bytes) {
	static uint32_t totalFeed = 0;
	pthread_mutex_lock(&mutex);
	if (gBuff.getReadableSize() < bytes) {
		userReadSize = bytes;
		pthread_cond_wait(&cond, &mutex);
	}
	if ( totalFeed > userRate * getFormatSize((pa_sample_format_t)userFormat) * 5) {
		printf("[%u:%lu] 5sec data recorded\n", getpid(), syscall(__NR_gettid));
		totalFeed = 0;
	}
	gBuff.getData(buf, bytes);
	totalFeed += bytes;
	pthread_mutex_unlock(&mutex);
}


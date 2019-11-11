#include "pulse-input-handler.h"
#include <iostream>
#include <pthread.h>
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

typedef struct mThread {
	pthread_t myTh;
	void* dat;
	bool isRun;
	mThread () {
		isRun = true; myTh = 0; dat = nullptr;
	};
	~mThread() {
		if (dat) delete dat;
	}
} mThread_t;

int fd = -1;

void* tReadFunc(void* data) {
	auto pData = static_cast<mThread_t*>(data);
	assert(pData);

	PulseInputHandler h = PulseInputHandler(48000, SAMPLE_S32LE, 1);
	h.PS_CreateStream("alsa_input.pci-0000_00_1f.3.analog-stereo", "testRec");
	fd = open("./rec.pcm", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	assert(fd >= 0);

	while(pData->isRun) {
		size_t bufsize = 1024*12;
		uint8_t buf[bufsize] = {0,};
		h.PS_readDataSync(buf, bufsize);
		int nWrite = write(fd, buf, bufsize);
		if (nWrite != bufsize) {
			cout << "not all data were written!!!" << endl;
		}
		usleep(5000);
	}
	h.PS_FreeStream();
	close(fd);
	return NULL;
}

int main()
{
	mThread_t* readT = new mThread_t;
	cout << "Test Starts\n" << endl;
	pthread_create(&readT->myTh, nullptr, tReadFunc, readT);
	while(1) {
		char c;
		cout << "Type x to Exit" << endl;
		cin >> c;
		if (c == 'x' || c == 'X') {
			readT->isRun = false;
			break;
		}
		usleep(500000);
	}
	void* ret = nullptr;
	pthread_join(readT->myTh, &ret);
	return 0;
}

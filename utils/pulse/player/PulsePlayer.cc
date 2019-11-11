#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "PulsePlayer.h"
#include <pulse/volume.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "<PPlayer> "

#include "logs.h"

PulsePlayer::PulsePlayer(int rate, int bitDepth, int channels, const char* dev) :
	mRate(rate),
	mBitDepth(bitDepth),
	mChannels(channels)
{
	ss.rate = mRate;
	ss.channels = mChannels;
	switch(mBitDepth) {
		case 8:
			ss.format = PA_SAMPLE_U8;
			break;
		case 16:
			ss.format = PA_SAMPLE_S16LE;
			break;
		case 24:
			ss.format = PA_SAMPLE_S24LE;
			break;
		case 32:
			ss.format = PA_SAMPLE_S32LE;
			break;
		default:
			ss.format = PA_SAMPLE_S16LE;
			break;
	}
	strDev = pa_xstrdup((dev)? dev : "default");
}

PulsePlayer::~PulsePlayer() {
	if (mPlayer) {
		int error;
		pa_simple_drain(mPlayer, &error);
		pa_simple_free(mPlayer);
		mPlayer = nullptr;
	}
	if (strDev)
		pa_xfree(strDev);
}

int PulsePlayer::PP_CreateStream() {
	return createStream();
}

void PulsePlayer::PP_DeleteStream() {
}

void PulsePlayer::PP_PushData(const void* data, const uint32_t bytes) {
	pushData(data, bytes);
}
void PulsePlayer::PP_Drain() {
	drain();
}

int PulsePlayer::createStream() {
	attr.maxlength = 15*882;
	attr.tlength   = 10 * 882;
	attr.prebuf    = 882*2;
	attr.minreq    = 882;
	attr.fragsize  = 0;
	pa_channel_map map;
	pa_channel_map_init_auto(&map, mChannels, PA_CHANNEL_MAP_DEFAULT);
	LOGS_I("channels=%d\n", map.channels);

	int error = 0;
	mPlayer = pa_simple_new(NULL, "psimple-player", PA_STREAM_PLAYBACK, strDev, "playback", &ss, NULL, &attr, &error);
	if (!mPlayer) {
		LOGS_E("## Fail to Create simple playstream (%s)\n", pa_strerror(error));
	}
	assert(mPlayer);
	return error;
}

void PulsePlayer::deleteStream() {
	if (mPlayer) {
		int error = 0;
		pa_simple_drain(mPlayer, &error);
		pa_simple_free(mPlayer);
		mPlayer = nullptr;
	}
}

void PulsePlayer::pushData(const void* data, const uint32_t bytes) {
	if (!mPlayer) return;
	int error;
	if ( 0 > pa_simple_write(mPlayer, data, bytes, &error))
		LOGS_E("## pa write fail(%s)\n", pa_strerror(error));
}

void PulsePlayer::drain() {
	if (!mPlayer) return;
	int error;
	if (0 > pa_simple_drain(mPlayer, &error))
		LOGS_E("## pa write fail(%s)\n", pa_strerror(error));
}

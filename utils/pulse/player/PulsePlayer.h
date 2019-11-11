/*******************************************
 * A simple pulse player instance
 * ffkaka
 ******************************************/

#ifndef __PULSE_SIMPLE_PLAYER__
#define __PULSE_SIMPLE_PLAYER__

#include <string>
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

class PulsePlayer {
	public:
		PulsePlayer(int rate = 48000, int bitDepth = 16, int channels = 1, const char* dev = nullptr);
		~PulsePlayer();

		int PP_CreateStream();
		void PP_DeleteStream();
		void PP_PushData(const void* data, const uint32_t bytes);
		void PP_Drain();

	private:
		int createStream();
		void deleteStream();
		void pushData(const void* data, const uint32_t bytes);
		void drain();

		int mRate;
		int mBitDepth;
		int mChannels;
		char* strDev = nullptr;

		pa_simple* mPlayer = nullptr;
		pa_sample_spec ss;
		pa_buffer_attr attr;
};

#endif //__PULSE_SIMPLE_PLAYER__

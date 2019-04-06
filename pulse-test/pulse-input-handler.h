#ifndef __PULSE_INPUT_HANDLER_H__
#define __PULSE_INPUT_HANDLER_H__


#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>
#include <pthread.h>

typedef enum in_read_format {
    SAMPLE_U8,
    SAMPLE_ALAW,
    /**< 8 Bit a-Law */

    SAMPLE_ULAW,
    /**< 8 Bit mu-Law */

    SAMPLE_S16LE,
    /**< Signed 16 Bit PCM, little endian (PC) */

    SAMPLE_S16BE,
    /**< Signed 16 Bit PCM, big endian */

    SAMPLE_FLOAT32LE,
    /**< 32 Bit IEEE floating point, little endian (PC), range -1.0 to 1.0 */

    SAMPLE_FLOAT32BE,
    /**< 32 Bit IEEE floating point, big endian, range -1.0 to 1.0 */

    SAMPLE_S32LE,
    /**< Signed 32 Bit PCM, little endian (PC) */

    SAMPLE_S32BE,
    /**< Signed 32 Bit PCM, big endian */

    SAMPLE_S24LE,
    /**< Signed 24 Bit PCM packed, little endian (PC). \since 0.9.15 */

    SAMPLE_S24BE,
    /**< Signed 24 Bit PCM packed, big endian. \since 0.9.15 */

    SAMPLE_S24_32LE,
    /**< Signed 24 Bit PCM in LSB of 32 Bit words, little endian (PC). \since 0.9.15 */

    SAMPLE_S24_32BE,
    /**< Signed 24 Bit PCM in LSB of 32 Bit words, big endian. \since 0.9.15 */

    SAMPLE_MAX,
    /**< Upper limit of valid sample types */

    SAMPLE_INVALID = -1
    /**< An invalid value */
} in_read_format_t;


/*
 * This Class will create a pulse simple audio stream and will provide method which can 
 *  get a data with sync way. 
 * Supported rate : 16000, 32000, 48000
 * Supported format : SAMPLE_S32LE, SAMPLE_S16LE
 * Supported channels : only mono channel is supported now
 *  !!! pingpong only support the mono/16K/SAMPLE_S16LE
 */

class PulseInputHandler {
	public:
		PulseInputHandler(int rate=48000, in_read_format_t format = SAMPLE_S32LE, int channels = 1);
		~PulseInputHandler();

		void PS_CreateStream(char* streamName = nullptr);
		void PS_FreeStream();
		void PS_readDataSync(void* buf, size_t bytes);

	private:
		char *stream_name;
		char *client_name;
		char *device;
		size_t latencyByte;
		pa_simple *stream;
		pa_sample_spec *sample_spec;
		pa_channel_map *channel_map;
		pthread_t readThread;
		bool needRun;
		uint32_t readBufSize;
		uint32_t feedBufSize;
		int userRate;
		in_read_format_t userFormat;

		void createStream(char* streamName);
		void freeStream();
		void readDataSync(void* buf, size_t bytes);
		static void* readThreadFunc(void* data);
};

#endif //__PULSE_INPUT_HANDLER_H__

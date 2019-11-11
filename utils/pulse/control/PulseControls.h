/***********************************************
 * The async pulse audio controller instance
 * ffkaka
 **********************************************/

#ifndef __PULSE_ASYNC_CONTROLLER__
#define __PULSE_ASYNC_CONTROLLER__


#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <pulse/pulseaudio.h>


class PulseAsyncCtrl {
	enum {
		PAC_ERROR_PULSE= -3,
		PAC_ERROR_NOT_READY= -2,
		PAC_ERROR_CONTEXT = -1,
		PAC_ERROR_NONE = 0
	};
	public:
		PulseAsyncCtrl();
		~PulseAsyncCtrl();

		/**
		 * @brief : Prepare pulse context
		 */
		int Prepare();

		/**
		 * @brief : Remove pulse context
		 */
		void UnPrepare();

		/**
		 * @brief : Set named sink mute
		 * @param name : a name string of sink
		 * return : 0 if no error else negative value
		 */
		int SetSinkMute(const char * name);

		/**
		 * @brief : Set named sink unmute
		 * @param name : a name string of sink
		 * return : 0 if no error else negative value
		 */
		int SetSinkUnMute(const char * name);

		/**
		 * @brief : Set named sink volume
		 * @param name : a name string of sink
		 * @param vol : a linear volume (0 -> 0%,  1.0 -> 100%)
		 * @param channels : Current Sink Channels(unsigend int)
		 * return : 0 if no error else negative value
		 */
		int SetSinkVolumeLin(const char * name, double vol, unsigned int channels = 2);

	private:
		char *client_name = NULL;
		char *device = NULL;
		size_t latencyByte;
		pa_context *context = NULL;
		pa_threaded_mainloop *mloop = NULL;
		pa_mainloop_api *mainloop_api = NULL;
		pa_sample_spec *sample_spec = NULL;
		pa_channel_map *channel_map = NULL;
		bool isReady = false;

		int prepare();
		void unPrepare();
		int setSinkMute(const char * name);
		int setSinkUnMute(const char * name);
		int setSinkVolumeLin(const char * name, double vol);

		static void context_state_callback(pa_context *c, void *userdata);
		static void time_event_callback(pa_mainloop_api *m, pa_time_event *e, const struct timeval *tv, void *userdata);
		static void context_drain_complete(pa_context *c, void *userdata);
};

#endif //__PULSE_ASYNC_CONTROLLER__

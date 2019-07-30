#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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
#include <PulseAsyncCtrl.h>
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "<PCtrl> "
#include "logs.h"

PulseAsyncCtrl::PulseAsyncCtrl() {
	client_name = nullptr;
	device = nullptr;
	latencyByte = 0;
	context = nullptr;
	mloop = nullptr;
	mainloop_api = nullptr;
	sample_spec = nullptr;
	channel_map = nullptr;
}

PulseAsyncCtrl::~PulseAsyncCtrl() {
	unPrepare();
}

int PulseAsyncCtrl::Prepare() {
	return this->prepare();
}

int PulseAsyncCtrl::prepare(void) {

	sample_spec = pa_xnew0(pa_sample_spec, 1);

	/* Set up a new main loop */
	if (!(mloop = pa_threaded_mainloop_new())) {
		LOGS_E("pa_threaded_mainloop_new() failed.\n");
		pa_xfree(sample_spec);
		sample_spec = NULL;
		return PAC_ERROR_PULSE;
	}

	mainloop_api = pa_threaded_mainloop_get_api(mloop);

	/*
	int r = pa_signal_init(mainloop_api);
	assert(r == 0);
	*/

	/* Run the threaded main loop */
	int ret = 0;
	if (ret = pa_threaded_mainloop_start(mloop) < 0) {
		LOGS_E("pa_threaded_mainloop_start() failed. %s\n", pa_strerror(ret));
		pa_context_unref(context);
		pa_threaded_mainloop_free(mloop);
		pa_xfree(sample_spec);
		context = NULL;
		mloop = NULL;
		sample_spec = NULL;
		return PAC_ERROR_CONTEXT;
	}

	/* Create a new connection context */
	if (!(context = pa_context_new(mainloop_api, client_name))) {
		LOGS_E("pa_context_new() failed.\n");
		pa_threaded_mainloop_free(mloop);
		pa_xfree(sample_spec);
		mloop = NULL;
		sample_spec = NULL;
		return PAC_ERROR_CONTEXT;
	}

	pa_context_set_state_callback(context, context_state_callback, this);

	/* Connect the context */
	if (pa_context_connect(context, NULL, (pa_context_flags_t)0, NULL) < 0) {
		LOGS_E("pa_context_connect() failed: %s\n", pa_strerror(pa_context_errno(context)));
		pa_context_unref(context);
		pa_threaded_mainloop_free(mloop);
		pa_xfree(sample_spec);
		context = NULL;
		mloop = NULL;
		sample_spec = NULL;
		return PAC_ERROR_CONTEXT;
	}

	return 0;
}

int PulseAsyncCtrl::SetSinkMute(const char * name) {
	if (!isReady) return PAC_ERROR_NOT_READY;
	LOGS_D("Set Sink[%s] Mute On\n", name);
	pa_operation* opr = NULL;
	opr = pa_context_set_sink_mute_by_name(context,
			name,
			1,
			NULL,
			NULL);
	pa_operation_unref(opr);
	return 0;
}

int PulseAsyncCtrl::SetSinkUnMute(const char * name) {
	if (!isReady) return PAC_ERROR_NOT_READY;
	LOGS_D("Set Sink[%s] Mute Off\n", name);
	pa_operation* opr = NULL;
	opr = pa_context_set_sink_mute_by_name(context,
			name,
			0,
			NULL,
			NULL);
	pa_operation_unref(opr);
	return 0;
}

int PulseAsyncCtrl::SetSinkVolumeLin(const char * name, double vol, unsigned int channels) {
	if (!isReady) return PAC_ERROR_NOT_READY;
	pa_operation* opr = NULL;
	pa_cvolume pvol;
	pa_volume_t voltype = pa_sw_volume_from_linear(vol);
	if (!pa_channels_valid(channels)) {
		
		channels = 2;
	}
	pa_cvolume_set(&pvol, channels, voltype);
	opr = pa_context_set_sink_volume_by_name(context,
			name,
			&pvol,
			NULL,
			NULL);
	pa_operation_unref(opr);
	return 0;
}

void PulseAsyncCtrl::UnPrepare() {
	unPrepare();
}

void PulseAsyncCtrl::context_state_callback(pa_context *c, void *userdata) {
	assert(userdata);
	auto pthis = static_cast<PulseAsyncCtrl*>(userdata);

	switch (pa_context_get_state(c)) 
	{
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
			break;

		case PA_CONTEXT_READY: 
			{
				assert(c);
				pthis->isReady = true;
				LOGS_I("PA Context is Ready\n");
				break;
			}

		case PA_CONTEXT_TERMINATED:
			pthis->isReady = false;
			LOGS_I("PA Context is Terminated\n");
			break;

		case PA_CONTEXT_FAILED:
		default:
			pthis->isReady = false;
			LOGS_E("Connection failure: %s\n", pa_strerror(pa_context_errno(c)));
			break;
	}

	return;
}

void PulseAsyncCtrl::unPrepare(void)
{
	if (context) {
		pa_operation* opr;
		LOGS_D("Disconnect Context\n");
		if ( !(opr=pa_context_drain(context, NULL, this)) ) {
			pa_context_disconnect(context);
		}
		else {
			pa_operation_unref(opr);
		}
		pa_context_unref(context);
		context = NULL;
	}

	/*
	int ret = 0;
	mainloop_api->quit(mainloop_api, ret);
	*/

	//pa_signal_done();
	if (mloop) {
		LOGS_D("Stop Threaded Main loop\n");
		pa_threaded_mainloop_stop(mloop);
	}
	if (mloop) {
		LOGS_D("Free Threaded Main loop\n");
		pa_threaded_mainloop_free(mloop);
		mloop = NULL;
	}
	if (sample_spec) {
		pa_xfree(sample_spec);
		sample_spec = NULL;
	}
}

void PulseAsyncCtrl::context_drain_complete(pa_context *c, void *userdata) {
	LOGS_D("Disconnect Context in Callback\n");
	pa_context_disconnect(c);
}

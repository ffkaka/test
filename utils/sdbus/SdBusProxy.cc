#include <assert.h>
#include "SdBusProxy.h"

#ifdef LOG_TAG
#endif
#define LOG_TAG "<sd-prox>: "
#include "logs.h"

_SdBusProxy_::_SdBusProxy_() {
	vtable = NULL;
	bus = NULL;
	slot = NULL;
	objPath = "";
	objIf = "";
	run = false;
	pthread_t eventThread = 0;
}

_SdBusProxy_::~_SdBusProxy_() {
}

void _SdBusProxy_::SetVtTable(const char* obj_path,
		const char* interface,
		const sd_bus_vtable *vt,
		void* userdata)
{
	assert(obj_path);
	assert(interface);
	assert(vt);

	objPath = obj_path;
	objIf = interface;
	vtable = vt;
	userData = userdata;
}

int _SdBusProxy_::StartServer() {
	return startServer();
}

void _SdBusProxy_::StopServer() {
	stopServer();
}

int _SdBusProxy_::startServer() {
	int r;
	int threadId = -1;
	if (!objPath.compare("") || !objIf.compare("")) {
		LOGS_E("Set the VCTable First");
		return BUS_PROX_ERROR_LONG_VTABLE;
	}
	r = sd_bus_open_user(&bus);
	if (r < 0) {
		LOGS_E("Failed to connect to user bus: %s\n", strerror(-r));
		goto start_fail;
	}

	/* Install the object */
	r = sd_bus_add_object_vtable(bus,
			&slot,
			objPath.c_str(),  /* object path */
			objIf.c_str(),   /* interface name */
			vtable,
			userData);
	if (r < 0) {
		LOGS_E("Failed to issue method call: %s\n", strerror(-r));
		goto start_fail;
	}

	/* Take a well-known service name so that clients can find us */
	r = sd_bus_request_name(bus, objIf.c_str(), 0);
	if (r < 0) {
		LOGS_E("Failed to acquire service name: %s\n", strerror(-r));
		goto start_fail;
	}

	run = true;
	threadId = pthread_create(&eventThread, NULL, evtThreadFunc, this);
	if (0 != threadId) {
		LOGS_E("Fail to Create Thread func: %d\n", threadId);
		r = BUS_PROX_ERROR_THREAD_FAIL;
		goto start_fail;
	}

	return 0;

start_fail:
	run = false;
	if (slot) {
		sd_bus_slot_unref(slot);
		slot = NULL;
	}
	if (bus) {
		sd_bus_unref(bus);
		bus = NULL;
	}
	return r;
}

void _SdBusProxy_::stopServer() {
	if (run) {
		run = false;
		void* ret = NULL;
		LOGS_I("Wait for joining Thread\n");
		pthread_join(eventThread, &ret);
		eventThread = 0;
	}

	if (slot) {
		sd_bus_slot_unref(slot);
		slot = NULL;
	}
	if (bus) {
		sd_bus_unref(bus);
		bus = NULL;
	}
}

void* _SdBusProxy_::evtThreadFunc(void* data) {
	assert(data);
	auto pthis = static_cast<_SdBusProxy_*>(data);

	LOGS_I("Start D-Bus event loop\n");
	int r;
	while(pthis->run) {
		r = sd_bus_process(pthis->bus, NULL);
		if (r < 0) {
			LOGS_E("Failed to process requests: %d\n", r);
			break;
		}

		if (r == 0) {
			r = sd_bus_wait(pthis->bus, (uint64_t) 1000000);
			if (r < 0) {
				LOGS_E("Failed to wait: %d\n", r);
				break;
			}
			continue;
		}
	}
	sd_bus_flush(pthis->bus);
	LOGS_I("Disconnect Bus\n");
	sd_bus_close(pthis->bus);
	LOGS_I("Stop Event Loop\n");
	return NULL;
}

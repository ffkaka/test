#ifndef ___SD_BUS_PROXY___
#define ___SD_BUS_PROXY___

#include <iostream>
#include <systemd/sd-bus.h>
#include <pthread.h>
#include <string>

/************************************************
 * A simple SD bus Server with vector table
 * This will make a event thread for a specified sdbus
 * ffkaka
 * *********************************************/

class _SdBusProxy_ {
	public:
		enum sdbus_proxy_error {
			BUS_PROX_ERROR_THREAD_FAIL = -98,
			BUS_PROX_ERROR_LONG_VTABLE = -99,
			BUS_PROX_ERROR_NONE = 0
		};

		_SdBusProxy_();
		~_SdBusProxy_();

		/**
		 * @brief - Set Vector Table for the methods, properties or signals
		 * @param obj_path : object path for dbus (ex : /org/freedesktop/Properties)
		 * @param interface : object interface for dbus (ex : org.freedesktop.properties)
		 * @param vt : sd_bus_vtable for this Sd Bus Proxy
		 * @param userdata : user data pointer
		 */
		void SetVtTable(const char* obj_path = NULL,
				const char* interface = NULL,
				const sd_bus_vtable *vt = NULL,
				void* userdata = NULL);
		/**
		 * @brief - Start a dbus event loop and dbus message will be handled
		 * on this loop thread
		 * @return : 0 if there is no error, or negative value in fail cases.
		 */
		int StartServer(void);
		
		/**
		 * @brief - Stop the dbus event log and disconnect user bus
		 */
		void StopServer(void);

		const sd_bus_vtable *vtable = NULL;

	private:
		sd_bus *bus = NULL;
		sd_bus_slot *slot = NULL;
		void* userData = NULL;
		bool run = false;
		std::string objPath;
		std::string objIf;
		pthread_t eventThread;
		int startServer();
		void stopServer();
		static void* evtThreadFunc(void* data);
};

#endif // ___SD_BUS_PROXY___

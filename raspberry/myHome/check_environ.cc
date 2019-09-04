/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "scd30.h"
#include "time.h"

/* TO USE CONSOLE OUTPUT (printf) AND WAIT (sleep/usleep) PLEASE ADAPT THEM TO
 * YOUR PLATFORM, OR REMOVE THE DEFINES BELOW TO USE AS-IS.
 */
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep, usleep */
#include <thread>
#include <chrono>
#include <iostream>
//#define printf(...)
//#define sleep(...)
//#define usleep(...)
#define STR_TIME_FORMAT      "%y%m%d-%H:%M:%S"

struct _user_data_ {
	bool isRepeat;
	int16_t Sec;
};

static void checkTimer(void* data)
{
	auto udata = static_cast<struct _user_data_*>(data);
	bool run = true;
    float32_t co2_ppm, temperature, relative_humidity;
	uint16_t data_ready, ret;
	while (run) {
		/* Measure co2, temperature and relative humidity and store into
		 * variables.
		 */
		ret = scd30_get_data_ready(&data_ready);
		if (ret == STATUS_OK) {
			if (data_ready) {
				ret = scd30_read_measurement(&co2_ppm, &temperature,
						&relative_humidity);
				if (ret != STATUS_OK) {
					printf("error reading measurement\n");

				} else {
					struct timespec tp;
					struct tm   tm_now;
					char buf[32] = {0};
					clock_gettime(CLOCK_REALTIME, &tp);
					localtime_r((time_t *)&tp.tv_sec, &tm_now);
					strftime(buf, 32, STR_TIME_FORMAT, &tm_now);
					printf("[%s] co2 concentr: %4.2f ppm, "
							"Temp: %0.2f C, "
							"Humidity: %0.2f %%RH\n",
							buf,co2_ppm,
							temperature,
							relative_humidity);
					run = false;
				}
			} else {
				//printf("measurement not ready\n");
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				continue;
			}

		} else {
			//printf("error reading data ready flag\n");
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	if (udata->isRepeat) {
		std::this_thread::sleep_for(std::chrono::seconds(udata->Sec));
		std::thread th(checkTimer, udata); 
		th.detach();
	}
}


int main(void) {
    float32_t co2_ppm, temperature, relative_humidity;
    uint16_t data_ready;
    int16_t ret;
    int16_t interval_in_seconds = 3;
	//static int16_t isRepeat = 1;
	struct _user_data_ udata = {true, interval_in_seconds};

    /* Initialize I2C */
    sensirion_i2c_init();

    /* Busy loop for initialization, because the main loop does not work without
     * a sensor.
     */
    while (scd30_probe() != STATUS_OK) {
        printf("SCD30 sensor probing failed\n");
		std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    printf("SCD30 sensor probing successful\n");

    scd30_set_measurement_interval(1);
	std::this_thread::sleep_for(std::chrono::microseconds(20000));
    scd30_start_periodic_measurement(0);

	std::thread th(checkTimer, &udata);
	th.detach();

    while (1) {
		char in;
		std::cin >> in;
		if (in == 'x' || in == 'X') {
			udata.isRepeat = false;
			break;
		}
	}
    scd30_stop_periodic_measurement();
    return 0;
}

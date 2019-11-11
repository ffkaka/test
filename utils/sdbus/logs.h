#ifndef __KAKAO_LOG_UTILS___
#define __KAKAO_LOG_UTILS___

#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/syslog.h>
#include <stdio.h>

#ifndef LOG_TAG
#define LOG_TAG "<kautils> "
#endif

#define LOGC(fmt, ...)  printf("[%u:%lu]" fmt, getpid(), syscall(__NR_gettid), ##__VA_ARGS__)
#define LOGS_E(fmt, ...)  syslog(3, "[%u:%lu]" LOG_TAG fmt, getpid(), syscall(__NR_gettid), ##__VA_ARGS__)
#define LOGS_W(fmt, ...)  syslog(4, "[%u:%lu]" LOG_TAG fmt, getpid(), syscall(__NR_gettid), ##__VA_ARGS__)
#define LOGS_D(fmt, ...)  syslog(7, "[%u:%lu]" LOG_TAG fmt, getpid(), syscall(__NR_gettid), ##__VA_ARGS__)
#define LOGS_I(fmt, ...)  syslog(6, "[%u:%lu]" LOG_TAG fmt, getpid(), syscall(__NR_gettid), ##__VA_ARGS__)

#endif //__KAKAO_LOG_UTILS___

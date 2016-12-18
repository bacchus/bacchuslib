#pragma once

#include <stdarg.h>
#include <cstdio>
#include <iostream>
#include <cstring>

#define BCC_LOG_SIZE 1024

static inline int bccLogPrint(const char* level, const char* tag, const char* file, int line, const char* message) {
    std::cout << level << "|" << tag << " " << "| " << file << ":" << line << " : " << message << std::endl;
    return 0;
}

static int bcc_log_print_impl(const char* prio, const char* tag, const char* file, int line, const char* fmt, ...) {
    va_list ap;
    char buf[BCC_LOG_SIZE];

    va_start(ap, fmt);
    vsnprintf(buf, BCC_LOG_SIZE, fmt, ap);
    va_end(ap);

    return bccLogPrint(prio, tag, file, line, buf);
}

#define BCC_LOG(LOG_LEVEL, LOG_TAG, fmt...) bcc_log_print_impl(#LOG_LEVEL, LOG_TAG, __FILE__, __LINE__, fmt)

#define ALOG(LOG_LEVEL, LOG_TAG, ...) BCC_LOG(LOG_LEVEL, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) BCC_LOG(LOG_ERROR, "BACCHUS", __VA_ARGS__)
#define LOG_FATAL(...) BCC_LOG(LOG_FATAL, "BACCHUS", __VA_ARGS__)
#define LOG_DEBUG(...) BCC_LOG(LOG_DEBUD, "BACCHUS", __VA_ARGS__)
#define LOG_INFO(...) BCC_LOG(LOG_INFO, "BACCHUS", __VA_ARGS__)
#define LOG_ERROR(...) BCC_LOG(LOG_ERROR, "BACCHUS", __VA_ARGS__)

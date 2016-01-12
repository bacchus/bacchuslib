#pragma once

#define LNEND(X) std::cout << std::endl
#define PRINT(X) std::cout << #X": " << X << std::endl
#define BCC_LOG_TAG "BACCHUSLIB"

#ifdef ANDROID
#include <strings.h>
#include <android/log.h>
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, BCC_LOG_TAG, __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, BCC_LOG_TAG, __VA_ARGS__)

#else // ANDROID
#include <cstdarg>
#include <iostream>

namespace bacchus {

inline int bccLogPrint(const char* level, const char* tag, const char* file, int line, const char* message) {
    std::cout << level << "|" << tag << " " << "| " << file << ":" << line << " : " << message << std::endl;
    return 0;
}

inline int bcc_log_print_impl(const char* prio, const char* tag, const char* file, int line, const char* fmt, ...) {
    va_list ap;
    const size_t BCC_LOG_SIZE = 1024;
    char buf[BCC_LOG_SIZE];

    va_start(ap, fmt);
    vsnprintf(buf, BCC_LOG_SIZE, fmt, ap);
    va_end(ap);

    return bccLogPrint(prio, tag, file, line, buf);
}

} // namespace bacchus

#define BCC_LOG(LOG_LEVEL, LOG_TAG, fmt...) bcc_log_print_impl(#LOG_LEVEL, LOG_TAG, __FILE__, __LINE__, fmt)
#define ALOG(LOG_LEVEL, LOG_TAG, ...) BCC_LOG(LOG_LEVEL, LOG_TAG, __VA_ARGS__)

#define ALOGE(...) BCC_LOG(LOG_ERROR, BCC_LOG_TAG, __VA_ARGS__)
#define ALOGD(...) BCC_LOG(LOG_DEBUD, BCC_LOG_TAG, __VA_ARGS__)

#define LOG_DEBUG(...)  BCC_LOG(LOG_DEBUD, BCC_LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...)   BCC_LOG(LOG_INFO, BCC_LOG_TAG, __VA_ARGS__)
#define LOG_ERROR(...)  BCC_LOG(LOG_ERROR, BCC_LOG_TAG, __VA_ARGS__)
#define LOG_FATAL(...)  BCC_LOG(LOG_FATAL, BCC_LOG_TAG, __VA_ARGS__)

#endif // ANDROID

#ifdef BCC_DEBUG_GLERROR

static int gLogOnce = true;

#ifdef BCC_CHECK_GL_ERROR
#define CHECK_GL_ERROR(DRAW_COMMAND) DRAW_COMMAND; {    \
    if (gLogOnce) LOG_INFO(#DRAW_COMMAND);              \
    GLenum glStatus = GL_NO_ERROR;                      \
    while ((glStatus = glGetError()) != GL_NO_ERROR) {  \
    char errstr[50];                                    \
    switch (glStatus) {                                 \
    case GL_INVALID_ENUM:                               \
    sprintf(errstr, "  intvalid enum! ");              \
    break;                                              \
    case GL_INVALID_VALUE:                              \
    sprintf(errstr, "  invalid value! ");             \
    break;                                              \
    case GL_INVALID_OPERATION:                          \
    sprintf(errstr, "  invalid operation! ");         \
    break;                                              \
    case GL_OUT_OF_MEMORY:                              \
    sprintf(errstr, "  out of memory! ");               \
    break;                                              \
    }                                                   \
    LOG_INFO("Check GL error " #DRAW_COMMAND ": %s 0x%x\n", errstr, glStatus);  \
    }}
#else // BCC_CHECK_GL_ERROR
#define CHECK_GL_ERROR(DRAW_COMMAND) DRAW_COMMAND; { if (gLogOnce) LOG_INFO(#DRAW_COMMAND); }
#endif // BCC_CHECK_GL_ERROR

#else // BCC_DEBUG_GLERROR
#define CHECK_GL_ERROR(DRAW_COMMAND) DRAW_COMMAND;
#endif // BCC_DEBUG_GLERROR

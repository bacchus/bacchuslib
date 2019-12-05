#pragma once

#define LNEND(X) std::cout << std::endl
#define PRINT(X) std::cout << #X": " << X << std::endl

#ifdef ANDROID
#include <strings.h>
#include <android/log.h>
#define BCC_LOG_TAG "BACCHUSLIB"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, BCC_LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, BCC_LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, BCC_LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, BCC_LOG_TAG, __VA_ARGS__)

#else // ANDROID
#include <cstdarg>
#include <iostream>

namespace bacchus {

inline int bccLogPrint(const char* level, const char* file, int line, const char* message) {
    std::cout << level << "| " << file << ":" << line << ": " << message << std::endl;
    return 0;
}

inline int bcc_log_print_impl(const char* prio, const char* file, int line, const char* fmt, ...) {
    va_list ap;
    const size_t BCC_LOG_SIZE = 1024;
    char buf[BCC_LOG_SIZE];

    va_start(ap, fmt);
    vsnprintf(buf, BCC_LOG_SIZE, fmt, ap);
    va_end(ap);

    return bccLogPrint(prio, file, line, buf);
}

} // namespace bacchus

#define BCC_LOG_IMPL(LOG_LEVEL, fmt...) bcc_log_print_impl(LOG_LEVEL, __FILE__, __LINE__, fmt)
#define BCC_LOG(LOG_LEVEL, ...) BCC_LOG_IMPL(LOG_LEVEL, __VA_ARGS__)

#define LOGI(...)   BCC_LOG("I", __VA_ARGS__)
#define LOGD(...)   BCC_LOG("D", __VA_ARGS__)
#define LOGW(...)   BCC_LOG("W", __VA_ARGS__)
#define LOGE(...)   BCC_LOG("E", __VA_ARGS__)

#endif // ANDROID

#ifdef BCC_DEBUG_GLERROR

static int gLogOnce = true;

#ifdef BCC_CHECK_GL_ERROR
#define CHECK_GL_ERROR(DRAW_COMMAND)                    \
    DRAW_COMMAND; {                                     \
    if (gLogOnce) LOGI(#DRAW_COMMAND);                  \
    GLenum glStatus = GL_NO_ERROR;                      \
    while ((glStatus = glGetError()) != GL_NO_ERROR) {  \
    char errstr[50];                                    \
    switch (glStatus) {                                 \
    case GL_INVALID_ENUM:                               \
    sprintf(errstr, "  intvalid enum! ");               \
    break;                                              \
    case GL_INVALID_VALUE:                              \
    sprintf(errstr, "  invalid value! ");               \
    break;                                              \
    case GL_INVALID_OPERATION:                          \
    sprintf(errstr, "  invalid operation! ");           \
    break;                                              \
    case GL_OUT_OF_MEMORY:                              \
    sprintf(errstr, "  out of memory! ");               \
    break; }                                            \
    LOGE("Check GL error " #DRAW_COMMAND ": %s 0x%x\n"  \
    , errstr, glStatus); }}

#else // BCC_CHECK_GL_ERROR
#define CHECK_GL_ERROR(DRAW_COMMAND)                    \
    DRAW_COMMAND; { if (gLogOnce) LOGI(#DRAW_COMMAND); }

#endif // BCC_CHECK_GL_ERROR

#else // BCC_DEBUG_GLERROR
#define CHECK_GL_ERROR(DRAW_COMMAND) DRAW_COMMAND;
#endif // BCC_DEBUG_GLERROR

#include <iostream>
#include <vector>
#include <iomanip>

namespace bacchus {

template<class T>
inline std::ostream& operator <<(std::ostream& os, const std::vector<T>& vec) {
    for (auto val: vec) {
        os << std::setw(6)
           << std::setprecision(3)
           << std::setiosflags(std::ios_base::showpoint)
           << std::setiosflags(std::ios_base::fixed)
           << std::setiosflags(std::ios_base::skipws)
           << val << " ";
    }
    return os << std::endl;
}

} // namespace bacchus

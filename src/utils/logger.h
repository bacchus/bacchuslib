#pragma once

#define LNEND(X) std::cout << std::endl
#define PRINT(X) std::cout << #X": " << X << std::endl

#ifdef ANDROID
#include <strings.h>
#include <android/log.h>
#define BCC_LOG_TAG "BACCHUSLIB"
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, BCC_LOG_TAG, __VA_ARGS__)
//#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, BCC_LOG_TAG, __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, BCC_LOG_TAG, __VA_ARGS__)

#else // ANDROID
#include <cstdarg>
#include <iostream>
#include <libgen.h>

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

#define LOGD(...)   BCC_LOG("D", __VA_ARGS__)
#define LOGI(...)   BCC_LOG("I", __VA_ARGS__)
#define LOGE(...)   BCC_LOG("E", __VA_ARGS__)

#define ALOGD(...)   LOGD(__VA_ARGS__)
#define ALOGI(...)   LOGI(__VA_ARGS__)
#define ALOGE(...)   LOGE(__VA_ARGS__)

#endif // else ANDROID


inline int64_t bcc_debug_gettime_ns() {
    timespec t;
    int ret = clock_gettime(CLOCK_MONOTONIC, &t);

    if (ret)
        return -1;

    const int64_t billion = 1000000000;
    return (int64_t)t.tv_nsec + (int64_t)t.tv_sec * billion;
}

inline void bcc_print(const char* format, ...) {
    char buff[4096];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    //printf("%s\n", buff);
    //ALOGD("%s", buff);
    std::cout << buff << std::endl;
    va_end(args);
}

__attribute__((format(printf, 5, 6))) inline void bcc_debug_print(const char* prefix,
                                                           const char* func,
                                                           const char* file, int line,
                                                           const char* format, ...) {
    char buff[4096];
    va_list args;
    va_start(args, format);
    int n = sprintf(buff, "%s:%s():%s:%d:", prefix, func, basename(const_cast<char*>(file)), line);
    vsprintf(buff + n, format, args);
    bcc_print("%s", buff);
    va_end(args);
}

#define bcc_debug_error(...)                                                    \
    do {                                                                        \
        bcc_debug_print("ERROR", __func__, __FILE__, __LINE__, __VA_ARGS__);    \
    } while (0)

#define bcc_debug_warn(...)                                                     \
    do {                                                                        \
        bcc_debug_print("WARNING", __func__, __FILE__, __LINE__, __VA_ARGS__);  \
    } while (0)

#define bcc_debug_log(...)                                                      \
    do {                                                                        \
        bcc_debug_print("LOG", __func__, __FILE__, __LINE__, __VA_ARGS__);      \
    } while (0)

#define CHECK_RES_FATAL(ret)                                                    \
    do {                                                                        \
        if ((ret) < 0) {                                                        \
            bcc_debug_error("[%s]: failed with error: %s", #ret, strerror(-ret)); \
            return ret;                                                         \
        }                                                                       \
    } while (0)

#define CHECK_RES_WARN(ret)                                                     \
    do {                                                                        \
        if ((ret) < 0) {                                                        \
            bcc_debug_warn("[%s]: failed with error: %s", #ret, strerror(-ret));  \
        }                                                                       \
    } while (0)

#define CHECK_COND_FATAL(cond)                                                  \
    do {                                                                        \
        if (!(cond)) {                                                          \
            bcc_debug_error("check [%s] failed; error: %s"                      \
                , #cond, strerror(-errno));                                     \
            return errno;                                                       \
        }                                                                       \
    } while (0)

#define CHECK_COND_WARN(cond)                                                   \
    do {                                                                        \
        if (!(cond)) {                                                          \
            bcc_debug_warn("check [%s] failed; error: %s"                       \
                , #cond, strerror(-errno));                                     \
        }                                                                       \
    } while (0)


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

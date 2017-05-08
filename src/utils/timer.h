#pragma once

#include <ctime>
#include <cstdio>
#include <iostream>
#include <cmath>

namespace bacchus {

class Timer {
public:
    Timer() {}
    void start() { start_ms = now_ms(); }
    double ms() { return now_ms() - start_ms; }
    double sec() { return ms()/1000.0; }

private:
    double now_ms() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (ts.tv_sec*1000.0) + (ts.tv_nsec/1000000.0);
    }

    double start_ms = 0.0;
};

class ScopedTimer {
public:
    ScopedTimer(const char* name=""): name(name) {
        timer.start();
    }

    ~ScopedTimer() {
        double elapsed_ms = timer.ms();
        char tmp[64];
        sprintf(tmp,"Timer %s: %.4f ms\n", name, elapsed_ms);
        std::cout << tmp << std::endl;
    }

private:
    const char* name;
    Timer timer;
};

class ScopedMeanTimer {
public:
    static void reset(int count = 0) {
        if (count > 0) max_cycles = count;
        cur_time = 0;
        cur_cycles = 0;
    }

    static double get_totalt() { return total_time; }
    static int get_totaln() { return total_cycles; }

    ScopedMeanTimer(const char* name=""): name(name) {
        timer.start();
    }

    ~ScopedMeanTimer() {
        double elapsed_ms = timer.ms();
        cur_time += elapsed_ms;
        ++cur_cycles;
        total_time += elapsed_ms;
        ++total_cycles;
        if (cur_cycles > max_cycles) {
            char tmp[64];
            sprintf(tmp,"Timer %s: %.4f ms\n", name, cur_time/(double)cur_cycles);
            std::cout << tmp << std::endl;
            reset();
        }
    }

private:
    const char* name;
    static double total_time;
    static int total_cycles;
    static double cur_time;
    static int cur_cycles;
    static int max_cycles;
    Timer timer;
};

class FpsTimer {
public:
    FpsTimer() {}

    void begin() {
        m_timer.start();
    }

    void end() {
        ++fps_dn;
        if (fps_dn >= fps_maxn) {
            double t2 = m_timer.sec();
            cur_fps = fps_dn/t2;
            std::cout << (t2/fps_dn) << " sec, fps: " << cur_fps << std::endl;
            fps_dn = 0;
        }
    }

    float get_fps() const {
        return cur_fps;
    }

private:
    Timer m_timer;
    int fps_maxn = 30;
    int fps_dn = 0;
    double t1 = 0;
    float cur_fps = 0;
};

//TODO: generate_timed_filename not tested
//inline const char* generate_timed_filename() {
//    time_t rawtime;
//    struct tm * timeinfo;
//    char buf[10];
//    time(&rawtime);
//    timeinfo = localtime(&rawtime);
//    strftime(buf, 10, "%I_%M.txt", timeinfo);
//    return buf;
//}

//#include <sys/time.h>
//double getTimeMsec() {
//    timeval end;
//    gettimeofday(&end, nullptr);
//    return (end.tv_sec - timeMark.tv_sec)*1000.0
//         + (end.tv_usec - timeMark.tv_usec)/1000.0;
//}

} // namespace bacchus

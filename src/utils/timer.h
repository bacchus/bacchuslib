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
    double sec() { return ms()/1000.f; }

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
        t1 = m_timer.ms();
    }

    void end() {
        float t2 = m_timer.ms();
        fps_dt += t2 - t1;
        ++fps_dn;
        if (fps_dn >= fps_maxn) {
            cur_fps = 1000.f*fps_dn/fps_dt;
            std::cout << fps_dt/fps_dn << " ms, fps: " << cur_fps << std::endl;
            fps_dt = 0;
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
    float fps_dt = 0;
    float t1 = 0;
    float cur_fps = 0;
};

} // namespace bacchus

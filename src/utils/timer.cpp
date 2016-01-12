#include "timer.h"

double bacchus::ScopedMeanTimer::total_time = 0;
int bacchus::ScopedMeanTimer::total_cycles = 0;
double bacchus::ScopedMeanTimer::cur_time = 0;
int bacchus::ScopedMeanTimer::cur_cycles = 0;
int bacchus::ScopedMeanTimer::max_cycles = 100;

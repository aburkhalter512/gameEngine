#include "util/msTimer.h"

struct timespec start_msTimer()
{
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    return start;
}

uint64_t diff_msTimer(struct timespec* startTime)
{
    struct timespec end;
    struct timespec diff;

    clock_gettime(CLOCK_REALTIME, &end);

    diff.tv_sec = end.tv_sec - startTime->tv_sec;
    diff.tv_nsec = end.tv_nsec - startTime->tv_nsec;

    return diff.tv_sec * 1000 + diff.tv_nsec / 1000000;
}

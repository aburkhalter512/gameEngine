#pragma once

#include <stdint.h>
#include <time.h>

struct timespec start_msTimer();
uint64_t diff_msTimer(struct timespec* startTime);


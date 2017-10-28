#pragma once

#include <stdio.h>
#include "engine/math/vec.h"

typedef struct _polygon polygon;

#define SWAP(x, y) do { typeof(x) __SWAP = x; x = y; y = __SWAP; } while (0)
#define MAX(x, y) (x >= y ? x : y)

void print_vec2f(FILE* file, vec2f* v);

int sprint_vec2f(char* result, vec2f* v);
int sprint_polygon(char* result, polygon* p);

#include "engine/util.h"

#include "engine/math/polygon.h"
#include "engine/math/vec.h"

#include <stdio.h>
#include <stdbool.h>

void print_vec2f(FILE* file, vec2f* v)
{
    if (!file || !v)
    {
        return;
    }

    fprintf(file, "(%.3f, %.3f)", GET_X(*v), GET_Y(*v));
}

int sprint_vec2f(char* result, vec2f* v)
{
    return sprintf(result, "(%.3f, %.3f)", GET_X(*v), GET_Y(*v));
}

int sprint_polygon(char* result, polygon* p)
{
    result[0] = '\0';
    int offset = 0;
    for (int i = 0; i < p->vertexCount; i++)
    {
        offset += sprintf(&result[offset], " [%d]", i);
        offset += sprint_vec2f(&result[offset], &(p->vertices[i]));
    }

    return offset;
}

#pragma once

#include <stdbool.h>

#include "engine/math/vec.h"

typedef struct _polygon
{
    vec2f* vertices;
    int vertexCount;
} polygon;

/*
Allocates and initializes memory for a polygon

Arguments
    polygon* poly: The polygon to initialize

    int vertexCount: The number of vertices on the polygon

Returns
    Returns true if the polygon was successfully initialized, false if not
*/
bool create_polygon(polygon* poly, int vertexCount);

/*
Frees and de-initializes memory for a polygon. The pointer to the polygon
itself is not freed

Arguments
    polygon* poly: The polygon to de-initialize

Returns
    Returns true if the polygon was successfully de-initialized, false if not.
*/
bool free_polygon(polygon* poly);

/*
Translates a polygon with a vec2f

Runs in O(n) time

Arguments
    polygon* in: The polygon whose vertices will be the base of translation

    polygon* out: The polygon whose vertices will contain the result of the translation

    vec2f translation: The vector to translate the polygon by

Returns
    Returns false if the polygon could not be translated. This can happen
    if either polygons are NULL ptrs or if the vertex counts don't match
*/
bool translate_polygon(polygon* in, polygon* out, vec2f translation);

/*
Decomposes a polygon into convex polygons, which can be used for collision detection

Arguments
    polygon* in: The polygon to decompose

    polygon** out: A reference to a pointer to hold the decomposed polygon array. The value
        of out should not be NULL. The memory at *out is allocated, so free it, along with every
        polygon created from decompostion.

    int* outCount: A pointer that will contain the number of polygons created from the decomposition.

Returns
    Returns false if any of the arguments are bad or if memory allocation fails.
*/
bool decompose_polygon(polygon* in, polygon** out, int* outCount);

/*
Projects a polygon onto a line that passes through (0, 0) with a give slope where
only the two most extreme points are kept

Arguments
    polygon* poly: The polygon to project

    vec2f slope: The slope of the line to project onto

    vec2f* projectResult: An array with a length of 2 that will contain the most extreme
        project points

    float* projectionDistanceSqrd: The distance squared between the two project points.

Returns
    Returns false if any of the pointer arguments are NULL
*/
bool project_polygon(polygon* poly, vec2f slope, vec2f* projectionResult, float* projectionDistanceSqrd);

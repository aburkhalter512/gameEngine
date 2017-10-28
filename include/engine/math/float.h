#pragma once

#include <stdbool.h>

extern const float DEFAULT_TOLERANCE;

/*
Determines if two floats are equal with a given tolerance. The formula used
to do the comparison is |f1 - f2| < |tolerance|

Arguments
    float f1: The first float to compare

    float f2: The second float to compare

    float tolerance: The tolerance used to determine if the two floats are equal.

Returns
    Returns a boolean value if the two floats are equal
*/
bool equal_f(float f1, float f2, float tolerance);

/*
Calculates the perpendicular slope to the given slope "s"

Arguments
    float s: The base slope from which the perpendicular slope is calculated from.

Preconditions
    s != 0

Returns
    Returns float which represents the slope perpendicular to the slope "s"
*/
float perpendicular_f(float s);

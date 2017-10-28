#include "engine/math/float.h"

#include <math.h>

const float DEFAULT_TOLERANCE = 0.001f;

bool equal_f(float f1, float f2, float tolerance)
{
    tolerance = fabs(tolerance);

    return fabs(f1 - f2) < tolerance;
}

float perpendicular_f(float s)
{
    return -1.0f / s;
}

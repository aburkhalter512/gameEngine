#include "engine/math/vec.h"
#include "engine/math/float.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

vec3f fromVec2f_vec3f(vec2f v)
{
    return to_vec3f(GET_X(v), GET_Y(v), 1.0f);
}

vec2f fromVec3f_vec2f(vec3f v)
{
    return to_vec2f(GET_X(v), GET_Y(v));
}

bool equal_vec2f(vec2f v1, vec2f v2, float tolerance)
{
    return equal_f(GET_X(v1), GET_X(v2), tolerance) && equal_f(GET_Y(v1), GET_Y(v2), tolerance);
}

vec2f neg_vec2f(vec2f v)
{
    return to_vec2f(-GET_X(v), -GET_Y(v));
}

vec2f add_vec2f(vec2f v1, vec2f v2)
{
    return to_vec2f(GET_X(v1) + GET_X(v2), GET_Y(v1) + GET_Y(v2));
}

vec2f sub_vec2f(vec2f v1, vec2f v2)
{
    return to_vec2f(GET_X(v1) - GET_X(v2), GET_Y(v1) - GET_Y(v2));
}

vec2f mul_vec2f(vec2f v, float s)
{
    return to_vec2f(GET_X(v) * s, GET_Y(v) * s);
}

vec2f div_vec2f(vec2f v, float s)
{
    return to_vec2f(GET_X(v) / s, GET_Y(v) / s);
}

float slope_vec2f(vec2f v)
{
    return GET_Y(v) / GET_X(v);
}

vec2f perpendicular_vec2f(vec2f v)
{
    return to_vec2f(-GET_Y(v), GET_X(v));
}

float radius_vec2f(vec2f v)
{
    return sqrt(GET_X(v) * GET_X(v) + GET_Y(v) * GET_Y(v));
}

vec2f intersect_vec2f(vec2f p1, float m1, vec2f p2, float m2)
{
    if (equal_vec2f(p1, p2, DEFAULT_TOLERANCE))
    {
        return p1;
    }

    // Transform operation to p1 local space
    p2 = sub_vec2f(p2, p1);

    // Calcuate intersection of lines
    // solve for x
    // * y = m1 x
    // * y = m2 x + b
    // * m1 x = m2 x + b
    // * (m1 - m2) x = b
    // * x = b / (m1 - m2)
    // solve for b
    // * GET_Y(p2) = m2 GET_X(p2) + b
    // * b = GET_Y(p2) - m2 GET_X(p2)
    vec2f intersection;
    GET_X(intersection) = (GET_Y(p2) - m2 * GET_X(p2)) / (m1 - m2);
    GET_Y(intersection) = m1 * GET_X(intersection);

    // Transform back to world space
    intersection = add_vec2f(intersection, p1);

    return intersection;
}

float distanceSqrd_vec2f(vec2f p1, vec2f p2)
{
    float dX = GET_X(p2) - GET_X(p1);
    float dY = GET_Y(p2) - GET_Y(p1);
    return dX * dX + dY * dY;
}

float distance_vec2f(vec2f p1, vec2f p2)
{
    return sqrt(distanceSqrd_vec2f(p1, p2));
}

enum LINEARITY linearity_vec2f(vec2f v1, vec2f v2, vec2f v3)
{
    // A X B (X == cross product)
    float linearity = (GET_X(v2) - GET_X(v1)) * (GET_Y(v3) - GET_Y(v1)) - (GET_X(v3) - GET_X(v1)) * (GET_Y(v2) - GET_Y(v1));
    if (equal_f(linearity, 0.0f, DEFAULT_TOLERANCE))
    {
        return LINEARITY_COLLINEAR;
    }

    return linearity < LINEARITY_COLLINEAR ? LINEARITY_CLOCKWISE : LINEARITY_COUNTER_CLOCKWISE;
}

// Precondition: v1-3 are collinear points
bool _between_vec2f(vec2f endpoint1, vec2f endpoint2, vec2f point)
{
    // If the line is vertical
    if (equal_f(GET_X(endpoint1), GET_X(endpoint2), DEFAULT_TOLERANCE))
    {
        return (GET_Y(endpoint1) <= GET_Y(point) && GET_Y(point) <= GET_Y(endpoint2)) || (GET_Y(endpoint2) <= GET_Y(point) && GET_Y(point) <= GET_Y(endpoint1));
    }
    else
    {
        return (GET_X(endpoint1) <= GET_X(point) && GET_X(point) <= GET_X(endpoint2)) || (GET_X(endpoint2) <= GET_X(point) && GET_X(point) <= GET_X(endpoint1));
    }
}

bool areSegmentsIntersecting_vec2f(vec2f endpoint1_1, vec2f endpoint1_2, vec2f endpoint2_1, vec2f endpoint2_2)
{
    enum LINEARITY linearity[] = {
        linearity_vec2f(endpoint1_1, endpoint1_2, endpoint2_1),
        linearity_vec2f(endpoint1_1, endpoint1_2, endpoint2_2),
        linearity_vec2f(endpoint1_1, endpoint2_1, endpoint2_2),
        linearity_vec2f(endpoint1_2, endpoint2_1, endpoint2_2)
    };

    // Check for the enpoints lying on one of the segments
    if (linearity[0] == LINEARITY_COLLINEAR)
    {
        return _between_vec2f(endpoint1_1, endpoint1_2, endpoint2_1);
    }
    else if (linearity[1] == LINEARITY_COLLINEAR)
    {
        return _between_vec2f(endpoint1_1, endpoint1_2, endpoint2_2);
    }
    else if (linearity[2] == LINEARITY_COLLINEAR)
    {
        return _between_vec2f(endpoint2_1, endpoint2_2, endpoint1_1);
    }
    else if (linearity[3] == LINEARITY_COLLINEAR)
    {
        return _between_vec2f(endpoint2_1, endpoint2_2, endpoint1_2);
    }

    // ensure that each segment only has one point of the other segment to the left of it
    return (linearity[0] == LINEARITY_CLOCKWISE) ^ (linearity[1] == LINEARITY_CLOCKWISE) &&
        (linearity[2] == LINEARITY_CLOCKWISE) ^ (linearity[3] == LINEARITY_CLOCKWISE);
}

vec2f project_vec2f(vec2f slope, vec2f point)
{
    // If the slope is horizontal or vertical, then we need to do some custom processing
    if (equal_f(GET_Y(slope), 0.0f, DEFAULT_TOLERANCE))
    {
        return to_vec2f(GET_X(point), 0.0f);
    }
    else if (equal_f(GET_X(slope), 0.0f, DEFAULT_TOLERANCE))
    {
        return to_vec2f(0.0f, GET_Y(point));
    }
    else
    {
        float projectionSlope = slope_vec2f(slope);
        float intersectionSlope = perpendicular_f(projectionSlope);
        return intersect_vec2f(to_vec2f(0, 0), projectionSlope, point, intersectionSlope);
    }
}

float magnitude_vec2f(vec2f v)
{
    return sqrt(GET_X(v) * GET_X(v) + GET_Y(v) * GET_Y(v));
}

vec2f normalize_vec2f(vec2f v)
{
    float mag = magnitude_vec2f(v);
    return div_vec2f(v, mag);
}

float dot_vec2f(vec2f v1, vec2f v2)
{
    return GET_X(v1) * GET_X(v2) + GET_Y(v1) * GET_Y(v2);
}

vec2f reflect_vec2f(vec2f v, vec2f normal)
{
    vec2f n = normalize_vec2f(normal);

    // d − 2 * (d ⋅ n) * n
    return sub_vec2f(v, mul_vec2f(n, 2.0f * dot_vec2f(v, n)));
}

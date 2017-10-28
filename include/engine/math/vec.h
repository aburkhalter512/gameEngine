#pragma once

#include <stdbool.h>

#include "engine/math/matrix.h"

typedef MATRIX_TYPE(1, 1) vec1f;
typedef MATRIX_TYPE(2, 1) vec2f;
typedef MATRIX_TYPE(3, 1) vec3f;

#define GET_X(v) (v).data[0][0]
#define GET_Y(v) (v).data[1][0]
#define GET_Z(v) (v).data[2][0]

enum LINEARITY {
    LINEARITY_CLOCKWISE = -1,
    LINEARITY_COLLINEAR = 0,
    LINEARITY_COUNTER_CLOCKWISE = 1,
};

/*
Creates a vec1f from its components

Allocates no memory

Arguments
    float x: The x value of the vec1f

Returns
    The vec1f created from (x)
*/
#define to_vec1f(x) ((vec1f) {{{ x }}})

/*
Creates a vec2f from its components

Allocates no memory

Arguments
    float x: The x value of the vec2f

    float y: The y value of the vec2f

Returns
    The vec2f created from (x, y)
*/
#define to_vec2f(x, y) ((vec2f) {{{ x }, { y }}})

/*
Creates a vec3f from its components

Allocates no memory

Arguments
    float x: The x value of the vec3f

    float y: The y value of the vec3f

    float y: The y value of the vec3f

Returns
    The vec2f created from (x, y)
*/
#define to_vec3f(x, y, z) ((vec3f) {{{ x }, { y }, { z }}})

vec3f fromVec2f_vec3f(vec2f v);
vec2f fromVec3f_vec2f(vec3f v);

/*
Determines if two vec2f's are equal, given a finite tolerance. Each coordinate
is compared using said given tolerance.

Arguments
    vec2f v1: The first vector to compare

    vec2f v2: The second vector to compare

    float tolerance: The acceptable tolerance used when comparing the individual
        coordinates

Returns
    Returns true if the two vectors are equal
*/
bool equal_vec2f(vec2f v1, vec2f v2, float tolerance);

/*
Negates a vector.

Arguments
    vec2f v: The vector to negate

Returns
    Returns a vector with the coordinates (-v.x, -v.y)
*/
vec2f neg_vec2f(vec2f v);

/*
Adds one vector to another and returns a new vector as the result

Arguments
    vec2f v1: The first vector to add

    vec2f v2: The second vector to add

Returns
    Returns a vector with the coordinates (v1.x + v2.x, v1.y + v2.y)
*/
vec2f add_vec2f(vec2f v1, vec2f v2);

/*
Subtracts one vector from another and returns a new vector as the result

Arguments
    vec2f v1: The base vector

    vec2f v2: The vector to subtract from the base vector

Returns
    Returns a vector with the coordinates (v1.x - v2.x, v1.y - v2.y)
*/
vec2f sub_vec2f(vec2f v1, vec2f v2);

/*
Performs scalar multiplication of a vector

Arguments
    vec2f v: The vector to multiply

    float s: The scalar to multiply the vector by

Returns
    Returns a vector with the coordinates (v.x * s, v.y * s)
*/
vec2f mul_vec2f(vec2f v, float s);

/*
Performs scalar division of a vector

Arguments
    vec2f v: The vector to divide

    float s: The scalar to divide the vector by

Returns
    Returns a vector with the coordinates (v.x / s, v.y / s)
*/
vec2f div_vec2f(vec2f v, float s);

/*
Calculates the slope of a vector

Arguments
    vec2f v: The vector to calculate the slope from

Preconditions
    v.x is not 0

Returns
    Returns a float with the value v.y / v.x
*/
float slope_vec2f(vec2f v);

/*
Calculates the vector perpendicular to v

Arguments
    vec2f v: The vector from which to calculate the perpendicular vector

Returns
    Returns a vector with the coordinates (-v.y, v.x)
*/
vec2f perpendicular_vec2f(vec2f v);

/*
Calculates the radius of a circle centered at (0, 0) given a radius vector v.
This function can be considered an optimization of distance_vec2f.

Arguments
    vec2f v: The vector from which to calculate the radius from

Returns
    Returns a float that represents the radius of a circle centered at (0, 0)
*/
float radius_vec2f(vec2f v);

/*
Calculates the point of intersection for two non-parallel lines

Arguments
    vec2f p1: A point on the first line

    float m1: The slope of the first line

    vec2f p2: A point on the second line

    float m2: The slope of the second line

Preconditions
    m1 - m2 != 0

Returns
    Returns a point where the lines intersect
*/
vec2f intersect_vec2f(vec2f p1, float m1, vec2f p2, float m2);

/*
Calculates the distance squared between two points. This function is useful
for comparing distances in a performant manner

Arguments
    vec2f p1: The first point

    vec2f p2: The second point

Returns
    Returns a float which is distance squared betwen p1 and p2
*/
float distanceSqrd_vec2f(vec2f p1, vec2f p2);

/*
Calculates the distance between two points.

Arguments
    vec2f p1: The first point

    vec2f p2: The second point

Returns
    Returns a float which is distance betwen p1 and p2
*/
float distance_vec2f(vec2f p1, vec2f p2);

/*
Calculates the linearity or order of 3 points. If the 3 points appear in clockwise order
LINEARITY_CLOCKWISE will be returned.

Arguments
    vec2f p1: The first point to determine the LINEARITY from

    vec2f p2: The second point to determine the LINEARITY from

    vec2f p3: The third point to determine the LINEARITY from
*/
enum LINEARITY linearity_vec2f(vec2f v1, vec2f v2, vec2f v3);

/*
Determines if two line segments are intersecting. This function defines intersecting
as if the line segments cross AND if one of the points lies on the other line
segment.

Arguments
    vec2f endpoint1_1: The first endpoint of line segment 1

    vec2f endpoint1_2: The second endpoint of line segment 1

    vec2f endpoint2_1: The first endpoint of line segment 2

    vec2f endpoint2_2: The second endpoint of line segment 2

Returns
    Returns true if the line segments are intersecting, false if not.
*/
bool areSegmentsIntersecting_vec2f(vec2f endpoint1_1, vec2f endpoint1_2, vec2f endpoint2_1, vec2f endpoint2_2);

/*
Projects a point onto a line that passes through (0, 0) with a given slope specified. The projection
slope is perpendicular to the slope of the line.

Arguments
    vec2f slope: The slope of the line to project the point onto

    vec2f point: The point to project

Returns
    Returns the projected point
*/
vec2f project_vec2f(vec2f slope, vec2f point);

/*
Returns the magnitude of the vector

Arguments
    vec2f v: The vector to get the magnitude of

Returns
    Returns the magnitude of v
*/
float magnitude_vec2f(vec2f v);

/*
Returns the normalized version of the given vector

Arguments
    vec2f v: The vector to normalize

Returns
    Returns v normalized
*/
vec2f normalize_vec2f(vec2f v);

/*
Calculates the dot product of two vectors

Returns
    Returns the dot product of the two vectors
*/
float dot_vec2f(vec2f v1, vec2f v2);

/*
Reflects a vector about the normal vector of surface

Arguments
    vec2f v: The vector to reflect

    vec2f normal: The normal of the surface to reflect the vector about.
*/
vec2f reflect_vec2f(vec2f v, vec2f normal);

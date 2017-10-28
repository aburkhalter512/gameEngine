#ifndef UNIT_TEST
#include "engine/math/transform.h"
#else
#include "engine/unit/math/transform.unit.h"
#endif

#include "engine/math/matrix.h"
#include "engine/math/polygon.h"

#include <math.h>
#include <stdlib.h>

MATRIX_TYPE(3, 3) getMatrix_transform(transform* t)
{
    MATRIX_TYPE(3, 3) transMatrix = {{
        {1, 0, GET_X(t->position)},
        {0, 1, GET_Y(t->position)},
        {0, 0, 1},
    }};

    double sinAngle = sin(t->rotation);
    double cosAngle = cos(t->rotation);
    MATRIX_TYPE(3, 3) rotMatrix = {{
        {cosAngle,  sinAngle, 0},
        {-sinAngle, cosAngle, 0},
        {0,         0,        1}
    }};
    
    MATRIX_TYPE(3, 3) scaleMatrix = {{
        {GET_X(t->scale), 0,               0},
        {0,               GET_Y(t->scale), 0},
        {0,               0,               1},
    }};

    MATRIX_TYPE(3, 3) rotScaleMatrix = MULTIPLY_MATRIX_FN(3, 3, 3)(&rotMatrix, &scaleMatrix);


    return MULTIPLY_MATRIX_FN(3, 3, 3)(
        &transMatrix,
        &rotScaleMatrix
    );
}

/*
Applies a 3x3 matrix to a 2d point. A 3x3 matrix is used so that translations can
be encoded in matrix form

Arguments
    vec2f point: The point to transform

    matrix33f* matrix: The matrix to apply to point

Returns
    Returns the transformed 2d point
*/
vec2f _applymMatrix_vec2f(vec2f point, MATRIX_TYPE(3, 3)* matrix)
{
    MATRIX_TYPE(3, 1) p = PAD_MATRIX_FN(2, 1, 3, 1)(&point, 1.0f);
    MATRIX_TYPE(3, 1) result = MULTIPLY_MATRIX_FN(3, 3, 1)(matrix, &p);
    return PAD_MATRIX_FN(3, 1, 2, 1)(&result, 0.0f);
}

bool applyTransform_polygon(polygon* in, transform* t, polygon* out)
{
    // Make sure we are working with valid data
    if (!in || !t || !out)
    {
        return false;
    }

    // Memory allocation check
    if (!create_polygon(out, in->vertexCount))
    {
        return false;
    }

    // Transform the polygon
    MATRIX_TYPE(3, 3) transformMatrix = getMatrix_transform(t);
    for (size_t i = 0; i < out->vertexCount; i++)
    {
        out->vertices[i] = _applymMatrix_vec2f(in->vertices[i], &transformMatrix);
    }

    return true;
}

#pragma once

#include <stdbool.h>

#include "engine/math/matrix.h"
#include "engine/math/vec.h"

typedef struct _polygon polygon;

typedef struct _transform {
    vec2f position;
    float rotation;
    vec2f scale;
} transform;

/*
Gets the 3x3 matrix describing the transform t. This matrix can be applied to
any point to transform it with respect to the origin. The final transformation 
is calculated as so: scale * rotation * position

Arguments
    transform* t: The transform to get the matrix from

Returns
    Returns a 3x3 matrix describing the transform t
*/
MATRIX_TYPE(3, 3) getMatrix_transform(transform* t);

/*
Applies a transform to a polygon centered around the origin and returns it

Arguments:
    polygon* in: The polygon to apply the transform to

    transform* t: The transform to apply to the polygon

    polygon* out: The resulting transformed polygon will be saved to this pointer

Returns
    Returns false if any of the arguments are NULL or if memory allocation fails.
    Out should be deallocated by calling free_polygon()
*/
bool applyTransform_polygon(polygon* in, transform* t, polygon* out);

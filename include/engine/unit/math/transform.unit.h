#pragma once

#include "engine/math/transform.h"

#include "util/unit.h"

vec2f _applymMatrix_vec2f(vec2f point, MATRIX_TYPE(3, 3)* matrix);

PROTOTYPE_TEST(getMatrix_transform);
PROTOTYPE_TEST(_applymMatrix_vec2f);
PROTOTYPE_TEST(applyTransform_polygon);
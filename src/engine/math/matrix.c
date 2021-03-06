#include "engine/math/matrix.h"

#include <string.h>

///////////////////////////////////////////////////////////////////////////////
// Multiply Matrix
///////////////////////////////////////////////////////////////////////////////
DEFINE_MULTIPLY_MATRIX(2, 2, 2); // 2x2 * 2x2
DEFINE_MULTIPLY_MATRIX(3, 3, 3); // 3x3 * 3x3
DEFINE_MULTIPLY_MATRIX(3, 3, 1); // 3x3 * 3x1

///////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
///////////////////////////////////////////////////////////////////////////////
DEFINE_TRANSPOSE_MATRIX(3, 1);

///////////////////////////////////////////////////////////////////////////////
// Pad Matrix
///////////////////////////////////////////////////////////////////////////////
DEFINE_PAD_MATRIX(2, 2, 3, 3);
DEFINE_PAD_MATRIX(2, 1, 3, 1);
DEFINE_PAD_MATRIX(3, 1, 2, 1);

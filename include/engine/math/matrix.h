#pragma once

#include <stddef.h>
#include <stdio.h>

#define MATRIX_TYPE(rows, cols) struct matrix_ ## rows ## _ ## cols

#define DEFINE_MATRIX(rows, cols) MATRIX_TYPE(rows, cols)\
{\
	float data[rows][cols];\
}



// PROTOTYPE_CROSS_MATRIX(3, 3, 1) is equivalent to 
// 3x3 * 3x1
// ^ ^   ^ ^
// ^ ^   ^ second number (height of final matrix)
// ^ ^   ^
// ^ third number (height of first matrix and width of second)
// ^
// first number (width of final matrix)
#define MULTIPLY_MATRIX_FN(rows, depth, cols) multiply_ ## rows ## x ## depth ## _ ## depth ## x ## cols ## _matrix
#define PROTOTYPE_MULTIPLY_MATRIX(rows, depth, cols) MATRIX_TYPE(rows, cols) MULTIPLY_MATRIX_FN(rows, depth, cols)(MATRIX_TYPE(rows, depth)* m1, MATRIX_TYPE(depth, cols)* m2)
#define DEFINE_MULTIPLY_MATRIX(rows, depth, cols) PROTOTYPE_MULTIPLY_MATRIX(rows, depth, cols)\
{\
	MATRIX_TYPE(rows, cols) result;\
	for (size_t row = 0; row < rows; row++)\
	{\
		for (size_t col = 0; col < cols; col++)\
		{\
			result.data[row][col] = 0.0f;\
			for (size_t i = 0; i < depth; i++)\
			{\
				result.data[row][col] += m1->data[row][i] * m2->data[i][col];\
			}\
		}\
	}\
	return result;\
}\

#define TRANSPOSE_MATRIX_FN(rows, cols) transpose_ ## rows ## x ## cols ## _matrix
#define PROTOTYPE_TRANSPOSE_MATRIX(rows, cols) MATRIX_TYPE(cols, rows) TRANSPOSE_MATRIX_FN(rows, cols)(MATRIX_TYPE(rows, cols)* m)
#define DEFINE_TRANSPOSE_MATRIX(rows, cols) PROTOTYPE_TRANSPOSE_MATRIX(rows, cols)\
{\
	MATRIX_TYPE(cols, rows) result;\
	for (size_t row = 0; row < rows; row++)\
	{\
		for (size_t col = 0; col < cols; col++)\
		{\
			result.data[col][row] = m->data[row][col];\
		}\
	}\
	return result;\
}\

#define PAD_MATRIX_FN(rows, cols, padRows, padCols) pad_ ## rows ## x ## cols ## _ ## padRows ## x ## padCols ## _matrix
#define PROTOTYPE_PAD_MATRIX(rows, cols, padRows, padCols) MATRIX_TYPE(padRows, padCols) PAD_MATRIX_FN(rows, cols, padRows, padCols)(MATRIX_TYPE(rows, cols)* m, float fillValue)
#define DEFINE_PAD_MATRIX(rows, cols, padRows, padCols) PROTOTYPE_PAD_MATRIX(rows, cols, padRows, padCols)\
{\
	MATRIX_TYPE(padRows, padCols) result;\
	for (size_t row = 0; row < padRows; row++)\
	{\
		for (size_t col = 0; col < padCols; col++)\
		{\
			if (row < rows && col < cols)\
			{\
				result.data[row][col] = m->data[row][col];\
			}\
			else\
			{\
				result.data[row][col] = fillValue;\
			}\
		}\
	}\
	return result;\
}\

// Types
DEFINE_MATRIX(1, 1);
DEFINE_MATRIX(1, 2);
DEFINE_MATRIX(1, 3);
DEFINE_MATRIX(2, 1);
DEFINE_MATRIX(2, 2);
DEFINE_MATRIX(2, 3);
DEFINE_MATRIX(3, 1);
DEFINE_MATRIX(3, 2);
DEFINE_MATRIX(3, 3);

///////////////////////////////////////////////////////////////////////////////
// Multiply Matrix
///////////////////////////////////////////////////////////////////////////////
PROTOTYPE_MULTIPLY_MATRIX(2, 2, 2); // 2x2 * 2x2
PROTOTYPE_MULTIPLY_MATRIX(3, 3, 3); // 3x3 * 3x3
PROTOTYPE_MULTIPLY_MATRIX(3, 3, 1); // 3x3 * 3x1

///////////////////////////////////////////////////////////////////////////////
// Transpose Matrix
///////////////////////////////////////////////////////////////////////////////
PROTOTYPE_TRANSPOSE_MATRIX(3, 1);

///////////////////////////////////////////////////////////////////////////////
// Pad Matrix
///////////////////////////////////////////////////////////////////////////////
PROTOTYPE_PAD_MATRIX(2, 2, 3, 3);
PROTOTYPE_PAD_MATRIX(2, 1, 3, 1);
PROTOTYPE_PAD_MATRIX(3, 1, 2, 1);

#include "engine/unit/math/transform.unit.h"

#include "engine/math/float.h"
#include "engine/math/polygon.h"
#include "engine/math/vec.h"

#include "engine/util.h"

#include "engine/unit/math/polygon.unit.h"

#include <math.h>

IMPLEMENT_TEST(getMatrix_transform)
{
	char resultMessage[120];
	transform t;
	MATRIX_TYPE(3, 3) expected;
	MATRIX_TYPE(3, 3) actual;

	///////////////////////////////////////////////////////////////////////////
	// Scenario: No transformation
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		0.0f, // rotation
		to_vec2f(1.0f, 1.0f), //scale
	};

	expected = (MATRIX_TYPE(3, 3)) {{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
	}};

	actual = getMatrix_transform(&t);

	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (!equal_f(actual.data[row][col], expected.data[row][col], DEFAULT_TOLERANCE))
			{
				sprintf(resultMessage, "Scenario 1: matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Only position
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(2.0f, -2.0f), // position
		0.0f, // rotation
		to_vec2f(1.0f, 1.0f), //scale
	};

	expected = (MATRIX_TYPE(3, 3)) {{
		{1.0f, 0.0f, 2.0f},
		{0.0f, 1.0f, -2.0f},
		{0.0f, 0.0f, 1.0f},
	}};

	actual = getMatrix_transform(&t);

	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (!equal_f(actual.data[row][col], expected.data[row][col], DEFAULT_TOLERANCE))
			{
				sprintf(resultMessage, "Scenario 2: matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Only rotation
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		M_PI / 6, // rotation
		to_vec2f(1.0f, 1.0f), //scale
	};

	expected = (MATRIX_TYPE(3, 3)) {{
		{0.86602f, 0.5f,     0.0f},
		{-0.5f,    0.86602f, 0.0f},
		{0.0f,     0.0f,     1.0f},
	}};

	actual = getMatrix_transform(&t);

	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (!equal_f(actual.data[row][col], expected.data[row][col], DEFAULT_TOLERANCE))
			{
				sprintf(resultMessage, "Scenario 3: matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Only scale
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		0.0f, // rotation
		to_vec2f(2.0f, -1.0f), //scale
	};

	expected = (MATRIX_TYPE(3, 3)) {{
		{2.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
	}};

	actual = getMatrix_transform(&t);

	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (!equal_f(actual.data[row][col], expected.data[row][col], DEFAULT_TOLERANCE))
			{
				sprintf(resultMessage, "Scenario 4: matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Rotation and scale
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		M_PI / 6, // rotation
		to_vec2f(2.0f, -1.0f), //scale
	};

	expected = (MATRIX_TYPE(3, 3)) {{
		{1.7320, -0.5f,     0.0f},
		{-1.0f,  -0.86602f, 0.0f},
		{0.0f,   0.0f,     1.0f},
	}};

	actual = getMatrix_transform(&t);

	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (!equal_f(actual.data[row][col], expected.data[row][col], DEFAULT_TOLERANCE))
			{
				sprintf(resultMessage, "Scenario 5: matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Combination of scale + rot + translate
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(2.0f, -2.0f), // position: not 1 or 0
		M_PI / 6, // rotation: 30 deg
		to_vec2f(2.0f, -1.0f), // scale: not 1, 0, or position % 2 == 0
	};

	expected = (MATRIX_TYPE(3, 3)) {{
		{1.7320, -0.5f,     2.0f},
		{-1.0f,  -0.86602f, -2.0f},
		{0.0f,   0.0f,     1.0f},
	}};

	actual = getMatrix_transform(&t);

	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (!equal_f(actual.data[row][col], expected.data[row][col], DEFAULT_TOLERANCE))
			{
				sprintf(resultMessage, "matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	PASS_TEST();
}

IMPLEMENT_TEST(_applymMatrix_vec2f)
{
	int offset = 0;
	char resultMessage[120];
	transform t;
	MATRIX_TYPE(3, 3) transformMatrix;

	vec2f expected;
	vec2f actual;

	///////////////////////////////////////////////////////////////////////////
	// Scenario: No transformation
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		0.0f, // rotation
		to_vec2f(1.0f, 1.0f), //scale
	};
	transformMatrix = getMatrix_transform(&t);

	expected = to_vec2f(2.0f, -2.0f);
	actual = _applymMatrix_vec2f(to_vec2f(2.0f, -2.0f), &transformMatrix);

	if (!equal_f(0.0f, distance_vec2f(expected, actual), DEFAULT_TOLERANCE))
	{
		offset += sprintf(resultMessage + offset, "Identity transform not applied correctly. expected: ");
		offset += sprint_vec2f(resultMessage + offset, &expected);
		offset += sprintf(resultMessage + offset, ", actual: ");
		offset += sprint_vec2f(resultMessage + offset, &actual);
		FAIL_TEST(resultMessage);
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Only scale
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		0.0f, // rotation
		to_vec2f(2.0f, 3.0f), //scale
	};
	transformMatrix = getMatrix_transform(&t);

	expected = to_vec2f(4.0f, -6.0f);
	actual = _applymMatrix_vec2f(to_vec2f(2.0f, -2.0f), &transformMatrix);

	if (!equal_f(0.0f, distance_vec2f(expected, actual), DEFAULT_TOLERANCE))
	{
		offset += sprintf(resultMessage + offset, "Scale transform not applied correctly. expected: ");
		offset += sprint_vec2f(resultMessage + offset, &expected);
		offset += sprintf(resultMessage + offset, ", actual: ");
		offset += sprint_vec2f(resultMessage + offset, &actual);
		FAIL_TEST(resultMessage);
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Only rotation
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		M_PI / 6, // rotation
		to_vec2f(1.0f, 1.0f), //scale
	};
	transformMatrix = getMatrix_transform(&t);

	expected = to_vec2f(0.7320, -2.7320);
	actual = _applymMatrix_vec2f(to_vec2f(2.0f, -2.0f), &transformMatrix);

	if (!equal_f(0.0f, distance_vec2f(expected, actual), DEFAULT_TOLERANCE))
	{
		offset += sprintf(resultMessage + offset, "Rotation transform not applied correctly. expected: ");
		offset += sprint_vec2f(resultMessage + offset, &expected);
		offset += sprintf(resultMessage + offset, ", actual: ");
		offset += sprint_vec2f(resultMessage + offset, &actual);
		FAIL_TEST(resultMessage);
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Scale and rotation
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(0.0f, 0.0f), // position
		M_PI / 6, // rotation
		to_vec2f(2.0f, -1.0f), //scale
	};
	transformMatrix = getMatrix_transform(&t);

	expected = to_vec2f(4.4641f, -0.2680f);
	actual = _applymMatrix_vec2f(to_vec2f(2.0f, -2.0f), &transformMatrix);

	if (!equal_f(0.0f, distance_vec2f(expected, actual), DEFAULT_TOLERANCE))
	{
		offset += sprintf(resultMessage + offset, "Scale + Rotation transform not applied correctly. expected: ");
		offset += sprint_vec2f(resultMessage + offset, &expected);
		offset += sprintf(resultMessage + offset, ", actual: ");
		offset += sprint_vec2f(resultMessage + offset, &actual);
		FAIL_TEST(resultMessage);
	}

	///////////////////////////////////////////////////////////////////////////
	// Scenario: Scale, rotation, and position
	///////////////////////////////////////////////////////////////////////////
	t = (transform) {
		to_vec2f(2.0f, 3.0f), // position
		M_PI / 6, // rotation
		to_vec2f(2.0f, -1.0f), //scale
	};
	transformMatrix = getMatrix_transform(&t);

	expected = to_vec2f(6.4641f, 2.7320f);
	actual = _applymMatrix_vec2f(to_vec2f(2.0f, -2.0f), &transformMatrix);

	if (!equal_f(0.0f, distance_vec2f(expected, actual), DEFAULT_TOLERANCE))
	{
		offset += sprintf(resultMessage + offset, "Scale + Rotation + position transform not applied correctly. expected: ");
		offset += sprint_vec2f(resultMessage + offset, &expected);
		offset += sprintf(resultMessage + offset, ", actual: ");
		offset += sprint_vec2f(resultMessage + offset, &actual);
		FAIL_TEST(resultMessage);
	}

	PASS_TEST();
}

IMPLEMENT_TEST(applyTransform_polygon)
{
	char resultMsg[320];

	vec2f pVertices[3] = {
		to_vec2f(0.0f, 0.0f),
		to_vec2f(3.0f, 1.0f),
		to_vec2f(2.0f, 2.0f),
	};

	polygon p = {
		pVertices,
		3,
	};

	transform t = (transform) {
		to_vec2f(2.0f, 3.0f), // position
		-M_PI / 6, // rotation
		to_vec2f(2.0f, -1.0f), //scale
	};

	vec2f expectedVertices[3] = {
		to_vec2f(2.0f,   3.0f),
		to_vec2f(7.696f, 5.134f),
		to_vec2f(6.464f, 3.268f),
	};

	polygon expected = {
		expectedVertices,
		3,
	};

	polygon actual;
	if (!applyTransform_polygon(&p, &t, &actual))
	{
		FAIL_TEST("Memory allocation failed");
	}

	if (!verifyPolygon(&actual, &expected, resultMsg, 0))
	{
		FAIL_TEST(resultMsg);
	}

	free_polygon(&actual);

	PASS_TEST();
}

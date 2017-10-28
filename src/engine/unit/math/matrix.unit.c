#include "engine/unit/math/matrix.unit.h"

#include "engine/math/float.h"

DEFINE_TEST(TRANSPOSE_MATRIX_FN(3, 1))
{
	MATRIX_TYPE(3, 1) m = {{{1.0f}, {2.0f}, {3.0f}}};

	MATRIX_TYPE(1, 3) t = TRANSPOSE_MATRIX_FN(3, 1)(&m);

	if (t.data[0][0] != m.data[0][0])
	{
		FAIL_TEST("m.data[0][0] not transposed to t.data[0][0].");
	}
	else if(t.data[0][1] != m.data[1][0])
	{
		FAIL_TEST("m.data[0][1] not transposed to t.data[1][0].");
	}
	else if (t.data[0][2] != m.data[2][0])
	{
		FAIL_TEST("m.data[0][2] not transposed to t.data[2][0].");
	}

	PASS_TEST();
}

DEFINE_TEST(MULTIPLY_MATRIX_FN(2, 2, 2))
{
	MATRIX_TYPE(2, 2) m1 = {{
		{1.0f, 2.0f},
		{3.0f, 4.0f},
	}};

	MATRIX_TYPE(2, 2) m2 = {{
		{1.0f, 2.0f},
		{3.0f, 4.0f},
	}};

	MATRIX_TYPE(2, 2) actual = MULTIPLY_MATRIX_FN(2, 2, 2)(&m1, &m2);

	MATRIX_TYPE(2, 2) expected = {{
		{7.0f, 10.0f},
		{15.0f, 22.0f},
	}};

	char resultMessage[120];
	for (size_t row = 0; row < 2; row++)
	{
		for (size_t col = 0; col < 2; col++)
		{
			if (actual.data[row][col] != expected.data[row][col])
			{
				sprintf(resultMessage, "matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	PASS_TEST();
}

DEFINE_TEST(MULTIPLY_MATRIX_FN(3, 3, 3))
{
	MATRIX_TYPE(3, 3) m1 = {{
		{1.0f, 2.0f, 3.0f},
		{4.0f, 5.0f, 6.0f},
		{7.0f, 8.0f, 9.0f},
	}};

	MATRIX_TYPE(3, 3) m2 = {{
		{1.0f, 2.0f, 3.0f},
		{4.0f, 5.0f, 6.0f},
		{7.0f, 8.0f, 9.0f},
	}};

	MATRIX_TYPE(3, 3) actual = MULTIPLY_MATRIX_FN(3, 3, 3)(&m1, &m2);

	MATRIX_TYPE(3, 3) expected = {{
		{30.0f,  36.0f,  42.0f},
		{66.0f,  81.0f,  96.0f},
		{102.0f, 126.0f, 150.0f},
	}};

	char resultMessage[120];
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (actual.data[row][col] != expected.data[row][col])
			{
				sprintf(resultMessage, "matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	PASS_TEST();
}

DEFINE_TEST(MULTIPLY_MATRIX_FN(3, 3, 1))
{
	MATRIX_TYPE(3, 3) m1 = {{
		{1.0f, 2.0f, 3.0f},
		{4.0f, 5.0f, 6.0f},
		{7.0f, 8.0f, 9.0f},
	}};

	MATRIX_TYPE(3, 1) m2 = {{
		{1.0f},
		{4.0f},
		{7.0f},
	}};

	MATRIX_TYPE(3, 1) actual = MULTIPLY_MATRIX_FN(3, 3, 1)(&m1, &m2);

	MATRIX_TYPE(3, 1) expected = {{
		{30.0f},
		{66.0f},
		{102.0f},
	}};

	char resultMessage[120];
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 1; col++)
		{
			if (actual.data[row][col] != expected.data[row][col])
			{
				sprintf(resultMessage, "matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	PASS_TEST();
}

DEFINE_TEST(PAD_MATRIX_FN(2, 2, 3, 3))
{
	MATRIX_TYPE(2, 2) m = {{
		{1.0f, 2.0f},
		{4.0f, 5.0f},
	}};

	MATRIX_TYPE(3, 3) actual = PAD_MATRIX_FN(2, 2, 3, 3)(&m, 1.0f);

	MATRIX_TYPE(3, 3) expected = {{
		{1.0f, 2.0f, 1.0f},
		{4.0f, 5.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	}};

	char resultMessage[120];
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (actual.data[row][col] != expected.data[row][col])
			{
				sprintf(resultMessage, "matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	PASS_TEST();
}
DEFINE_TEST(PAD_MATRIX_FN(2, 1, 3, 1))
{
	MATRIX_TYPE(2, 1) m = {{
		{1.0f},
		{4.0f},
	}};

	MATRIX_TYPE(3, 1) actual = PAD_MATRIX_FN(2, 1, 3, 1)(&m, -1.0f);

	MATRIX_TYPE(3, 1) expected = {{
		{1.0f},
		{4.0f},
		{-1.0f},
	}};

	char resultMessage[120];
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 1; col++)
		{
			if (actual.data[row][col] != expected.data[row][col])
			{
				sprintf(resultMessage, "matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	PASS_TEST();
}
DEFINE_TEST(PAD_MATRIX_FN(3, 1, 2, 1))
{
	MATRIX_TYPE(3, 1) m = {{
		{1.0f},
		{4.0f},
		{-1.0f}
	}};

	MATRIX_TYPE(2, 1) actual = PAD_MATRIX_FN(3, 1, 2, 1)(&m, 0.0f);

	MATRIX_TYPE(2, 1) expected = {{
		{1.0f},
		{4.0f},
	}};

	char resultMessage[120];
	for (size_t row = 0; row < 2; row++)
	{
		for (size_t col = 0; col < 1; col++)
		{
			if (actual.data[row][col] != expected.data[row][col])
			{
				sprintf(resultMessage, "matrix[%zu][%zu] not computed correctly. Actual value: %.3f, Expected value: %.3f", row, col, actual.data[row][col], expected.data[row][col]);
				FAIL_TEST(resultMessage);
			}
		}
	}

	PASS_TEST();
}

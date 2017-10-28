#include "engine/unit/math/float.unit.h"

IMPLEMENT_TEST(equal_f)
{
    float f1 = 0;
    float f2 = 0.09;
    float f3 = -0.09;
    float f4 = 0.19;

    float tolerance = 0.1;

    if(!equal_f(f1, f2, tolerance))
    {
        FAIL_TEST("equal_f(0, 0.09, 0.1) failed");
    }

    if(!equal_f(f1, f3, tolerance))
    {
        FAIL_TEST("equal_f(0, -0.09, 0.1) failed");
    }

    if(!equal_f(f2, f4, tolerance))
    {
        FAIL_TEST("equal_f(0.09, 0.19, 0.1) failed");
    }

    if(equal_f(f1, f4, tolerance))
    {
        FAIL_TEST("!equal_f(0, 0.19, 0.1) failed");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(perpendicular_f)
{
    float f1 = 2;
    float f2 = perpendicular_f(f1);
    float f3 = perpendicular_f(f2);

    if(!equal_f(-1 / f1, f2, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("equal_f(-1 / f1, f2, 0.1) failed");
    }

    if(!equal_f(-1 / f2, f3, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("equal_f(-1 / f2, f3, 0.1) failed");
    }

    if(!equal_f(f1, f3, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("equal_f(f1, f3, 0.1) failed");
    }

    PASS_TEST();
}

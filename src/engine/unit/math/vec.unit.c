#include "engine/unit/math/vec.unit.h"

#include "engine/math/float.h"
#include "engine/math/vec.h"

IMPLEMENT_TEST(to_vec2f)
{
    vec2f v = to_vec2f(-1, -1);
    if (GET_X(v) != -1 || GET_Y(v) != -1)
    {
        FAIL_TEST("to_vec2f(-1, -1) failed");
    }

    v = to_vec2f(0, 0);
    if (GET_X(v) != 0 || GET_Y(v) != 0)
    {
        FAIL_TEST("to_vec2f(0, 0) failed");
    }

    v = to_vec2f(1, 1);
    if (GET_X(v) != 1 || GET_Y(v) != 1)
    {
        FAIL_TEST("to_vec2f(1, 1) failed");
    }

    v = to_vec2f(0.16f, -0.16f);
    if (GET_X(v) != 0.16f || GET_Y(v) != -0.16f)
    {
        FAIL_TEST("to_vec2f(0.16f, -0.16f) failed");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(add_vec2f)
{
    vec2f v1 = (vec2f) to_vec2f(0, 0);
    vec2f v2 = (vec2f) to_vec2f(1, 1);
    vec2f v3 = (vec2f) to_vec2f(-1, -1);
    vec2f v4 = (vec2f) to_vec2f(0.16f, -0.16f);

    vec2f result = add_vec2f(v1, v1);
    if (GET_X(result) != GET_X(v1) + GET_X(v1) || GET_Y(result) != GET_Y(v1) + GET_Y(v1))
    {
        FAIL_TEST("add_vec2f(v1, v1) failed");
    }

    result = add_vec2f(v1, v2);
    if (GET_X(result) != GET_X(v1) + GET_X(v2) || GET_Y(result) != GET_Y(v1) + GET_Y(v2))
    {
        FAIL_TEST("add_vec2f(v1, v2) failed");
    }

    result = add_vec2f(v1, v3);
    if (GET_X(result) != GET_X(v1) + GET_X(v3) || GET_Y(result) != GET_Y(v1) + GET_Y(v3))
    {
        FAIL_TEST("add_vec2f(v1, v3) failed");
    }

    result = add_vec2f(v1, v4);
    if (GET_X(result) != GET_X(v1) + GET_X(v4) || GET_Y(result) != GET_Y(v1) + GET_Y(v4))
    {
        FAIL_TEST("add_vec2f(v1, v4) failed");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(sub_vec2f)
{
    vec2f v1 = (vec2f) to_vec2f(0, 0);
    vec2f v2 = (vec2f) to_vec2f(1, 1);
    vec2f v3 = (vec2f) to_vec2f(-1, -1);
    vec2f v4 = (vec2f) to_vec2f(0.16f, -0.16f);

    vec2f result = sub_vec2f(v1, v1);
    if (GET_X(result) != GET_X(v1) - GET_X(v1) || GET_Y(result) != GET_Y(v1) - GET_Y(v1))
    {
        FAIL_TEST("sub_vec2f(v1, v1) failed");
    }

    result = sub_vec2f(v1, v2);
    if (GET_X(result) != GET_X(v1) - GET_X(v2) || GET_Y(result) != GET_Y(v1) - GET_Y(v2))
    {
        FAIL_TEST("sub_vec2f(v1, v2) failed");
    }

    result = sub_vec2f(v1, v3);
    if (GET_X(result) != GET_X(v1) - GET_X(v3) || GET_Y(result) != GET_Y(v1) - GET_Y(v3))
    {
        FAIL_TEST("sub_vec2f(v1, v3) failed");
    }

    result = sub_vec2f(v1, v4);
    if (GET_X(result) != GET_X(v1) - GET_X(v4) || GET_Y(result) != GET_Y(v1) - GET_Y(v4))
    {
        FAIL_TEST("sub_vec2f(v1, v4) failed");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(slope_vec2f)
{
    if (slope_vec2f((vec2f) to_vec2f(1, 0)) != 0)
    {
        FAIL_TEST("slope_vec2f((vec2f) { 1, 0 }) failed");
    }

    if (slope_vec2f((vec2f) to_vec2f(1, 1)) != 1)
    {
        FAIL_TEST("slope_vec2f((vec2f) { 1, 1 }) failed");
    }

    if (slope_vec2f((vec2f) to_vec2f(1, 2)) != 2)
    {
        FAIL_TEST("slope_vec2f((vec2f) { 1, 2 }) failed");
    }

    if (slope_vec2f((vec2f) to_vec2f(2, 1)) != 0.5f)
    {
        FAIL_TEST("slope_vec2f((vec2f) { 2, 1 }) failed");
    }

    if (slope_vec2f((vec2f) to_vec2f(0.25f, 0.125f)) != 0.5f)
    {
        FAIL_TEST("slope_vec2f((vec2f) {{{ 0.25f, 0.125f }) failed");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(perpendicular_vec2f)
{
    vec2f original = (vec2f) to_vec2f(0.16f, -0.32f);
    vec2f perpendicular = perpendicular_vec2f(original);
    vec2f expectedOriginal = neg_vec2f(perpendicular_vec2f(perpendicular));

    if (!equal_f(GET_X(original), GET_Y(perpendicular), DEFAULT_TOLERANCE) || !equal_f(GET_Y(original), -GET_X(perpendicular), DEFAULT_TOLERANCE))
    {
        FAIL_TEST("perpendicular_vec2f(original) did not correctly compute");
    }

    if (!equal_f(GET_X(perpendicular), -GET_Y(expectedOriginal), DEFAULT_TOLERANCE) || !equal_f(GET_Y(perpendicular), GET_X(expectedOriginal), DEFAULT_TOLERANCE))
    {
        FAIL_TEST("perpendicular_vec2f(perpendicular) did not correctly compute");
    }

    if (!equal_vec2f(original, expectedOriginal, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("2 perpendicular calculations did not return the original vector");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(radius_vec2f)
{
    vec2f v1 = to_vec2f(0.0f, 0.0f);
    vec2f v2 = to_vec2f(1.0f, 0);
    vec2f v3 = to_vec2f(0.7071f, -0.7071f);

    if (!equal_f(radius_vec2f(v1), 0.0f, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("radius_vec2f((x: 0, y: 0)) != 0");
    }

    if (!equal_f(radius_vec2f(v2), 1.0f, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("radius_vec2f((x: 1.0f, y: 0)) != 1");
    }

    if (!equal_f(radius_vec2f(v3), 1.0f, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("radius_vec2f((x: 0.7071f, y: -0.7071f)) != 1");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(intersect_vec2f)
{
    // -------------------------------------------------------------------------
    // Test 1: Same origin, same slope
    // -------------------------------------------------------------------------
    vec2f origin = to_vec2f(0.0f, 0.0f);
    float defaultSlope = 1;
    vec2f intersection = intersect_vec2f(origin, defaultSlope, origin, defaultSlope);

    if (!equal_vec2f(intersection, origin, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("Lines with the same origins did not intersect");
    }

    // -------------------------------------------------------------------------
    // Test 2: Perpendicular, diagonal slopes, same x coordinate values
    // -------------------------------------------------------------------------
    float slope2[2] = { 1, -1 };
    vec2f origin2[2] = { to_vec2f(0.0f, 0.0f), to_vec2f(0.0f, 1.4142f) };
    vec2f actualIntersection2;
    vec2f expectedIntersection2 = to_vec2f(0.7071f, 0.7071f);

    actualIntersection2 = intersect_vec2f(origin2[0], slope2[0], origin2[1], slope2[1]);
    if (!equal_vec2f(actualIntersection2, expectedIntersection2, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("Failed to project (0, 1.4142) onto a diagonal vector.");
    }

    PASS_TEST();

}
IMPLEMENT_TEST(distanceSqrd_vec2f)
{
    vec2f v1 = to_vec2f(0.0f, 0.0f);
    vec2f v2 = to_vec2f(2.0f, 0.0f);
    vec2f v3 = to_vec2f(3.0f, -3.0f);
    vec2f v4 = to_vec2f(0.7071f, -0.7071f);
    vec2f v5 = to_vec2f(-0.7071f, 0.7071f);

    if (!equal_f(distanceSqrd_vec2f(v1, v1), 0.0f, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("distanceSqrd_vec2f((x: 0, y: 0), (x: 0, y: 0)) != 0");
    }

    if (!equal_f(distanceSqrd_vec2f(v2, v1), 4.0f, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("distanceSqrd_vec2f((x: 2, y: 0), (x: 0, y: 0)) != 4");
    }

    if (!equal_f(distanceSqrd_vec2f(v1, v3), 18.0f, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("distanceSqrd_vec2f((x: 0, y: 0), (x: 3, y: -3)) != 18");
    }

    if (!equal_f(distanceSqrd_vec2f(v4, v5), 4.0f, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("distanceSqrd_vec2f((x: 0.7071f, y: -0.7071f), (x: -0.7071f, y: 0.7071f)) != 4");
    }

    PASS_TEST();
}

// enum LINEARITY linearity_vec2f(vec2f v1, vec2f v2, vec2f v3);
IMPLEMENT_TEST(linearity_vec2f)
{
    // clockwise
    vec2f clockwisePoints[] = {to_vec2f(.25f, .75f), to_vec2f(.5f, .5f), to_vec2f(-0.25f, -0.25f)};
    if (linearity_vec2f(clockwisePoints[0], clockwisePoints[1], clockwisePoints[2]) != LINEARITY_CLOCKWISE)
    {
        FAIL_TEST("Clockwise points are not clockwise");
    }

    // counter clockwise
    if (linearity_vec2f(clockwisePoints[2], clockwisePoints[1], clockwisePoints[0]) != LINEARITY_COUNTER_CLOCKWISE)
    {
        FAIL_TEST("Counter clockwise points are not counter clockwise");
    }

    // collinear
    vec2f collinearPoints[] = {to_vec2f(-1.0f, 0.0f), to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 0.0f)};
    if (linearity_vec2f(collinearPoints[0], collinearPoints[1], collinearPoints[2]) != LINEARITY_COLLINEAR)
    {
        FAIL_TEST("Collinear points are not collinear");
    }

    // floating point error collinear
    GET_Y(collinearPoints[1]) = 0.00001f; // simulate floating point error
    if (linearity_vec2f(collinearPoints[0], collinearPoints[1], collinearPoints[2]) != LINEARITY_COLLINEAR)
    {
        FAIL_TEST("Collinear points with floating point error are not collinear");
    }

    // same point
    if (linearity_vec2f(collinearPoints[0], collinearPoints[0], collinearPoints[0]) != LINEARITY_COLLINEAR)
    {
        FAIL_TEST("The same point should be collinear to itself");
    }

    PASS_TEST();
}

// bool areSegmentsIntersecting_vec2f(vec2f endpoint1_1, vec2f endpoint1_2, vec2f endpoint2_1, vec2f endpoint2_2);
IMPLEMENT_TEST(areSegmentsIntersecting_vec2f)
{
    // simple intersection
    vec2f segment1[] = {to_vec2f(0.25f, 0.75f), to_vec2f(0.75f, 0.25f)};
    vec2f segment2[] = {to_vec2f(0.25f, 0.25f), to_vec2f(0.75f, 0.75f)};

    if (!areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment2[0], segment2[1]) ||
        !areSegmentsIntersecting_vec2f(segment2[0], segment2[1], segment1[0], segment1[1]) ||
        !areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment2[1], segment2[0]) ||
        !areSegmentsIntersecting_vec2f(segment2[1], segment2[0], segment1[0], segment1[1]))
    {
        FAIL_TEST("Intersecting line segments did not register as intersecting");
    }

    // endpoint on line
    vec2f segment3[] = {to_vec2f(0.5f, 0.5f), to_vec2f(0.75f, 0.75f)};
    if (!areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment3[0], segment3[1]) ||
        !areSegmentsIntersecting_vec2f(segment3[0], segment3[1], segment1[0], segment1[1]) ||
        !areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment3[1], segment3[0]) ||
        !areSegmentsIntersecting_vec2f(segment3[1], segment3[0], segment1[0], segment1[1]))
    {
        FAIL_TEST("Intersecting line segments where one endpoint in on the line segment did not register as intersecting");
    }

    // endpoint on endpoint
    vec2f segment4[] = {to_vec2f(0.25f, 0.75f), to_vec2f(0.75f, 0.75f)};
    if (!areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment4[0], segment4[1]) ||
        !areSegmentsIntersecting_vec2f(segment4[0], segment4[1], segment1[0], segment1[1]) ||
        !areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment4[1], segment4[0]) ||
        !areSegmentsIntersecting_vec2f(segment4[1], segment4[0], segment1[0], segment1[1]))
    {
        FAIL_TEST("Intersecting line segments where one endpoint in on another endpoint did not register as intersecting");
    }

    // line projection intersects, but segment does not
    vec2f segment5[] = {to_vec2f(-0.25f, 0.75f), to_vec2f(0.25f, 1.25f)};
    if (areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment5[0], segment5[1]) ||
        areSegmentsIntersecting_vec2f(segment5[0], segment5[1], segment1[0], segment1[1]) ||
        areSegmentsIntersecting_vec2f(segment1[0], segment1[1], segment5[1], segment5[0]) ||
        areSegmentsIntersecting_vec2f(segment5[1], segment5[0], segment1[0], segment1[1]))
    {
        FAIL_TEST("2 non-intersecting line segments are detected as intersecting");
    }

    PASS_TEST();
}

#include "engine/util.h"

// vec2f project_vec2f(vec2f slope, vec2f point)
IMPLEMENT_TEST(project_vec2f)
{
    // -------------------------------------------------------------------------
    // 1: Horizontal vector, normalized, point to project at (2, 2)
    // -------------------------------------------------------------------------
    vec2f vecSlope1 = to_vec2f(1.0f, 0.0f);
    vec2f pointToProject1 = to_vec2f(2.0f, 2.0f);
    vec2f actualProjectedPoint1;
    vec2f expectedProjectedPoint1 = to_vec2f(2.0f, 0.0f);

    actualProjectedPoint1 = project_vec2f(vecSlope1, pointToProject1);

    if (!equal_vec2f(actualProjectedPoint1, expectedProjectedPoint1, DEFAULT_TOLERANCE))
    {
        char msg[320];
        int offset = 0;
        offset += sprintf(&msg[offset], "actualProjectedPoint1: ");
        offset += sprint_vec2f(&msg[offset], &actualProjectedPoint1);
        offset += sprintf(&msg[offset], ", expectedProjectedPoint1: ");
        offset += sprint_vec2f(&msg[offset], &expectedProjectedPoint1);
        printf("%s", msg);

        FAIL_TEST("Failed to project (2, 2) onto a horizontal vector.");
    }

    // -------------------------------------------------------------------------
    // 2: Vertical vector, normalized, point to project at (2, 2)
    // -------------------------------------------------------------------------
    vec2f vecSlope2 = to_vec2f(0.0f, 1.0f);
    vec2f pointToProject2 = to_vec2f(2.0f, 2.0f);
    vec2f actualProjectedPoint2;
    vec2f expectedProjectedPoint2 = to_vec2f(0.0f, 2.0f);

    actualProjectedPoint2 = project_vec2f(vecSlope2, pointToProject2);

    if (!equal_vec2f(actualProjectedPoint2, expectedProjectedPoint2, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("Failed to project (2, 2) onto a vertical vector.");
    }

    // -------------------------------------------------------------------------
    // 3: Diagonal vector, normalized, point to project at (0, 1.4142)
    // -------------------------------------------------------------------------
    vec2f vecSlope3 = to_vec2f(0.7071f, 0.7071f);
    vec2f pointToProject3 = to_vec2f(0.0f, 1.4142f);
    vec2f actualProjectedPoint3;
    vec2f expectedProjectedPoint3 = to_vec2f(0.7071f, 0.7071f);

    actualProjectedPoint3 = project_vec2f(vecSlope3, pointToProject3);

    if (!equal_vec2f(actualProjectedPoint3, expectedProjectedPoint3, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("Failed to project (0, 1.4142) onto a diagonal vector.");
    }

    PASS_TEST();
}

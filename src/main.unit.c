#include "engine/unit/math.unit.h"
#include "engine/unit/collision.unit.h"
#include "datastructures/unit/hashtable.unit.h"

FILE* UNIT_TEST_OUT;
FILE* UNIT_TEST_ERR;

void run_engine_math_tests()
{
    // float
    RUN_TEST(equal_f);
    RUN_TEST(perpendicular_f);

    // vec2f
    RUN_TEST(to_vec2f);
    RUN_TEST(add_vec2f);
    RUN_TEST(sub_vec2f);
    RUN_TEST(slope_vec2f);
    RUN_TEST(perpendicular_vec2f);
    RUN_TEST(radius_vec2f);
    RUN_TEST(intersect_vec2f);
    RUN_TEST(distanceSqrd_vec2f);
    RUN_TEST(linearity_vec2f);
    RUN_TEST(areSegmentsIntersecting_vec2f);
    RUN_TEST(project_vec2f);

    // polygon
    RUN_TEST(create_polygon);
    RUN_TEST(free_polygon);
    RUN_TEST(translate_polygon);
    RUN_TEST(decompose_polygon_quad);
    RUN_TEST(decompose_polygon_quadReflex);
    RUN_TEST(decompose_polygon_complexReflex);
    RUN_TEST(decompose_polygon_fewTowers);
    RUN_TEST(decompose_polygon_towers);
    RUN_TEST(project_polygon);
    RUN_TEST(_isDiagonal_polygon);
    RUN_TEST(_create_referenceVertices);
    RUN_TEST(_init_vertexDiagonal);
    RUN_TEST(_triangulate_polygon_quad);
    RUN_TEST(_triangulate_polygon_quadReflex);
    RUN_TEST(_triangulate_polygon_pentagon);
    RUN_TEST(_triangulate_polygon_complexReflex);
    RUN_TEST(_isEssential_vertexDiagonal);
    RUN_TEST(_isInessential_vertexDiagonal);

    // transform
    RUN_TEST(getMatrix_transform);
    RUN_TEST(_applymMatrix_vec2f);
    RUN_TEST(applyTransform_polygon);

    // matrix
    RUN_TEST(TRANSPOSE_MATRIX_FN(3, 1));

    RUN_TEST(MULTIPLY_MATRIX_FN(2, 2, 2));
    RUN_TEST(MULTIPLY_MATRIX_FN(3, 3, 3));
    RUN_TEST(MULTIPLY_MATRIX_FN(3, 3, 1));

    RUN_TEST(PAD_MATRIX_FN(2, 2, 3, 3));
    RUN_TEST(PAD_MATRIX_FN(2, 1, 3, 1));
    RUN_TEST(PAD_MATRIX_FN(3, 1, 2, 1));
}

void run_engine_collision_tests()
{
    // Internal function tests
    RUN_TEST(_detectCollision_polygon);

    // Public function tests
    RUN_TEST(create_collider_quad);
    RUN_TEST(detectCollision_collider);
}

void run_hashtable_tests()
{
    RUN_TEST(create_hashtable);
    RUN_TEST(setGet_hashtable);
}

int main(int argc, char* argv[])
{
    UNIT_TEST_OUT = stdout;
    UNIT_TEST_ERR = stderr;

    // engine/math
    run_engine_math_tests();

    // engine/collision
    run_engine_collision_tests();

    // datastructures/hashtable
    run_hashtable_tests();

    writeUnitTestReport();
}

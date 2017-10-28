#include "engine/unit/math/polygon.unit.h"

#include <stdlib.h>

#include "engine/util.h"
#include "engine/math/float.h"
#include "engine/math/vec.h"

IMPLEMENT_TEST(create_polygon)
{
    polygon poly;

    // -------------------------------------------------------------------------
    // Test 1: Can't create a polygon without at least 3 vertices
    // -------------------------------------------------------------------------
    if (create_polygon(&poly, 2))
    {
        free_polygon(&poly);

        FAIL_TEST("Created a polygon with < 3 vertices");
    }

    // -------------------------------------------------------------------------
    // Test 2: Can create a polygon with exactly 3 vertices
    // -------------------------------------------------------------------------
    if (!create_polygon(&poly, 3))
    {
        FAIL_TEST("Could not create a polygon with 3 vertices");
    }
    free_polygon(&poly);

    // -------------------------------------------------------------------------
    // Test 3: Can create a polygon with more than 3 vertices
    // -------------------------------------------------------------------------
    if (!create_polygon(&poly, 4))
    {
        FAIL_TEST("Could not create a polygon with more than 3 vertices");
    }
    free_polygon(&poly);

    // -------------------------------------------------------------------------
    // Test 4: Can't create a polygon with a null pointer
    // -------------------------------------------------------------------------
    if (create_polygon(NULL, 4))
    {
        FAIL_TEST("Created a polygon with a NULL pointer. This might be a memory leak.");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(free_polygon)
{
    polygon poly = { NULL, 0 };

    // -------------------------------------------------------------------------
    // Test 1: Can't free a NULL pointer
    // -------------------------------------------------------------------------
    if (free_polygon(NULL))
    {
        FAIL_TEST("Freed a null pointer");
    }

    // -------------------------------------------------------------------------
    // Test 2: Can't free an uninitialized polygon
    // -------------------------------------------------------------------------
    if (free_polygon(&poly))
    {
        FAIL_TEST("Freed an uninitialized polygon");
    }

    // -------------------------------------------------------------------------
    // Test 3: Can free a successfully initialized polygon
    // -------------------------------------------------------------------------
    create_polygon(&poly, 3);
    if (!free_polygon(&poly))
    {
        FAIL_TEST("Could not free a successfully created polygon");
    }

    PASS_TEST();
}

IMPLEMENT_TEST(translate_polygon)
{
    polygon basePolygon, expectedPolygon, actualPolygon;

    create_polygon(&basePolygon, 3);
    create_polygon(&expectedPolygon, 3);
    create_polygon(&actualPolygon, 3);

    basePolygon.vertices[0] = to_vec2f(0.0f, 0.0f);
    basePolygon.vertices[1] = to_vec2f(1.0f, 0.0f);
    basePolygon.vertices[2] = to_vec2f(0.0f, 1.0f);

    // -------------------------------------------------------------------------
    // Test 1: translate (0, 0)
    // -------------------------------------------------------------------------
    expectedPolygon.vertices[0] = to_vec2f(0.0f, 0.0f);
    expectedPolygon.vertices[1] = to_vec2f(1.0f, 0.0f);
    expectedPolygon.vertices[2] = to_vec2f(0.0f, 1.0f);
    if (!translate_polygon(&basePolygon, &actualPolygon, to_vec2f(0.0f, 0.0f)))
    {
        free_polygon(&basePolygon);
        free_polygon(&expectedPolygon);
        free_polygon(&actualPolygon);

        FAIL_TEST("translate_polygon failed with all valid arguments");
    }

    if (!equal_vec2f(actualPolygon.vertices[0], expectedPolygon.vertices[0], DEFAULT_TOLERANCE) ||
        !equal_vec2f(actualPolygon.vertices[1], expectedPolygon.vertices[1], DEFAULT_TOLERANCE) ||
        !equal_vec2f(actualPolygon.vertices[2], expectedPolygon.vertices[2], DEFAULT_TOLERANCE))
    {
        free_polygon(&basePolygon);
        free_polygon(&expectedPolygon);
        free_polygon(&actualPolygon);

        FAIL_TEST("translate_polygon failed with a (0, 0) translation");
    }

    // -------------------------------------------------------------------------
    // Test 2: translate (1, -1)
    // -------------------------------------------------------------------------
    expectedPolygon.vertices[0] = to_vec2f(1.0f, -1.0f);
    expectedPolygon.vertices[1] = to_vec2f(2.0f, -1.0f);
    expectedPolygon.vertices[2] = to_vec2f(1.0f, 0.0f);

    if (!translate_polygon(&basePolygon, &actualPolygon, to_vec2f(1.0f, -1.0f)))
    {
        free_polygon(&basePolygon);
        free_polygon(&expectedPolygon);
        free_polygon(&actualPolygon);

        FAIL_TEST("translate_polygon failed with all valid arguments");
    }

    if (!equal_vec2f(actualPolygon.vertices[0], expectedPolygon.vertices[0], DEFAULT_TOLERANCE) ||
        !equal_vec2f(actualPolygon.vertices[1], expectedPolygon.vertices[1], DEFAULT_TOLERANCE) ||
        !equal_vec2f(actualPolygon.vertices[2], expectedPolygon.vertices[2], DEFAULT_TOLERANCE))
    {
        free_polygon(&basePolygon);
        free_polygon(&expectedPolygon);
        free_polygon(&actualPolygon);

        FAIL_TEST("translate_polygon failed with a (1, -1) translation");
    }

    free_polygon(&basePolygon);
    free_polygon(&expectedPolygon);
    free_polygon(&actualPolygon);

    PASS_TEST();
}

// bool _isDiagonal_polygon(polygon* poly, referenceVertex* earVertices, int baseVertexIndex, int diagonalVertexIndex)
IMPLEMENT_TEST(_isDiagonal_polygon)
{
    // Square
    polygon poly;
    vec2f squareVertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(0.0f, 1.0f)};
    poly.vertexCount = 4;
    poly.vertices = squareVertices;

    referenceVertex squareEarVertices[] = {{3, 1, true}, {0, 2, true}, {1, 3, true}, {2, 0, true}};

    if (!_isDiagonal_polygon(&poly, squareEarVertices, 0, 2) || !_isDiagonal_polygon(&poly, squareEarVertices, 2, 0))
    {
        FAIL_TEST("Square polygon indices 0, 2 should be a diagaonal");
    }
    else if (!_isDiagonal_polygon(&poly, squareEarVertices, 1, 3) || !_isDiagonal_polygon(&poly, squareEarVertices, 3, 1))
    {
        FAIL_TEST("Square polygon indices 1, 3 should be a diagaonal");
    }

    // 4-vertex with reflex
    vec2f quadReflexVertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(2.0f, 0.0f), to_vec2f(1.0f, 2.0f)};
    poly.vertexCount = 4;
    poly.vertices = quadReflexVertices;

    referenceVertex quadReflexEarVertices[] = {{3, 1, true}, {0, 2, false}, {1, 3, true}, {2, 0, false}};

    if (_isDiagonal_polygon(&poly, quadReflexEarVertices, 0, 2))
    {
        FAIL_TEST("Quad polygon, with a reflex vertex: indices 0, 2 should not be a diagaonal");
    }
    else if (_isDiagonal_polygon(&poly, quadReflexEarVertices, 2, 0))
    {
        FAIL_TEST("Quad polygon, with a reflex vertex: indices 2, 0 should not be a diagaonal");
    }
    else if (!_isDiagonal_polygon(&poly, quadReflexEarVertices, 1, 3))
    {
        FAIL_TEST("Quad polygon, with a reflex vertex: indices 1, 3 should be a diagaonal");
    }
    else if (!_isDiagonal_polygon(&poly, quadReflexEarVertices, 3, 1))
    {
        FAIL_TEST("Quad polygon, with a reflex vertex: indices 3, 1 should be a diagaonal");
    }

    // Internal, but intersects segment
    vec2f pentaReflexVertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(2.0f, 0.0f), to_vec2f(2.0f, 1.0f), to_vec2f(1.0f, 2.0f)};
    poly.vertexCount = 5;
    poly.vertices = pentaReflexVertices;

    referenceVertex pentaReflexEarVertices[] = {{4, 1, true}, {0, 2, false}, {1, 3, true}, {2, 4, true}, {3, 0, false}};

    if (_isDiagonal_polygon(&poly, pentaReflexEarVertices, 0, 2))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 0, 2 should not be a diagaonal");
    }
    else if (_isDiagonal_polygon(&poly, pentaReflexEarVertices, 2, 0))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 2, 0 should not be a diagaonal");
    }
    else if (_isDiagonal_polygon(&poly, pentaReflexEarVertices, 0, 3))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 0, 3 should not be a diagaonal");
    }
    else if (_isDiagonal_polygon(&poly, pentaReflexEarVertices, 3, 0))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 3, 0 should not be a diagaonal");
    }
    else if (!_isDiagonal_polygon(&poly, pentaReflexEarVertices, 1, 4))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 1, 4 should be a diagaonal");
    }
    else if (!_isDiagonal_polygon(&poly, pentaReflexEarVertices, 4, 1))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 4, 1 should be a diagaonal");
    }
    else if (!_isDiagonal_polygon(&poly, pentaReflexEarVertices, 2, 4))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 2, 4 should be a diagaonal");
    }
    else if (!_isDiagonal_polygon(&poly, pentaReflexEarVertices, 4, 2))
    {
        FAIL_TEST("Penta polygon, with a reflex vertex: indices 4, 2 should be a diagaonal");
    }

    PASS_TEST();
}

// referenceVertex* _create_referenceVertices(polygon* poly)
IMPLEMENT_TEST(_create_referenceVertices)
{
    vec2f quadReflexVertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(2.0f, 0.0f), to_vec2f(1.0f, 2.0f)};
    polygon poly;
    poly.vertexCount = 4;
    poly.vertices = quadReflexVertices;

    referenceVertex* actual = _create_referenceVertices(&poly);
    referenceVertex expected[] = {{3, 1, true}, {0, 2, false}, {1, 3, true}, {2, 0, false}};

    char failMessage[80];
    bool failedTest = false;
    for (int i = 0; i < poly.vertexCount; i++)
    {
        if (actual[i].prev != expected[i].prev)
        {
            failedTest = true;
            sprintf(failMessage, "referenceVertex[%d]: actual.prev (%d) != expected.prev (%d)", i, actual[i].prev, expected[i].prev);
        }
        else if (actual[i].next != expected[i].next)
        {
            failedTest = true;
            sprintf(failMessage, "referenceVertex[%d]: actual.next (%d) != expected.next (%d)", i, actual[i].next, expected[i].next);
        }
        else if (actual[i].isEar != expected[i].isEar)
        {
            failedTest = true;
            sprintf(failMessage, "referenceVertex[%d]: actual.isEar (%d) != expected.isEar (%d)", i, actual[i].isEar, expected[i].isEar);
        }

        if (failedTest) {
            break;
        }
    }

    free(actual);

    if (failedTest)
    {
        FAIL_TEST(failMessage);
    }
    else
    {
        PASS_TEST();
    }
}

// vertexDiagonal* _init_vertexDiagonal(vertexDiagonal* diagonal, vertexDiagonal* prev, vertexDiagonal* next, int index)
IMPLEMENT_TEST(_init_vertexDiagonal)
{
    const int PREV_DIAGONAL = 0;
    const int INIT_DIAGONAL = 1;
    const int NEXT_DIAGONAL = 2;
    vertexDiagonal sides[3];
    int index = 5;

    sides[PREV_DIAGONAL].next = &sides[NEXT_DIAGONAL];
    sides[NEXT_DIAGONAL].prev = &sides[PREV_DIAGONAL];

    vertexDiagonal* initActual = _init_vertexDiagonal(&sides[INIT_DIAGONAL], &sides[PREV_DIAGONAL], &sides[NEXT_DIAGONAL], index);

    if (initActual != &sides[INIT_DIAGONAL])
    {
        FAIL_TEST("A new memory was allocated where none was expected");
    }
    else if (sides[PREV_DIAGONAL].next == &sides[NEXT_DIAGONAL])
    {
        FAIL_TEST("side[PREV_DIAGONAL].next == &sides[NEXT_DIAGONAL]. It should be &sides[INIT_DIAGONAL]");
    }
    else if (sides[PREV_DIAGONAL].next != &sides[INIT_DIAGONAL])
    {
        FAIL_TEST("side[PREV_DIAGONAL].next == &sides[INIT_DIAGONAL]. It is connected to an unknown diagonal");
    }
    else if (sides[NEXT_DIAGONAL].prev == &sides[PREV_DIAGONAL])
    {
        FAIL_TEST("side[NEXT_DIAGONAL].prev == &sides[PREV_DIAGONAL]. It should be &sides[INIT_DIAGONAL]");
    }
    else if (sides[NEXT_DIAGONAL].prev != &sides[INIT_DIAGONAL])
    {
        FAIL_TEST("side[NEXT_DIAGONAL].prev == &sides[INIT_DIAGONAL]. It is connected to an unknown diagonal");
    }
    else if (sides[INIT_DIAGONAL].index != index)
    {
        FAIL_TEST("sides[INIT_DIAGONAL].endpoint is not set properly");
    }

    PASS_TEST();
}

bool verifyDiagonalData(int diagonalIndex, vertexDiagonal* actualDiagonal, vertexDiagonal* expectedDiagonal, char resultMsg[80])
{
    if (!actualDiagonal && expectedDiagonal)
    {
        sprintf(resultMsg, "diagonal[%d] should not be NULL", diagonalIndex);
        return false;
    }
    else if (actualDiagonal && !expectedDiagonal)
    {
        sprintf(resultMsg, "diagonal[%d] should be NULL", diagonalIndex);
        return false;
    }
    else if (actualDiagonal->index != expectedDiagonal->index)
    {
        sprintf(resultMsg, "diagonal[%d] index: %d, expected: %d", diagonalIndex, actualDiagonal->index, expectedDiagonal->index);
        return false;
    }
    else if (!actualDiagonal->opposite)
    {
        sprintf(resultMsg, "diagonal[%d] opposite link is NULL", diagonalIndex);
        return false;
    }
    else if (actualDiagonal->opposite->opposite != actualDiagonal)
    {
        sprintf(resultMsg, "diagonal[%d] opposite diagonal not linked to the original", diagonalIndex);
        return false;
    }
    else
    {
        return true;
    }
}

bool verifyDiagonal(int diagonalIndex, vertexDiagonal* actualDiagonal, vertexDiagonal* expectedDiagonal, char resultMsg[80])
{
    if (!actualDiagonal && !expectedDiagonal)
    {
        return true;
    }
    else if (!verifyDiagonalData(diagonalIndex, actualDiagonal, expectedDiagonal, resultMsg))
    {
        return false;
    }
    else
    {
        // Actual
        vertexDiagonal* actualPrev = actualDiagonal->prev;
        vertexDiagonal* actualNext = actualDiagonal->next;

        // Expected
        vertexDiagonal* expectedPrev = expectedDiagonal->prev;
        vertexDiagonal* expectedNext = expectedDiagonal->next;

        // Verify all previous vertexDiagonal*
        while (actualPrev || expectedPrev)
        {
            if (!verifyDiagonalData(diagonalIndex, actualPrev, expectedPrev, resultMsg))
            {
                return false;
            }

            actualPrev = actualPrev->prev;
            expectedPrev = expectedPrev->prev;
        }

        // Verify all next vertexDiagonal*
        while (actualNext || expectedNext)
        {
            if (!verifyDiagonalData(diagonalIndex, actualNext, expectedNext, resultMsg))
            {
                return false;
            }

            actualNext = actualNext->next;
            expectedNext = expectedNext->next;
        }

        return true;
    }
}

// resultMsg should be at least 80 characters pre-allocated memory
bool verifyDiagonals(int diagonalCount, vertexDiagonal** actualDiagonals, vertexDiagonal** expectedDiagonals, char resultMsg[80])
{
    for (int i = 0; i < diagonalCount; i++)
    {
        if (!verifyDiagonal(i, actualDiagonals[i], expectedDiagonals[i], resultMsg))
        {
            return false;
        }
    }

    return true;
}

// vertexDiagonal** _triangulate_polygon(polygon* poly, vertexDiagonal** diagonalPool)
IMPLEMENT_TEST(_triangulate_polygon_quad)
{
    vec2f vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(0.0f, 1.0f)};
    polygon poly;
    poly.vertexCount = 4;
    poly.vertices = vertices;

    vertexDiagonal* actualDiagonalPool;
    vertexDiagonal** actualDiagonals;

    actualDiagonals = _triangulate_polygon(&poly, &actualDiagonalPool);

    bool result = true;
    char resultMsg[80];
    if (actualDiagonals[1] && actualDiagonals[3])
    {
        vertexDiagonal expectedDiagonalPool[2] = {{1, NULL, NULL, NULL}, {3, NULL, NULL, NULL}};
        expectedDiagonalPool[0].opposite = &expectedDiagonalPool[1];
        expectedDiagonalPool[1].opposite = &expectedDiagonalPool[0];
        vertexDiagonal* expectedDiagonals[4] = {NULL, &expectedDiagonalPool[0], NULL, &expectedDiagonalPool[1]};

        result = verifyDiagonals(poly.vertexCount, actualDiagonals, expectedDiagonals, resultMsg);
    }
    else
    {
        result = false;
        strcpy(resultMsg, "pairs of diagonals at opposite vertices were not created");
    }

    free(actualDiagonalPool);
    free(actualDiagonals);

    if (result)
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

// vertexDiagonal** _triangulate_polygon(polygon* poly, vertexDiagonal** diagonalPool)
IMPLEMENT_TEST(_triangulate_polygon_quadReflex)
{
    vec2f vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(2.0f, 0.0f), to_vec2f(1.0f, 2.0f)};
    polygon poly;
    poly.vertexCount = 4;
    poly.vertices = vertices;

    vertexDiagonal* actualDiagonalPool;
    vertexDiagonal** actualDiagonals;

    actualDiagonals = _triangulate_polygon(&poly, &actualDiagonalPool);

    bool result;
    char resultMsg[80];
    if (actualDiagonals[1] && actualDiagonals[3])
    {
        vertexDiagonal expectedDiagonalPool[2] = {{1, false, NULL, NULL}, {3, false, NULL, NULL}};
        expectedDiagonalPool[0].opposite = &expectedDiagonalPool[1];
        expectedDiagonalPool[1].opposite = &expectedDiagonalPool[0];
        vertexDiagonal* expectedDiagonals[4] = {NULL, &expectedDiagonalPool[0], NULL, &expectedDiagonalPool[1]};

        result = verifyDiagonals(poly.vertexCount, actualDiagonals, expectedDiagonals, resultMsg);
    }
    else
    {
        result = false;
        sprintf(resultMsg, "The only valid diagonal between 1 and 3 was not found");
    }

    free(actualDiagonalPool);
    free(actualDiagonals);

    if (result)
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

// vertexDiagonal** _triangulate_polygon(polygon* poly, vertexDiagonal** diagonalPool)
IMPLEMENT_TEST(_triangulate_polygon_pentagon)
{
    vec2f vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(2.0f, 0.0f), to_vec2f(3.0f, 1.0f), to_vec2f(1.0f, 2.0f), to_vec2f(-1.0f, 1.0f)};
    polygon poly;
    poly.vertexCount = 5;
    poly.vertices = vertices;

    vertexDiagonal* actualDiagonalPool;
    vertexDiagonal** actualDiagonals;

    actualDiagonals = _triangulate_polygon(&poly, &actualDiagonalPool);

    bool result;
    char resultMsg[80];
    const char* INVALID_DIAGONAL_SET = "A valid diagonal set was not found";

    /*
                3
               / \
              /   \
             4     2
             |     |
             |     |
             0-----1

        Valid diagonal sets
            Base zero
            * 0 <-> 2, 0 <-> 3
            * 0 <-> 2, 2 <-> 4
            * 0 <-> 3, 1 <-> 3

            Base one
            * 1 <-> 4, 2 <-> 4: should be the first diagonal set calculated
            * 1 <-> 3, 1 <-> 4
    */

    if (actualDiagonals[1] && actualDiagonals[2] && actualDiagonals[4])
    {
        vertexDiagonal expectedDiagonalPool[4] = {{1, NULL, NULL, NULL}, {4, NULL, NULL, NULL}, {2, NULL, NULL, NULL}, {4, NULL, NULL, NULL}};

        expectedDiagonalPool[0].opposite = &expectedDiagonalPool[1];
        expectedDiagonalPool[1].opposite = &expectedDiagonalPool[0];

        expectedDiagonalPool[2].opposite = &expectedDiagonalPool[3];
        expectedDiagonalPool[3].opposite = &expectedDiagonalPool[2];

        expectedDiagonalPool[1].prev = &expectedDiagonalPool[3];
        expectedDiagonalPool[3].next = &expectedDiagonalPool[1];

        vertexDiagonal* expectedDiagonals[] = {NULL, &expectedDiagonalPool[0], &expectedDiagonalPool[2], NULL, &expectedDiagonalPool[3]};

        result = verifyDiagonals(poly.vertexCount, actualDiagonals, expectedDiagonals, resultMsg);
    }
    else
    {
        result = false;
        sprintf(resultMsg, "%s", INVALID_DIAGONAL_SET);
    }

    free(actualDiagonalPool);
    free(actualDiagonals);

    if (result)
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

// vertexDiagonal** _triangulate_polygon(polygon* poly, vertexDiagonal** diagonalPool)
IMPLEMENT_TEST(_triangulate_polygon_complexReflex)
{
    /*
        Polygon is roughly this shape

        4
          \ *
            \  *
             5    *
             |       *
             |          *
             |             *
             0                *
            /                   *
          /                       3 ----- 2
        1 * * * * * * * * * * * * * * * *
    */
    vec2f vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(-2.0f, -2.0f), to_vec2f(3.0f, -1.0f), to_vec2f(2.0f, -1.0f), to_vec2f(-2.0f, 5.0f), to_vec2f(0.0f, 1.0f)};
    polygon poly;
    poly.vertexCount = 6;
    poly.vertices = vertices;

    vertexDiagonal* actualDiagonalPool;
    vertexDiagonal** actualDiagonals;

    actualDiagonals = _triangulate_polygon(&poly, &actualDiagonalPool);

    bool result;
    char resultMsg[80];
    const char* INVALID_DIAGONAL_SET = "A valid diagonal set was not found";

    if (actualDiagonals[0] && actualDiagonals[1] && actualDiagonals[3] && actualDiagonals[5])
    {
        vertexDiagonal expectedDiagonalPool[6] = {{1, false, NULL, NULL}, {3, false, NULL, NULL}, {5, false, NULL, NULL}, {3, false, NULL, NULL}, {0, false, NULL, NULL}, {3, false, NULL, NULL}};
        expectedDiagonalPool[0].opposite = &expectedDiagonalPool[1];
        expectedDiagonalPool[1].opposite = &expectedDiagonalPool[0];
        expectedDiagonalPool[2].opposite = &expectedDiagonalPool[3];
        expectedDiagonalPool[3].opposite = &expectedDiagonalPool[2];
        expectedDiagonalPool[4].opposite = &expectedDiagonalPool[5];
        expectedDiagonalPool[5].opposite = &expectedDiagonalPool[4];

        expectedDiagonalPool[1].next = &expectedDiagonalPool[5];
        expectedDiagonalPool[5].prev = &expectedDiagonalPool[1];
        expectedDiagonalPool[5].next = &expectedDiagonalPool[3];
        expectedDiagonalPool[3].prev = &expectedDiagonalPool[5];

        vertexDiagonal* expectedDiagonals[6] = {&expectedDiagonalPool[4], &expectedDiagonalPool[0], NULL, &expectedDiagonalPool[5], NULL, &expectedDiagonalPool[2]};

        result = verifyDiagonals(poly.vertexCount, actualDiagonals, expectedDiagonals, resultMsg);
    }
    else
    {
        result = false;
        sprintf(resultMsg, "%s", INVALID_DIAGONAL_SET);
    }

    free(actualDiagonalPool);
    free(actualDiagonals);

    if (result)
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

// bool _isEssential_vertexDiagonal(polygon* poly, vertexDiagonal* base)
IMPLEMENT_TEST(_isEssential_vertexDiagonal)
{
    // Essential: false
    vec2f p1Vertices[] = {to_vec2f(-1.0f, 0.0f), to_vec2f(0.0f, -1.0f), to_vec2f(1.0f, -1.0f), to_vec2f(1.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(0.0f, 1.0f)};
    polygon p1;
    p1.vertexCount = 6;
    p1.vertices = p1Vertices;

    // No other diagonals
    vertexDiagonal d1[2];
    d1[0].index = 0;
    d1[0].prev = NULL;
    d1[0].next = NULL;
    d1[0].opposite = &d1[1];

    d1[1].index = 3;
    d1[1].prev = NULL;
    d1[1].next = NULL;
    d1[1].opposite = &d1[0];

    if (_isEssential_vertexDiagonal(&p1, &d1[0]))
    {
        FAIL_TEST("Diagonal should not be essential.");
    }

    // Prev diagonal
    vertexDiagonal d2[4];
    d2[0].index = 0;
    d2[0].prev = &d2[1];
    d2[0].next = NULL;
    d2[0].opposite = &d2[2];

    d2[1].index = 0;
    d2[1].prev = NULL;
    d2[1].next = &d2[0];
    d2[1].opposite = &d2[3];

    d2[2].index = 3;
    d2[2].prev = NULL;
    d2[2].next = NULL;
    d2[2].opposite = &d2[0];

    d2[3].index = 4;
    d2[3].prev = NULL;
    d2[3].next = NULL;
    d2[3].opposite = &d2[1];

    if (_isEssential_vertexDiagonal(&p1, &d2[0]))
    {
        FAIL_TEST("Diagonal should not be essential.");
    }

    // Next diagonal
    vertexDiagonal d3[4];
    d3[0].index = 0;
    d3[0].prev = NULL;
    d3[0].next = &d3[1];
    d3[0].opposite = &d3[2];

    d3[1].index = 0;
    d3[1].prev = &d3[0];
    d3[1].next = NULL;
    d3[1].opposite = &d3[3];

    d3[2].index = 3;
    d3[2].prev = NULL;
    d3[2].next = NULL;
    d3[2].opposite = &d3[0];

    d3[3].index = 2;
    d3[3].prev = NULL;
    d3[3].next = NULL;
    d3[3].opposite = &d3[1];

    if (_isEssential_vertexDiagonal(&p1, &d3[0]))
    {
        FAIL_TEST("Diagonal should not be essential.");
    }

    // Both diagonals
    vertexDiagonal d4[6];
    d4[0].index = 0;
    d4[0].prev = &d4[1];
    d4[0].next = &d4[2];
    d4[0].opposite = &d4[3];

    d4[1].index = 0;
    d4[1].prev = NULL;
    d4[1].next = &d4[0];
    d4[1].opposite = &d4[4];

    d4[2].index = 0;
    d4[2].prev = &d4[0];
    d4[2].next = NULL;
    d4[2].opposite = &d4[5];

    d4[3].index = 3;
    d4[3].prev = NULL;
    d4[3].next = NULL;
    d4[3].opposite = &d4[0];

    d4[4].index = 4;
    d4[4].prev = NULL;
    d4[4].next = NULL;
    d4[4].opposite = &d4[1];

    d4[5].index = 2;
    d4[5].prev = NULL;
    d4[5].next = NULL;
    d4[5].opposite = &d4[2];

    if (_isEssential_vertexDiagonal(&p1, &d4[0]))
    {
        FAIL_TEST("Diagonal should not be essential.");
    }

    // Essential: true
    vec2f p2Vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(2.0f, -2.0f), to_vec2f(2.0f, -1.0f), to_vec2f(0.0f, 1.0f), to_vec2f(-2.0f, -1.0f), to_vec2f(-2.0f, -2.0f)};
    polygon p2;
    p2.vertexCount = 6;
    p2.vertices = p2Vertices;

    // No other diagonals
    vertexDiagonal d5[2];
    d5[0].index = 0;
    d5[0].prev = NULL;
    d5[0].next = NULL;
    d5[0].opposite = &d5[1];

    d5[1].index = 3;
    d5[1].prev = NULL;
    d5[1].next = NULL;
    d5[1].opposite = &d5[0];

    if (!_isEssential_vertexDiagonal(&p2, &d5[0]))
    {
        FAIL_TEST("Diagonal should be essential.");
    }

    // Prev diagonal
    vertexDiagonal d6[4];
    d6[0].index = 0;
    d6[0].prev = &d6[1];
    d6[0].next = NULL;
    d6[0].opposite = &d6[2];

    d6[1].index = 0;
    d6[1].prev = NULL;
    d6[1].next = &d2[0];
    d6[1].opposite = &d6[3];

    d6[2].index = 3;
    d6[2].prev = NULL;
    d6[2].next = NULL;
    d6[2].opposite = &d6[0];

    d6[3].index = 4;
    d6[3].prev = NULL;
    d6[3].next = NULL;
    d6[3].opposite = &d6[1];

    if (!_isEssential_vertexDiagonal(&p2, &d6[0]))
    {
        FAIL_TEST("Diagonal should be essential.");
    }

    // Next diagonal
    vertexDiagonal d7[4];
    d7[0].index = 0;
    d7[0].prev = NULL;
    d7[0].next = &d7[1];
    d7[0].opposite = &d7[2];

    d7[1].index = 0;
    d7[1].prev = &d7[0];
    d7[1].next = NULL;
    d7[1].opposite = &d7[3];

    d7[2].index = 3;
    d7[2].prev = NULL;
    d7[2].next = NULL;
    d7[2].opposite = &d7[0];

    d7[3].index = 2;
    d7[3].prev = NULL;
    d7[3].next = NULL;
    d7[3].opposite = &d7[1];

    if (!_isEssential_vertexDiagonal(&p2, &d7[0]))
    {
        FAIL_TEST("Diagonal should not be essential.");
    }

    // Both diagonals
    vertexDiagonal d8[6];
    d8[0].index = 0;
    d8[0].prev = &d8[1];
    d8[0].next = &d8[2];
    d8[0].opposite = &d8[3];

    d8[1].index = 0;
    d8[1].prev = NULL;
    d8[1].next = &d8[0];
    d8[1].opposite = &d8[4];

    d8[2].index = 0;
    d8[2].prev = &d8[0];
    d8[2].next = NULL;
    d8[2].opposite = &d8[5];

    d8[3].index = 3;
    d8[3].prev = NULL;
    d8[3].next = NULL;
    d8[3].opposite = &d8[0];

    d8[4].index = 4;
    d8[4].prev = NULL;
    d8[4].next = NULL;
    d8[4].opposite = &d8[1];

    d8[5].index = 2;
    d8[5].prev = NULL;
    d8[5].next = NULL;
    d8[5].opposite = &d8[2];

    if (!_isEssential_vertexDiagonal(&p2, &d8[0]))
    {
        FAIL_TEST("Diagonal should not be essential.");
    }

    PASS_TEST();
}

// bool _isInessential_vertexDiagonal(polygon* poly, vertexDiagonal* base)
IMPLEMENT_TEST(_isInessential_vertexDiagonal)
{
    vec2f vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(0.0f, 2.0f), to_vec2f(-1.0f, 4.0f), to_vec2f(-2.0f, 3.0f), to_vec2f(-2.0f, -1.0f), to_vec2f(-1.0f, -2.0f)};
    polygon p;
    p.vertexCount = 7;
    p.vertices = vertices;

    vertexDiagonal d[8];

    // Both essential
    d[0].index = 0;
    d[0].prev = NULL;
    d[0].next = NULL;
    d[0].opposite = &d[1];

    d[1].index = 0;
    d[1].prev = NULL;
    d[1].next = NULL;
    d[1].opposite = &d[0];

    if (_isInessential_vertexDiagonal(&p, &d[0]) || _isInessential_vertexDiagonal(&p, &d[1]))
    {
        FAIL_TEST("Both diagonal sides are essential, but the diagonal is not");
    }

    // One essential
    d[2].index = 2;
    d[2].prev = NULL;
    d[2].next = NULL;
    d[2].opposite = &d[3];

    d[3].index = 4;
    d[3].prev = NULL;
    d[3].next = NULL;
    d[3].opposite = &d[2];

    if (_isInessential_vertexDiagonal(&p, &d[2]) || _isInessential_vertexDiagonal(&p, &d[3]))
    {
        FAIL_TEST("One diagonal side is essential, but the diagonal is not");
    }

    // Neither essential
    d[4].index = 4;
    d[4].prev = NULL;
    d[4].next = NULL;
    d[4].opposite = &d[5];

    d[5].index = 6;
    d[5].prev = NULL;
    d[5].next = NULL;
    d[5].opposite = &d[4];

    if (!_isInessential_vertexDiagonal(&p, &d[4]) || !_isInessential_vertexDiagonal(&p, &d[5]))
    {
        FAIL_TEST("No diagonal side is essential, but the diagonal is essential");
    }

    PASS_TEST();
}

bool verifyPolygon(polygon* actual, polygon* expected, char resultMsg[320], int initialOffset)
{
    if (actual->vertexCount != expected->vertexCount)
    {
        sprintf(resultMsg, "Vertex count does not match. Actual %d, expected %d", actual->vertexCount, expected->vertexCount);
        return false;
    }
    else
    {
        for (int i = 0; i < actual->vertexCount; i++)
        {
            if (!equal_vec2f(actual->vertices[i], expected->vertices[i], DEFAULT_TOLERANCE))
            {
                int offset = initialOffset;
                offset += sprintf(&resultMsg[offset], "Vertices do not match.\nActual:   ");
                offset += sprint_polygon(&resultMsg[offset], actual);
                offset += sprintf(&resultMsg[offset], "\nExpected: ");
                offset += sprint_polygon(&resultMsg[offset], expected);
                return false;
            }
        }
    }

    return true;
}

bool verifyPolygons(polygon* actual, int actualCount, polygon* expected, int expectedCount, char resultMsg[320])
{
    if (actualCount != expectedCount)
    {
        sprintf(resultMsg, "Generated the incorrect number of convex polygons. Actual: %d, Expected: %d\n", actualCount, expectedCount);

        return false;
    }
    else
    {
        int offset;
        for (int i = 0; i < actualCount; i++)
        {
            offset = sprintf(resultMsg, "Polygon[%d]: ", i);
            if (!verifyPolygon(&actual[i], &expected[i], resultMsg, offset))
            {
                return false;
            }
        }
    }

    return true;
}

// bool decompose_polygon(polygon* in, polygon** out, int* outCount)
IMPLEMENT_TEST(decompose_polygon_quad)
{
    vec2f vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(0.0f, 1.0f)};
    polygon expected;
    expected.vertexCount = 4;
    expected.vertices = vertices;

    polygon* actual;
    int actualCount;

    if (!decompose_polygon(&expected, &actual, &actualCount))
    {
        FAIL_TEST("Memory allocation failure");
    }

    char resultMsg[320];
    if (verifyPolygons(actual, actualCount, &expected, 1, resultMsg))
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

// bool decompose_polygon(polygon* in, polygon** out, int* outCount)
IMPLEMENT_TEST(decompose_polygon_quadReflex)
{
    vec2f vertices[] = {to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(2.0f, 0.0f), to_vec2f(1.0f, 2.0f)};
    polygon poly;
    poly.vertexCount = 4;
    poly.vertices = vertices;

    vec2f expectedVertices[2][3] = {
        {
            to_vec2f(0.0f, 0.0f), to_vec2f(1.0f, 1.0f), to_vec2f(1.0f, 2.0f)
        },
        {
            to_vec2f(1.0f, 1.0f), to_vec2f(2.0f, 0.0f), to_vec2f(1.0f, 2.0f)
        }
    };
    polygon expected[2];
    expected[0].vertexCount = 3;
    expected[0].vertices = expectedVertices[0];

    expected[1].vertexCount = 3;
    expected[1].vertices = expectedVertices[1];

    polygon* actual;
    int actualCount;

    if (!decompose_polygon(&poly, &actual, &actualCount))
    {
        FAIL_TEST("Memory allocation failure");
    }

    char resultMsg[320];
    if (verifyPolygons(actual, actualCount, expected, 2, resultMsg))
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

IMPLEMENT_TEST(decompose_polygon_complexReflex)
{
    /*
        Polygon is roughly this shape

        4
          \ *
            \  *
             5    *
           /         *
          /             *
         /                 *
        0                     *
        |                       *
        |                         3 ----- 2
        1 * * * * * * * * * * * * * * * *
    */
    vec2f vertices[] = {to_vec2f(-2.0f, 0.0f), to_vec2f(-2.0f, -2.0f), to_vec2f(3.0f, -1.0f), to_vec2f(2.0f, -1.0f), to_vec2f(-2.0f, 5.0f), to_vec2f(0.0f, 1.0f)};
    polygon poly;
    poly.vertexCount = 6;
    poly.vertices = vertices;

    vec2f expectedVertices1[4] = {to_vec2f(-2.0f, 0.0f), to_vec2f(-2.0f, -2.0f), to_vec2f(2.0f, -1.0f), to_vec2f(0.0f, 1.0f)}; // 0, 1, 3, 5
    vec2f expectedVertices2[3] = {to_vec2f(-2.0f, -2.0f), to_vec2f(3.0f, -1.0f), to_vec2f(2.0f, -1.0f)}; // 1, 2, 3
    vec2f expectedVertices3[3] = {to_vec2f(2.0f, -1.0f), to_vec2f(-2.0f, 5.0f), to_vec2f(0.0f, 1.0f)}; // 3, 4, 5

    polygon expected[3];
    expected[0].vertexCount = 4;
    expected[0].vertices = expectedVertices1;

    expected[1].vertexCount = 3;
    expected[1].vertices = expectedVertices2;

    expected[2].vertexCount = 3;
    expected[2].vertices = expectedVertices3;

    polygon* actual;
    int actualCount;

    if (!decompose_polygon(&poly, &actual, &actualCount))
    {
        FAIL_TEST("Memory allocation failure");
    }

    char resultMsg[320];
    if (verifyPolygons(actual, actualCount, expected, 3, resultMsg))
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

IMPLEMENT_TEST(decompose_polygon_fewTowers)
{/*
        Polygon is this shape

            *     *
           / \   / \
          /   \ /   \
         /     *     \
        *             *
         \     *     /
          \   / \   /
           \ /   \ /
            *     *
    */
    vec2f vertices[8] = {to_vec2f(0.0f, 0.0f), to_vec2f(4.0f, -4.0f), to_vec2f(7.0f, -1.0f), to_vec2f(10.0f, -4.0f),
                         to_vec2f(14.0f, 0.0f), to_vec2f(10.0f, 4.0f), to_vec2f(7.0f, 1.0f), to_vec2f(4.0f, 4.0f)};

    polygon poly;
    poly.vertexCount = 8;
    poly.vertices = vertices;

    vec2f expectedVertices1[4] = {to_vec2f(0.0f, 0.0f), to_vec2f(4.0f, -4.0f), to_vec2f(7.0f, -1.0f), to_vec2f(4.0f, 4.0f)};    // 0, 1, 2, 7
    vec2f expectedVertices2[4] = {to_vec2f(7.0f, -1.0f), to_vec2f(10.0f, -4.0f), to_vec2f(7.0f, 1.0f), to_vec2f(4.0f, 4.0f)};   // 2, 3, 6, 7
    vec2f expectedVertices3[4] = {to_vec2f(10.0f, -4.0f), to_vec2f(14.0f, 0.0f), to_vec2f(10.0f, 4.0f), to_vec2f(7.0f, 1.0f)};  // 3, 4, 5, 6

    polygon expected[3];

    expected[0].vertexCount = sizeof(expectedVertices1) / sizeof(vec2f);
    expected[0].vertices    =        expectedVertices1;

    expected[1].vertexCount = sizeof(expectedVertices2) / sizeof(vec2f);
    expected[1].vertices    =        expectedVertices2;

    expected[2].vertexCount = sizeof(expectedVertices3) / sizeof(vec2f);
    expected[2].vertices    =        expectedVertices3;

    polygon* actual;
    int actualCount;

    if (!decompose_polygon(&poly, &actual, &actualCount))
    {
        FAIL_TEST("Memory allocation failure");
    }

    char resultMsg[320];
    if (verifyPolygons(actual, actualCount, expected, sizeof(expected) / sizeof(polygon), resultMsg))
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

IMPLEMENT_TEST(decompose_polygon_towers)
{
    /*
        Polygon is this shape

            *     *   *     *
           / \   / \ / \   / \
          /   \ /   *   \ /   \
         /     *         *     \
        *                       *
         \     *         *     /
          \   / \   *   / \   /
           \ /   \ / \ /   \ /
            *     *   *     *
    */
    vec2f vertices[16] = {to_vec2f(0.0f, 0.0f), to_vec2f(4.0f, -4.0f), to_vec2f(7.0f, -1.0f), to_vec2f(10.0f, -4.0f), to_vec2f(12.0f, -2.0f), to_vec2f(14.0f, -4.0f), to_vec2f(17.0f, -1.0f), to_vec2f(20.0f, -4.0f),
                        to_vec2f(24.0f, 0.0f), to_vec2f(20.0f, 4.0f), to_vec2f(17.0f, 1.0f), to_vec2f(14.0f, 4.0f), to_vec2f(12.0f, 2.0f), to_vec2f(10.0f, 4.0f), to_vec2f(7.0f, 1.0f), to_vec2f(4.0f, 4.0f)};
    polygon poly;
    poly.vertexCount = 16;
    poly.vertices = vertices;

    vec2f expectedVertices1[4] = {to_vec2f(0.0f, 0.0f), to_vec2f(4.0f, -4.0f), to_vec2f(7.0f, -1.0f), to_vec2f(4.0f, 4.0f)};                    // 0, 1, 2, 15
    vec2f expectedVertices2[4] = {to_vec2f(7.0f, -1.0f), to_vec2f(10.0f, -4.0f), to_vec2f(7.0f, 1.0f), to_vec2f(4.0f, 4.0f)};                   // 2, 3, 14, 15
    vec2f expectedVertices3[3] = {to_vec2f(10.0f, -4.0f), to_vec2f(12.0f, -2.0f), to_vec2f(7.0f, 1.0f)};                                // 3, 4, 14
    vec2f expectedVertices4[4] = {to_vec2f(12.0f, -2.0f), to_vec2f(17.0f, -1.0f), to_vec2f(12.0f, 2.0f), to_vec2f(7.0f, 1.0f)};                 // 4, 6, 12, 14
    vec2f expectedVertices5[3] = {to_vec2f(12.0f, -2.0f), to_vec2f(14.0f, -4.0f), to_vec2f(17.0f, -1.0f)};                              // 4, 5, 6
    vec2f expectedVertices6[4] = {to_vec2f(17.0f, -1.0f), to_vec2f(17.0f, 1.0f), to_vec2f(14.0f, 4.0f), to_vec2f(12.0f, 2.0f)};                 // 6, 10, 11, 12
    vec2f expectedVertices7[5] = {to_vec2f(17.0f, -1.0f), to_vec2f(20.0f, -4.0f), to_vec2f(24.0f, 0.0f), to_vec2f(20.0f, 4.0f), to_vec2f(17.0f, 1.0f)}; // 6, 7, 8, 9, 10
    vec2f expectedVertices8[3] = {to_vec2f(12.0f, 2.0f), to_vec2f(10.0f, 4.0f), to_vec2f(7.0f, 1.0f)};                                  // 12, 13, 14

    polygon expected[8];

    expected[0].vertexCount = sizeof(expectedVertices1) / sizeof(vec2f);
    expected[0].vertices    =        expectedVertices1;

    expected[1].vertexCount = sizeof(expectedVertices2) / sizeof(vec2f);
    expected[1].vertices    =        expectedVertices2;

    expected[2].vertexCount = sizeof(expectedVertices3) / sizeof(vec2f);
    expected[2].vertices    =        expectedVertices3;

    expected[3].vertexCount = sizeof(expectedVertices4) / sizeof(vec2f);
    expected[3].vertices    =        expectedVertices4;

    expected[4].vertexCount = sizeof(expectedVertices5) / sizeof(vec2f);
    expected[4].vertices    =        expectedVertices5;

    expected[5].vertexCount = sizeof(expectedVertices6) / sizeof(vec2f);
    expected[5].vertices    =        expectedVertices6;

    expected[6].vertexCount = sizeof(expectedVertices7) / sizeof(vec2f);
    expected[6].vertices    =        expectedVertices7;

    expected[7].vertexCount = sizeof(expectedVertices8) / sizeof(vec2f);
    expected[7].vertices    =        expectedVertices8;

    polygon* actual;
    int actualCount;

    if (!decompose_polygon(&poly, &actual, &actualCount))
    {
        FAIL_TEST("Memory allocation failure");
    }

    char resultMsg[320];
    if (verifyPolygons(actual, actualCount, expected, sizeof(expected) / sizeof(polygon), resultMsg))
    {
        PASS_TEST();
    }
    else
    {
        FAIL_TEST(resultMsg);
    }
}

// bool project_polygon(polygon* poly, vec2f vecSlope, vec2f* projectionResult, float* projectionDistanceSqrd)
IMPLEMENT_TEST(project_polygon)
{
    // -------------------------------------------------------------------------
    // 1: Square, angle 0 deg, above horizontal line, size of 2
    // -------------------------------------------------------------------------
    polygon poly1;
    create_polygon(&poly1, 4);
    poly1.vertices[0] = to_vec2f(1.0f, 1.0f);
    poly1.vertices[1] = to_vec2f(1.0f, 3.0f);
    poly1.vertices[2] = to_vec2f(-1.0f, 3.0f);
    poly1.vertices[3] = to_vec2f(-1.0f, 1.0f);

    vec2f vecSlope1 = to_vec2f(1.0f, 0.0f);
    vec2f actualProjectionResult1[2];
    vec2f expectedProjectionResult1[2] = { to_vec2f(-1.0, 0.0f), to_vec2f(1.0f, 0.0f) };

    float actualProjectDistanceSqrd1;
    float expectedProjectDistanceSqrd1 = 4.0f;

    project_polygon(&poly1, vecSlope1, actualProjectionResult1, &actualProjectDistanceSqrd1);

    // INDENT(UNIT_TEST_OUT);
    // fprintf(UNIT_TEST_OUT, "Projection Result. Actual:   ");
    // print_vec2f(UNIT_TEST_OUT, &actualProjectionResult1[0]);
    // fprintf(UNIT_TEST_OUT, ", ");
    // print_vec2f(UNIT_TEST_OUT, &actualProjectionResult1[1]);
    // fprintf(UNIT_TEST_OUT, "\n    Projection Result. Expected: ");
    // print_vec2f(UNIT_TEST_OUT, &expectedProjectionResult1[0]);
    // fprintf(UNIT_TEST_OUT, ", ");
    // print_vec2f(UNIT_TEST_OUT, &expectedProjectionResult1[1]);
    // fprintf(UNIT_TEST_OUT, "\n");

    // INDENT(UNIT_TEST_OUT);
    // fprintf(UNIT_TEST_OUT, "Projection distance squared. Actual %.3f, expected, %.3f\n", actualProjectDistanceSqrd1, expectedProjectDistanceSqrd1);

    if (!equal_vec2f(actualProjectionResult1[0], expectedProjectionResult1[0], DEFAULT_TOLERANCE) ||
        !equal_vec2f(actualProjectionResult1[1], expectedProjectionResult1[1], DEFAULT_TOLERANCE) ||
        !equal_f(actualProjectDistanceSqrd1, expectedProjectDistanceSqrd1, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("Failed to project a square onto a horizontal vector.");
    }

    // -------------------------------------------------------------------------
    // 2: Square, angle 0 deg, left of vertical line, size of 2
    // -------------------------------------------------------------------------
    polygon poly2;
    create_polygon(&poly2, 4);
    poly2.vertices[0] = to_vec2f(-1.0f, 1.0f);
    poly2.vertices[1] = to_vec2f(-1.0f, 3.0f);
    poly2.vertices[2] = to_vec2f(-3.0f, 3.0f);
    poly2.vertices[3] = to_vec2f(-3.0f, 1.0f);

    vec2f vecSlope2 = to_vec2f(0.0f, 1.0f);
    vec2f actualProjectionResult2[2];
    vec2f expectedProjectionResult2[2] = { to_vec2f(0.0, 1.0f), to_vec2f(0.0f, 3.0f)};

    float actualProjectDistanceSqrd2;
    float expectedProjectDistanceSqrd2 = 4.0f;

    project_polygon(&poly2, vecSlope2, actualProjectionResult2, &actualProjectDistanceSqrd2);

    // INDENT(UNIT_TEST_OUT);
    // fprintf(UNIT_TEST_OUT, "Projection Result. Actual: ");
    // print_vec2f(UNIT_TEST_OUT, &actualProjectionResult2[0]);
    // fprintf(UNIT_TEST_OUT, ", ");
    // print_vec2f(UNIT_TEST_OUT, &actualProjectionResult2[1]);
    // fprintf(UNIT_TEST_OUT, ", expected: ");
    // print_vec2f(UNIT_TEST_OUT, &expectedProjectionResult2[0]);
    // fprintf(UNIT_TEST_OUT, ", ");
    // print_vec2f(UNIT_TEST_OUT, &expectedProjectionResult2[1]);
    // fprintf(UNIT_TEST_OUT, "\n");

    // INDENT(UNIT_TEST_OUT);
    // fprintf(UNIT_TEST_OUT, "Projection distance squared. Actual %.3f, expected, %.3f\n", actualProjectDistanceSqrd2, expectedProjectDistanceSqrd2);

    if (!equal_vec2f(actualProjectionResult2[0], expectedProjectionResult2[0], DEFAULT_TOLERANCE) ||
        !equal_vec2f(actualProjectionResult2[1], expectedProjectionResult2[1], DEFAULT_TOLERANCE) ||
        !equal_f(actualProjectDistanceSqrd2, expectedProjectDistanceSqrd2, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("Failed to project a square onto a vertical vector.");
    }

    // -------------------------------------------------------------------------
    // 3: Square, angle 45 deg, above of 45 deg line, size of 2
    // -------------------------------------------------------------------------
    polygon poly3;
    create_polygon(&poly3, 4);
    poly3.vertices[0] = to_vec2f(-0.7071f, 0.7071f);
    poly3.vertices[1] = to_vec2f(0.0f, 1.4142f);
    poly3.vertices[2] = to_vec2f(-0.7071f, 2.1213f);
    poly3.vertices[3] = to_vec2f(-1.4142f, 1.4142f);

    vec2f vecSlope3 = to_vec2f(0.7071f, 0.7071f);
    vec2f actualProjectionResult3[2];
    vec2f expectedProjectionResult3[2] = { to_vec2f(0.0f, 0.0f), to_vec2f(0.707, 0.707f) };

    float actualProjectDistanceSqrd3;
    float expectedProjectDistanceSqrd3 = 1.0f;

    project_polygon(&poly3, vecSlope3, actualProjectionResult3, &actualProjectDistanceSqrd3);

    // INDENT(UNIT_TEST_OUT);
    // fprintf(UNIT_TEST_OUT, "Projection Result. Actual: ");
    // print_vec2f(UNIT_TEST_OUT, &actualProjectionResult3[0]);
    // fprintf(UNIT_TEST_OUT, ", ");
    // print_vec2f(UNIT_TEST_OUT, &actualProjectionResult3[1]);
    // fprintf(UNIT_TEST_OUT, ", expected: ");
    // print_vec2f(UNIT_TEST_OUT, &expectedProjectionResult3[0]);
    // fprintf(UNIT_TEST_OUT, ", ");
    // print_vec2f(UNIT_TEST_OUT, &expectedProjectionResult3[1]);
    // fprintf(UNIT_TEST_OUT, "\n");

    // INDENT(UNIT_TEST_OUT);
    // fprintf(UNIT_TEST_OUT, "Projection distance squared. Actual %.3f, expected, %.3f\n", actualProjectDistanceSqrd3, expectedProjectDistanceSqrd3);

    if (!equal_vec2f(actualProjectionResult3[0], expectedProjectionResult3[0], DEFAULT_TOLERANCE) ||
        !equal_vec2f(actualProjectionResult3[1], expectedProjectionResult3[1], DEFAULT_TOLERANCE) ||
        !equal_f(actualProjectDistanceSqrd3, expectedProjectDistanceSqrd3, DEFAULT_TOLERANCE))
    {
        FAIL_TEST("Failed to project a rotated square onto a diagonal vector.");
    }

    PASS_TEST();
}

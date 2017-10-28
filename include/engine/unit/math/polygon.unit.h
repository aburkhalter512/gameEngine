#pragma once

#include "engine/math/polygon.h"

#include "util/unit.h"

typedef struct _referenceVertex referenceVertex;
typedef struct _vertexDiagonal vertexDiagonal;

struct _referenceVertex
{
    int prev;
    int next;
    bool isEar;
};

struct _vertexDiagonal
{
    int index;
    vertexDiagonal* opposite;

    vertexDiagonal* prev;
    vertexDiagonal* next;
};

// Useful verification functions
bool verifyPolygon(polygon* actual, polygon* expected, char resultMsg[320], int initialOffset);
bool verifyPolygons(polygon* actual, int actualCount, polygon* expected, int expectedCount, char resultMsg[320]);

// Don't access these functions directly, as they often have unwritten preconditions
bool _isDiagonal_polygon(polygon* poly, referenceVertex* earVertices, int baseVertexIndex, int diagonalVertexIndex);
referenceVertex* _create_referenceVertices(polygon* poly);
vertexDiagonal* _init_vertexDiagonal(vertexDiagonal* diagonal, vertexDiagonal* prev, vertexDiagonal* next, int endpoint);
void _addDiagonal_vertexDiagonal(polygon* poly, vertexDiagonal** currentDiagonal, vertexDiagonal** center, int prevVertex, int nextVertex);
vertexDiagonal** _triangulate_polygon(polygon* poly, vertexDiagonal** diagonalPool);
bool _isEssential_vertexDiagonal(polygon* poly, vertexDiagonal* base);
bool _isInessential_vertexDiagonal(polygon* poly, vertexDiagonal* base);
void _removeDiagonal_vertexDiagonal(vertexDiagonal** diagonals, vertexDiagonal* base);
int _removeInessentialDiagonals_vertexDiagonals(polygon* poly, vertexDiagonal** diagonals);
polygon* _generateConvexPolygons_vertexDiagonals(polygon* poly, vertexDiagonal** diagonals, int essentialDiagonalCount);

// Public API
PROTOTYPE_TEST(create_polygon);
PROTOTYPE_TEST(free_polygon);
PROTOTYPE_TEST(translate_polygon);
PROTOTYPE_TEST(decompose_polygon_quad);
PROTOTYPE_TEST(decompose_polygon_quadReflex);
PROTOTYPE_TEST(decompose_polygon_complexReflex);
PROTOTYPE_TEST(decompose_polygon_fewTowers);
PROTOTYPE_TEST(decompose_polygon_towers);
PROTOTYPE_TEST(project_polygon);

// Private API
PROTOTYPE_TEST(_isDiagonal_polygon);
PROTOTYPE_TEST(_create_referenceVertices);
PROTOTYPE_TEST(_init_vertexDiagonal);
PROTOTYPE_TEST(_triangulate_polygon_quad);
PROTOTYPE_TEST(_triangulate_polygon_quadReflex);
PROTOTYPE_TEST(_triangulate_polygon_pentagon);
PROTOTYPE_TEST(_triangulate_polygon_complexReflex);
PROTOTYPE_TEST(_isEssential_vertexDiagonal);
PROTOTYPE_TEST(_isInessential_vertexDiagonal);


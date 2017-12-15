#ifndef UNIT_TEST
#include "engine/math/polygon.h"
#else
#include "engine/unit/math/polygon.unit.h"
#endif

#include <stdlib.h>

#include "engine/util.h"
#include "engine/math/vec.h"

#ifndef UNIT_TEST
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
#endif

// see header for documentation
bool create_polygon(polygon* poly, int vertexCount)
{
    if (!poly || vertexCount < 3)
    {
        return false;
    }

    poly->vertexCount = vertexCount;
    poly->vertices = calloc(vertexCount, sizeof(vec2f));
    if (!poly->vertices)
    {
        return false;
    }

    return true;
}

// see header for documentation
bool free_polygon(polygon* poly)
{
    if (!poly || !poly->vertices)
    {
        return false;
    }

    free(poly->vertices);
    poly->vertices = NULL;
    poly->vertexCount = 0;

    return true;
}

// see header for documentation
bool translate_polygon(polygon* in, polygon* out, vec2f translation)
{
    if (!in || !out || in->vertexCount != out->vertexCount)
    {
        return false;
    }

    for (int i = 0; i < in->vertexCount; i++)
    {
        out->vertices[i] = add_vec2f(in->vertices[i], translation);
    }

    return true;
}

/*
Determines if a suspected diagonal is valid. A diagonal is valid if it is:
* Internal
* Does not intersect any other polygon segments

Runs in O(n) time.

Arguments
    polygon* poly: The polygon to utilize when determining if a diagonal is valid

    referenceVertex* earVertices: A array of reference vertices with the same length as poly->vertices

    int baseVertexIndex: The index of the vertex that is the base of the diagonal.
        This value with diagonalVertexIndex form a set line segment

    int diagonalVertexIndex: The index of the vertex that is the end of the diagonal.
        This value with baseVertexIndex form a set line segment

Preconditions
    baseVertexIndex and diagonalVertexIndex are not directly adjacent to each other.

    poly and earVertices are fully initialized. poly->vertices should be an array with the same length
    as earVertices

Returns
    Returns true if the suspected diagonal is valid, false if not.
*/
bool _isDiagonal_polygon(polygon* poly, referenceVertex* earVertices, int baseVertexIndex, int diagonalVertexIndex)
{
    vec2f baseVertex = poly->vertices[baseVertexIndex];
    vec2f diagonalVertex = poly->vertices[diagonalVertexIndex];

    // Verify the suspect diagonal does not intersect with any of the polygon's own segments
    for (int firstVertex = earVertices[baseVertexIndex].next; earVertices[firstVertex].next != baseVertexIndex; firstVertex = earVertices[firstVertex].next)
    {
        int nextVertex = earVertices[firstVertex].next;

        // Segments connected to other segments can never intersect (at least on the strict definition of intersecting)
        if (firstVertex == diagonalVertexIndex || nextVertex == diagonalVertexIndex)
        {
            continue;
        }

        vec2f diagonal[] = { poly->vertices[firstVertex], poly->vertices[nextVertex] };

        // If any one polygon segment and the diagonal intersect, then this diagonal is not valid
        if (areSegmentsIntersecting_vec2f(
            baseVertex, diagonalVertex, // segment 1
            diagonal[0], diagonal[1])) // segment 2
        {
            return false;
        }
    }

    vec2f prevBaseVertex = poly->vertices[earVertices[baseVertexIndex].prev];
    vec2f nextBaseVertex = poly->vertices[earVertices[baseVertexIndex].next];

    // Make sure the diagonal isn't external
    // if the vertex is convex
    if (linearity_vec2f(prevBaseVertex, baseVertex, nextBaseVertex) == LINEARITY_COUNTER_CLOCKWISE)
    {
        // Verify the diagonal is located inside the INTERNAL convex vertex
        // the idea here is to make sure the order of points appears in a counter clockwise fashion.
        return linearity_vec2f(prevBaseVertex, baseVertex, diagonalVertex) == LINEARITY_COUNTER_CLOCKWISE &&
               linearity_vec2f(baseVertex, nextBaseVertex, diagonalVertex) == LINEARITY_COUNTER_CLOCKWISE;
    }
    // if the vertex is reflex
    else
    {
        // Verify the diagonal is NOT located inside the EXTERNAL convex vertex
        // the idea here is to make sure the order of points do not appear in a counter clockwise fashion.
        return !(linearity_vec2f(baseVertex, diagonalVertex, nextBaseVertex) == LINEARITY_COUNTER_CLOCKWISE &&
                 linearity_vec2f(nextBaseVertex, baseVertex, prevBaseVertex) == LINEARITY_COUNTER_CLOCKWISE);
    }
}

/*
Allocates and initializes reference vertices with respect to a polygon. Additionally
each reference vertex is processed to determine if it is an ear or not.

A vertex is an ear if the prev and next polygon vertices form a valid diagonal.

Runs in O(n) time.

Arguments
    polygon* poly: The polygon to create the reference vertices from

Returns
    Returns an array of reference with the same length as poly->vertices. Each index of the returned array
    directly corresponds with the index in poly->vertices. Additionally each referenceVertex prev and next
    refer to the immediately adjacent vertex index (index +- 1).

    Returns NULL if memory allocation failed.
*/
referenceVertex* _create_referenceVertices(polygon* poly)
{
    referenceVertex* earVertices = calloc(poly->vertexCount, sizeof(referenceVertex));
    if (!earVertices)
    {
        return NULL;
    }

    int lastVertexIndex = poly->vertexCount - 1;

    // The first and last indices wrap around the end of the array, so calculate them outside the loop
    earVertices[0].prev = lastVertexIndex;
    earVertices[0].next = 1;

    earVertices[lastVertexIndex].prev = lastVertexIndex - 1;
    earVertices[lastVertexIndex].next = 0;

    // Link the array together
    for (int i = 1; i < lastVertexIndex; i++)
    {
        earVertices[i].prev = i - 1;
        earVertices[i].next = i + 1;
    }

    // Calculate which vertices are ears
    for (int i = 0; i < poly->vertexCount; i++)
    {
        earVertices[i].isEar = _isDiagonal_polygon(poly, earVertices, earVertices[i].prev, earVertices[i].next);
    }

    return earVertices;
}

/*
Initializes a single vertexDiagonal

Runs in O(1) time

Arguments
    vertexDiagonal* diagonal: The vertex diagonal to initialize

    vertexDiagonal* prev: The vertexDiagonal to set as the previous vertex diagonal. This
        value is exists to order vertexDiagonals in a meaningful way.

    vertexDiagonal* next:The vertexDiagonal to set as the next vertex diagonal. This
        value is exists to order vertexDiagonals in a meaningful way.

    int endpoint: The index of the endpoint of the diagonal.

Returns
    Returns the initialized diagonal, which is also the variable "diagonal"
*/
vertexDiagonal* _init_vertexDiagonal(vertexDiagonal* diagonal, vertexDiagonal* prev, vertexDiagonal* next, int index)
{
    diagonal->prev = prev;
    diagonal->next = next;
    diagonal->index = index;

    if (prev)
    {
        prev->next = diagonal;
    }

    if (next)
    {
        next->prev = diagonal;
    }

    return diagonal;
}

/*
Adds a single side of a vertexDiagonal set, explicitly NOT setting the `opposite` field

Runs in O(1) time.

Arguments
    polygon* poly: The polygon to add the diagonal to

    vertexDiagonal** currentDiagonal: A pool of pre-allocated vertexDiagonals

    vertexDiagonal** diagonals: An array of vertexDiagonal*'s where diagonal at the given
        index is the "center"

    int index: The index of the vertexDiagonal, or where the base of the diagonal is located

    int oppositeIndex: The index of the `opposite` `vertexDiagonal`

    enum LINEARITY diagonalLinearity: Indicates on which side of the center should this new diagonal be placed.
        LINEARITY_CLOCKWISE places it on the previous side and LINEARITY_COUNTER_CLOCKWISE places it on
        the next side. LINEARITY_COLLINEAR is an invalid value.

Returns
    `diagonals` is the output variable. A new center will be set for `diagonals` at `index`
*/
void _addSingleDiagonal_vertexDiagonal(polygon* poly, vertexDiagonal** currentDiagonal, vertexDiagonal** diagonals, int index, int oppositeIndex)
{
    // If the center diagonal does not exist yet, skip the LINEARITY processing step
    // and just set the diagonal
    if (!diagonals[index])
    {
        diagonals[index] = _init_vertexDiagonal(*currentDiagonal, NULL, NULL, index);
    }
    else
    {
        enum LINEARITY diagonalLinearity;

        vec2f oldOppositeVertex = poly->vertices[diagonals[index]->opposite->index];
        vec2f newOppositeVertex = poly->vertices[oppositeIndex];
        vec2f baseVertex = poly->vertices[index];
        vec2f prevVertex = poly->vertices[index == 0 ? poly->vertexCount - 1 : index - 1];
        vec2f nextVertex = poly->vertices[(index + 1) % poly->vertexCount];

        // The idea here is to find a convex angle between the oldOppositeVertex, the baseVertex, and one of the sides
        if (linearity_vec2f(prevVertex, baseVertex, oldOppositeVertex) == LINEARITY_COUNTER_CLOCKWISE)
        {
            // Does the new diagonal fall between the side and the diagonal?
            if (linearity_vec2f(prevVertex, baseVertex, newOppositeVertex) == LINEARITY_COUNTER_CLOCKWISE &&
               linearity_vec2f(newOppositeVertex, baseVertex, oldOppositeVertex) == LINEARITY_COUNTER_CLOCKWISE)
            {
                diagonalLinearity = LINEARITY_CLOCKWISE;
            }
            else
            {
                diagonalLinearity = LINEARITY_COUNTER_CLOCKWISE;
            }
        }
        else
        {
            // Does the new diagonal fall between the side and the diagonal?
            if (linearity_vec2f(oldOppositeVertex, baseVertex, newOppositeVertex) == LINEARITY_COUNTER_CLOCKWISE &&
               linearity_vec2f(newOppositeVertex, baseVertex, nextVertex) == LINEARITY_COUNTER_CLOCKWISE)
            {
                diagonalLinearity = LINEARITY_COUNTER_CLOCKWISE;
            }
            else
            {
                diagonalLinearity = LINEARITY_CLOCKWISE;
            }
        }

        switch (diagonalLinearity)
        {
            // If the diagonal is to the prev of the current center endpoint
            case LINEARITY_CLOCKWISE:
                diagonals[index] = _init_vertexDiagonal(*currentDiagonal, diagonals[index]->prev, diagonals[index], index);
                break;
                // If the diagonal is to the next of the current center endpoint
            case LINEARITY_COUNTER_CLOCKWISE:
                diagonals[index] = _init_vertexDiagonal(*currentDiagonal, diagonals[index], diagonals[index]->next, index);
                break;
            default:
                // We should never get here. _isDiagonal_polygon() should never generate a diagonal that touches another vertex
                break;
        }
    }

    // Increment the diagonal pool
    (*currentDiagonal)++;
}

/*
Adds a connected set of vertexDiagonal's to the specified indices

Runs in O(1) time.

Arguments
    polygon* poly: The polygon that a diagonal is being added to

    vertexDiagonal** currentDiagonal: A pool of pre-allocated vertexDiagonals

    vertexDiagonal** diagonals: An array of vertexDiagonal*'s where diagonal at the given
        index is the "center"

    int diagonalBasepoint: The base vertex index of the diagonal to add

    int diagonalEndpoint: The end vertex index of the diagonal to add

Postconditions
    diagonals[diagonalBasepoint] and diagonals[diagonalEndpoint] will never be NULL for those specific indices
    and their `opposite` fields will be set to the other vertexDiagonal.

Returns
    currentDiagonal and diagonals are the outputs
        currentDiagonal will be incremented
        diagonals[diagonalBasepoint] and diagonals[diagonalEndpoint] will have new center diagonals
*/
void _addDiagonal_vertexDiagonal(polygon* poly, vertexDiagonal** currentDiagonal, vertexDiagonal** diagonals, int diagonalBasepoint, int diagonalEndpoint)
{
    _addSingleDiagonal_vertexDiagonal(poly, currentDiagonal, diagonals, diagonalBasepoint, diagonalEndpoint);
    _addSingleDiagonal_vertexDiagonal(poly, currentDiagonal, diagonals, diagonalEndpoint, diagonalBasepoint);

    diagonals[diagonalBasepoint]->opposite = diagonals[diagonalEndpoint];
    diagonals[diagonalEndpoint]->opposite = diagonals[diagonalBasepoint];
}

/*
Triagulates a polygon by removing vertex ears in an iterative fashion.

Runs in O(n^2) time.

Arguments
    polygon* poly: The polygon to triagulate

    vertexDiagonal** diagonalPool: An out value that contains the allocated memory
        for all diagonals. Once you are done with the diagonals, deallocate this memory

Preconditions
    `poly` is a polygon with > 3 vertices

Returns
    Returns an array of vertex diagonals whose length is equal to poly->vertexCount.
    Each index of the returned array corresponds to an index in poly->vertices. Each
    vertexDiagonal directly accessible by the index is the "center" of that vertex's diagonals.
    The returned array is allocated memory, so free it once you are finished with it.

    Returns NULL if memory allocation failed.
*/
vertexDiagonal** _triangulate_polygon(polygon* poly, vertexDiagonal** diagonalPool)
{
    // Initialize the first earVertices. By doing this initialization, we transform this function
    // from O(n^3) to O(n^2)
    referenceVertex* earVertices = _create_referenceVertices(poly);
    if (!earVertices)
    {
        return NULL;
    }

    // We always know how many diagonals are needed to fully triangulate a polygon. That number
    // is vertex count - 3. A triangle can have no diagonals, a quad 1, pentagon 2, etc...
    int diagonalCount = poly->vertexCount - 3;

    // Initialize all of our memory upfront
    vertexDiagonal** diagonals = calloc(sizeof(vertexDiagonal*), poly->vertexCount);
    if (!diagonals)
    {
        free(earVertices);
        return NULL;
    }
    *diagonalPool = calloc(sizeof(vertexDiagonal), diagonalCount * 2);
    if (!*diagonalPool)
    {
        free(earVertices);
        free(diagonals);
        return NULL;
    }

    // The iterator for the bulk memory we assigned.
    vertexDiagonal* currentDiagonal = *diagonalPool;

    // Loop until we have 3 remaining vertices left
    int remainingVertices = poly->vertexCount;

    // Un-intuitively, this outer loop is O(n) rather than O(n^2) because of the immediate isEar check
    // The loop itself will iterate over 0(n^2) ear vertices, but only O(n) of them will incur the
    // O(n) is diagonal check.
    for (int currentVertex = 0; remainingVertices > 3; currentVertex = earVertices[currentVertex].next)
    {
        // We only remove ear vertices. This check also reduces the complexity from O(n^3) to O(n^2)
        if (!earVertices[currentVertex].isEar)
        {
            continue;
        }

        int prevVertex = earVertices[currentVertex].prev;
        int nextVertex = earVertices[currentVertex].next;

        _addDiagonal_vertexDiagonal(poly, &currentDiagonal, diagonals, prevVertex, nextVertex);

        // Remove the current vertex ear by linking around it
        earVertices[prevVertex].next = nextVertex;
        earVertices[nextVertex].prev = prevVertex;

        // Update the only vertices that could have changed ear status (the immediately adjacent vertices)
        earVertices[prevVertex].isEar = _isDiagonal_polygon(poly, earVertices, earVertices[prevVertex].prev, earVertices[prevVertex].next);
        earVertices[nextVertex].isEar = _isDiagonal_polygon(poly, earVertices, earVertices[nextVertex].prev, earVertices[nextVertex].next);

        remainingVertices--;
    }

    free(earVertices);

    return diagonals;
}

/*
Determines if a given diagonal is essential to decomposing `poly` into convex polygons.
A diagonal is essential if removed, it would create a reflex vertex.

Runs in O(1) time.

Arguments
    polygon* poly: The polygon being processed

    vertexDiagonal* base: The diagonal being tested

Preconditions
    `base` and `base->opposite` are not NULL

Returns
    Returns true if the previous, base, and next vertices have appear in clockwise order
*/
bool _isEssential_vertexDiagonal(polygon* poly, vertexDiagonal* base)
{
    // Get either the prev's vertexDiagonal endpoint or the prev polygon vertex
    vec2f prevVertex;
    if (base->prev)
    {
        prevVertex = poly->vertices[base->prev->opposite->index];
    }
    else
    {
        if (base->index == 0)
        {
            prevVertex = poly->vertices[poly->vertexCount - 1];
        }
        else
        {
            prevVertex = poly->vertices[base->index - 1];
        }
    }

    // Get either the next's vertexDiagonal endpoint or the next polygon vertex
    vec2f nextVertex;
    if (base->next)
    {
        nextVertex = poly->vertices[base->next->opposite->index];
    }
    else
    {
        if (base->index == poly->vertexCount - 1)
        {
            nextVertex = poly->vertices[0];
        }
        else
        {
            nextVertex = poly->vertices[base->index + 1];
        }
    }

    vec2f baseVertex = poly->vertices[base->index];

    // Would removing this diagonal create a reflex vertex?
    if (linearity_vec2f(prevVertex, baseVertex, nextVertex) == LINEARITY_CLOCKWISE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
Determines if both sides of a diagonal are inessential and can be removed without
affecting the decomposition of a polygon into convex polygon(s)

Runs in O(1) time.

Arguments
    polygon* poly: The polygon being processed

    vertexDiagonal* base: The diagonal being tested

Preconditions
    `base` is not NULL

Returns
    Returns true both sides of the diagonal are inessential and removing them
    would not create a reflex vertex
*/
bool _isInessential_vertexDiagonal(polygon* poly, vertexDiagonal* base)
{
    // If removal of the diagonal would make either side a reflex vertex, then it is essential
    if (_isEssential_vertexDiagonal(poly, base) ||
        _isEssential_vertexDiagonal(poly, base->opposite))
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
Removes a diagonal by unlinking it from surrounding diagonals.

Runs in O(1) time.

Arguments
    vertexDiagonal** diagonals: An array of vertexDiagonal*'s where diagonal at the given
        index is the "center"

    vertexDiagonal* toRemove: The vertexDiagonal* to remove
*/
void _removeDiagonal_vertexDiagonal(vertexDiagonal** diagonals, vertexDiagonal* toRemove)
{
    // Unlink toRemove from the prev and next vertexDiagonal's
    if (toRemove->prev)
    {
        toRemove->prev->next = toRemove->next;
    }

    if (toRemove->next)
    {
        toRemove->next->prev = toRemove->prev;
    }

    if (toRemove == diagonals[toRemove->index])
    {
        if (toRemove->prev)
        {
            diagonals[toRemove->index] = toRemove->prev;
        }
        else if (toRemove->next)
        {
            diagonals[toRemove->index] = toRemove->next;
        }
        else
        {
            diagonals[toRemove->index] = NULL;
        }
    }
}

/*
Removes all inessential diagonals from the `diagonals` array. An diagonal is removed
if and only if removing it would not create a reflex vertex.

This function implements the Herman-Mehlhorn algorithm

Runs in O(n) time.

Arguments
    polygon* poly: The polygon being decomposed into convex polygons

    vertexDiagonal** diagonals: An array of vertexDiagonal*'s where diagonal at the given
        index is the "center"

Returns
    Returns the number of essential diagonals left after processing
*/
int _removeInessentialDiagonals_vertexDiagonals(polygon* poly, vertexDiagonal** diagonals)
{
    int essentialDiagonalCount = 0;

    // Even though there are 2 loops, this is still O(n) since there are O(n) diagonals
    for (int baseIndex = 0; baseIndex < poly->vertexCount; baseIndex++)
    {
        if (!diagonals[baseIndex])
        {
            continue;
        }

        // Remove prev inessential diagonals
        vertexDiagonal* it = diagonals[baseIndex]->prev;
        vertexDiagonal* toRemove;
        while (it)
        {
            toRemove = it;
            it = it->prev;

            if (_isInessential_vertexDiagonal(poly, toRemove))
            {

                // Remove both sides of the diagonal
                _removeDiagonal_vertexDiagonal(diagonals, toRemove);
                _removeDiagonal_vertexDiagonal(diagonals, toRemove->opposite);
            }
            else
            {
                essentialDiagonalCount++;
            }
        }

        // Remove next inessential diagonals
        it = diagonals[baseIndex]->next;
        while (it)
        {
            toRemove = it;
            it = it->next;

            if (_isInessential_vertexDiagonal(poly, toRemove))
            {

                // Remove both sides of the diagonal
                _removeDiagonal_vertexDiagonal(diagonals, toRemove);
                _removeDiagonal_vertexDiagonal(diagonals, toRemove->opposite);
            }
            else
            {
                essentialDiagonalCount++;
            }
        }

        // Remove and reassign the center;
        toRemove = diagonals[baseIndex];
        if (_isInessential_vertexDiagonal(poly, toRemove))
        {

            // Remove both sides of the diagonal
            _removeDiagonal_vertexDiagonal(diagonals, toRemove);
            _removeDiagonal_vertexDiagonal(diagonals, toRemove->opposite);
        }
        else
        {
            essentialDiagonalCount++;
        }
    }

    return essentialDiagonalCount / 2; // We count each end of the diagonal once, so half to return the total number of essential diagonals
}

/*
Generates a polygon for a given starting diagonal and stores in in polyPool.

Any additional diagonals encountered while processing will kickoff new polygon generation

Arguments
    polygon* inPoly: The polygon to generate a convex polygon from

    vertexDiagonals** diagonals: An array of essential diagonals that are used
        to generate the convex polygons

    vertexDiagonal* start: The diagonal to start from when generating a new convex polygon.
        Passing NULL here will start at index 0, which may or may not have a starting diagonal.

    int* vertexAccumulator: Memory given to this function to temporarily store vertex indices.
        This total length of this memory should equal the number of polygon vertices + the number of essential diagonals * 2

    polygon*** currentPoly: A pointer to a pool of polygon pointers

Preconditions
    When calling this function with `start` == NULL, currentPoly should be a reference to allocated memory with size of
    `essentialDiagonalCount + 1`

Returns
    Returns false only if memory allocation fails
*/
bool _generateConvexPolygon_vertexDiagonals(polygon* inPoly, vertexDiagonal** diagonals, vertexDiagonal* start, int endIndex, int* vertexAccumulator, polygon** currentPoly)
{
    // printf("\n    _generateConvexPolygon_vertexDiagonals(), endIndex: %d\n", endIndex);

    // This will contain the newly created convex polygon for this initial diagonal
    polygon* outPoly = *currentPoly;
    (*currentPoly)++;

    int accumulatorIndex = 0;

    // We need both the currentIndex and currentDiagonal because not every vertex has a diagonal
    int currentIndex = start->index;
    vertexDiagonal* currentDiagonal = NULL;

    if (start->opposite) // Is this a real vertexDiagonal?
    {
        currentDiagonal = start;
    }
    else if (currentIndex == 0)
    {
        currentDiagonal = diagonals[currentIndex];
        while (currentDiagonal && currentDiagonal->prev)
        {
            currentDiagonal = currentDiagonal->prev;
        }
        endIndex = inPoly->vertexCount - 1;
    }

    // Follow the polygon vertices and diagonals counter clockwise
    while (currentIndex != endIndex)
    {
        // printf("        (%p) currentIndex: %d\n", outPoly, currentIndex);
        // if (currentDiagonal)
        // {
        //     printf("        (%p) currentDiagonal: opposite: %d\n", outPoly, currentDiagonal->opposite->index);
        // }
        // else
        // {
        //     printf("        (%p) currentDiagonal: NULL\n", outPoly);
        // }
        // printf("        (%p) accumulatorIndex: %d\n\n", outPoly, accumulatorIndex);

        // Save the index of our vertice we want to assign to the generated polygon later
        vertexAccumulator[accumulatorIndex++] = currentIndex;

        // Are we tranversing a diagonal?
        if (currentDiagonal)
        {
            // Do we generate a new polygon following the next vertexDiagonal?
            if (currentDiagonal->next)
            {
                vertexDiagonal* start = currentDiagonal->next;
                int endIndex = currentDiagonal->opposite->index;
                if (!_generateConvexPolygon_vertexDiagonals(inPoly, diagonals, start, endIndex, &vertexAccumulator[accumulatorIndex + 1], currentPoly))
                {
                    return false;
                }
            }
            // Or do we generate a new polygon following the polygon vertices?
            else
            {
                vertexDiagonal start = {currentDiagonal->index, NULL, NULL, NULL};
                int endIndex = currentDiagonal->opposite->index;
                if (!_generateConvexPolygon_vertexDiagonals(inPoly, diagonals, &start, endIndex, &vertexAccumulator[accumulatorIndex + 1], currentPoly))
                {
                    return false;
                }
            }

            // Follow the diagonal and save the opposite index here, otherwise we will jump back and forth
            currentDiagonal = currentDiagonal->opposite;

            // Follow the polygon path counter clockwise
            currentIndex = currentDiagonal->index;
            currentDiagonal = currentDiagonal->next;
        }
        // Nope, we are rotating ccw through the polygon vertices
        else
        {
            currentIndex = (currentIndex + 1) % inPoly->vertexCount;

            // If there are diagonals at the current index, find the left most one
            currentDiagonal = diagonals[currentIndex];
            while (currentDiagonal && currentDiagonal->prev)
            {
                currentDiagonal = currentDiagonal->prev;
            }
        }
    }

    // Save the last vertex to complete the polygon
    vertexAccumulator[accumulatorIndex++] = endIndex;

    // Allocate the newly created convex polygon
    if (!create_polygon(outPoly, accumulatorIndex))
    {
        return false;
    }

    // Assign the vertices
    for (int i = 0; i < accumulatorIndex; i++)
    {
        outPoly->vertices[i] = inPoly->vertices[vertexAccumulator[i]];
    }

    // printf("\n    (%p) done\n", outPoly);

    return true;
}

/*
Generates convex polygons from a source polygon and eseential diagonals.

Arguments
    polygon* poly: The source polygon to generate convex polygon's from

    vertexDiagonal** diagonals: An array of diagonals essential creating convex polygons

    int essentialDiagonalCount: A number >= 0 that represents the number of essential diagonals
        in `diagonals`. This value is used only to optimize the amount of memory allocated.

Returns
    An array of convex polygons with length of essentialDiagonal + 1. The array returned
    is dynamically allocated memory and must be freed.
*/
polygon* _generateConvexPolygons_vertexDiagonals(polygon* poly, vertexDiagonal** diagonals, int essentialDiagonalCount)
{
    int* vertexAccumulator = calloc(poly->vertexCount + essentialDiagonalCount * 2, sizeof(int));
    if (!vertexAccumulator)
    {
        return NULL;
    }

    int convexPolygonCount = essentialDiagonalCount + 1;
    polygon* convexPolygons = calloc(convexPolygonCount, sizeof(polygon));
    if (!convexPolygons)
    {
        free(vertexAccumulator);
        return NULL;
    }

    polygon* currentDiagonal = convexPolygons;
    vertexDiagonal start = {0, NULL, NULL, NULL};
    if (!_generateConvexPolygon_vertexDiagonals(poly, diagonals, &start, poly->vertexCount - 1, vertexAccumulator, &currentDiagonal))
    {
        for (int i = 0; i < convexPolygonCount; i++)
        {
            free_polygon(&convexPolygons[i]);
        }

        free(convexPolygons);
        convexPolygons = NULL;
    }

    free(vertexAccumulator);

    return convexPolygons;
}

void print_vertexDiagonals(vertexDiagonal* v)
{
    if (!v)
    {
        return;
    }
    printf("vertexDiagonal[%d]: ", v->index);

    while (v->prev)
    {
        v = v->prev;
    }

    while (v)
    {
        printf(" %d,", v->opposite->index);
        v = v->next;
    }

    printf("\n");
}

// see header for documentation
bool decompose_polygon(polygon* in, polygon** out, int* outCount)
{
    if (!in || !in->vertices || !out || !outCount)
    {
        return false;
    }

    *out = NULL;

    int vertexCount = in->vertexCount;
    polygon counterClockWisePoly;
    create_polygon(&counterClockWisePoly, vertexCount);

    // All of the decomposition functions assume a ccw polygon, so reverse the order of points as necessary
    if (isClockwise_polygon(in))
    {
        for (int i = 0; i < vertexCount; i++)
        {
            counterClockWisePoly.vertices[i] = in->vertices[vertexCount - 1 - i];
        }
    }
    else
    {
        for (int i = 0; i < vertexCount; i++)
        {
            counterClockWisePoly.vertices[i] = in->vertices[i];
        }
    }

    vertexDiagonal* diagonalPool;
    vertexDiagonal** diagonals = _triangulate_polygon(&counterClockWisePoly, &diagonalPool);
    if (!diagonals)
    {
        return false;
    }

    int essentialDiagonalCount = _removeInessentialDiagonals_vertexDiagonals(&counterClockWisePoly, diagonals);
    *outCount = essentialDiagonalCount + 1;

    // for (int i = 0; i < in->vertexCount; i++)
    // {
    //     print_vertexDiagonals(diagonals[i]);
    // }

    *out = _generateConvexPolygons_vertexDiagonals(&counterClockWisePoly, diagonals, essentialDiagonalCount);

    free_polygon(&counterClockWisePoly);
    free(diagonalPool);
    free(diagonals);

    return true;
}

bool project_polygon(polygon* poly, vec2f slope, vec2f* projectionResult, float* projectionDistanceSqrd)
{
    if (!poly || !projectionResult || !projectionDistanceSqrd)
    {
        return false;
    }

    vec2f projectedPoint;
    float endpointDistancesSqrd[2];

    projectionResult[0] = project_vec2f(slope, poly->vertices[0]);
    projectionResult[1] = projectionResult[0];

    *projectionDistanceSqrd = 0.0f;
    for (int vertexIndex = 1; vertexIndex < poly->vertexCount; vertexIndex++)
    {
        projectedPoint = project_vec2f(slope, poly->vertices[vertexIndex]);

        // These values are the distances between the project point and the endpoints of the current projection
        // By knowing the project end point distances, we can determine if the projected point
        // is inside or outside the current projection.
        endpointDistancesSqrd[0] = distanceSqrd_vec2f(projectionResult[0], projectedPoint);
        endpointDistancesSqrd[1] = distanceSqrd_vec2f(projectionResult[1], projectedPoint);

        // Detemine what is the largest end point distance is, so if an end point distance is larger than
        // the current project distance, we can correctly set the new largest projection distance
        if (endpointDistancesSqrd[0] > endpointDistancesSqrd[1])
        {
            if (endpointDistancesSqrd[0] > *projectionDistanceSqrd)
            {
                projectionResult[1] = projectedPoint;

                *projectionDistanceSqrd = endpointDistancesSqrd[0];
            }
        }
        else
        {
            if (endpointDistancesSqrd[1] > *projectionDistanceSqrd)
            {
                projectionResult[0] = projectedPoint;

                *projectionDistanceSqrd = endpointDistancesSqrd[1];
            }
        }
    }

    // Orient the projection result so that [0] is closest to the origin.
    // This ensures that comparing to different projection results for
    // the same vecSlope can use the same indices
    if (distanceSqrd_vec2f(projectionResult[0], to_vec2f(0.0f, 0.0f)) >
        distanceSqrd_vec2f(projectionResult[1], to_vec2f(0.0f, 0.0f)))
    {
        SWAP(projectionResult[0], projectionResult[1]);
    }

    return true;
}

bool isClockwise_polygon(polygon* poly)
{
    float area = 0.0f;

    vec2f baseVertex = poly->vertices[poly->vertexCount - 1];
    vec2f nextVertex = poly->vertices[0];
    area += (GET_X(nextVertex) - GET_X(baseVertex)) * (GET_Y(nextVertex) + GET_Y(baseVertex));

    for (int i = 0; i < poly->vertexCount - 1; i++)
    {
        baseVertex = poly->vertices[i];
        nextVertex = poly->vertices[i + 1];
        area += (GET_X(nextVertex) - GET_X(baseVertex)) * (GET_Y(nextVertex) + GET_Y(baseVertex));
    }

    return area > 0.0f ? true : false; // backwards?
}

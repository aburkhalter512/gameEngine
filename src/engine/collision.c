#include "engine/collision.h"

#include "engine/util.h"
#include "engine/math/float.h"
#include "engine/math/polygon.h"
#include "engine/math/transform.h"
#include "engine/math/vec.h"

#include <math.h>
#include <stdlib.h>

#ifndef UNIT_TEST

struct _collider {
    transform* transform;

    polygon* polygons;
    int polygonCount;
    float radius; // Used for bubble collision detection (cheap and fast to detect)
};
#else
#include "engine/unit/collision.unit.h"
#endif

collider* create_collider(transform* transform, polygon* polygon)
{
    if (!transform || !polygon)
    {
        return NULL;
    }

    collider* c = malloc(sizeof(collider));
    if (!c)
    {
        return NULL;
    }

    c->transform = transform;
    c->radius = 0.0f;

    if (!decompose_polygon(polygon, &c->polygons, &c->polygonCount))
    {
        free(c);
        return NULL;
    }

    for (int i = 0; i < c->polygonCount; i++)
    {
        for (int j = 0; j < c->polygons[i].vertexCount; j++)
        {
            c->radius = fmax(c->radius, radius_vec2f(c->polygons[i].vertices[j]));
        }
    }

    return c;
}

bool free_collider(collider* c)
{
    if (!c || !c->transform)
    {
        return false;
    }

    for (int i = 0; i < c->polygonCount; i++)
    {
        free_polygon(&c->polygons[i]);
    }

    free(c);

    return true;
}

collision create_collision(bool isColliding, vec2f overlap)
{
    return (collision) { isColliding, overlap };
}

// -----------------------------------------------------------------------------
// SAT Collision Detection
// -----------------------------------------------------------------------------
collision _detectCollision_polygon(polygon* base, polygon* target)
{
    // printf("================================ Detecting polygon collision =========================\n");

    // The end points of the polygon projections
    vec2f baseProjection[2];
    vec2f targetProjection[2];

    // Distance squared of various projections
    float largestProjectionDistanceSqrd;
    float smallestProjectionDistanceSqrd;

    // The base polygon axis points
    vec2f slopePoints[2];

    vec2f overlap;
    float overlapDistanceSqrd = INFINITY;

    for (int axisIndex = 0; axisIndex < base->vertexCount; axisIndex++)
    {
        // printf("Axis index: %d\n", axisIndex);
        slopePoints[0] = base->vertices[axisIndex];
        slopePoints[1] = base->vertices[(axisIndex + 1) % base->vertexCount];

        // printf("Base slope points: [0] (%.3f, %.3f), [1] (%.3f, %.3f)\n", GET_X(slopePoints[0]), GET_Y(slopePoints[0]), GET_X(slopePoints[1]), GET_Y(slopePoints[1]));

        vec2f slope = perpendicular_vec2f(to_vec2f(GET_X(slopePoints[1]) - GET_X(slopePoints[0]), GET_Y(slopePoints[1]) - GET_Y(slopePoints[0])));

        // printf("slope: (%.3f, %.3f)\n", GET_X(slope), GET_Y(slope));

        project_polygon(base, slope, baseProjection, &largestProjectionDistanceSqrd);
        project_polygon(target, slope, targetProjection, &smallestProjectionDistanceSqrd);

        // If the largest is not the largest, then swap them
        if (smallestProjectionDistanceSqrd > largestProjectionDistanceSqrd)
        {
            SWAP(smallestProjectionDistanceSqrd, largestProjectionDistanceSqrd);
        }
        // printf("base projection: [0] (%.3f, %.3f), [1] (%.3f, %.3f)\n", GET_X(baseProjection[0]), GET_Y(baseProjection[0]), GET_X(baseProjection[1]), GET_Y(baseProjection[1]));
        // printf("target projection:  [0] (%.3f, %.3f), [1] (%.3f, %.3f)\n", GET_X(targetProjection[0]), GET_Y(targetProjection[0]), GET_X(targetProjection[1]), GET_Y(targetProjection[1]));
        // printf("smallestProjectionDistanceSqrd: %.3f\n", smallestProjectionDistanceSqrd);
        // printf("largestProjectionDistanceSqrd: %.3f\n", largestProjectionDistanceSqrd);
        
        // If one axis doesn't collide, then there is no collision
        float endpointDistanceSqrd[2] = {
            distanceSqrd_vec2f(targetProjection[0], baseProjection[0]),
            distanceSqrd_vec2f(targetProjection[1], baseProjection[1])
        };
        // printf("endpointDistanceSqrd[0]: %.3f\n", endpointDistanceSqrd[0]);
        // printf("endpointDistanceSqrd[1]: %.3f\n", endpointDistanceSqrd[1]);
        

        // printf("\n");

        // If the set endpoints has a longer distance than the largest projection, we don't collide,
        if (endpointDistanceSqrd[0] >= largestProjectionDistanceSqrd + DEFAULT_TOLERANCE ||
            endpointDistanceSqrd[1] >= largestProjectionDistanceSqrd + DEFAULT_TOLERANCE)
        {
            return create_collision(false, to_vec2f(0, 0));
        }
        else
        {
            vec2f overlaps[2] = {
                sub_vec2f(targetProjection[0], baseProjection[1]),
                sub_vec2f(targetProjection[1], baseProjection[0]),
            };

            float overlapsDistanceSqrd[2] = {
                distanceSqrd_vec2f(overlaps[0], to_vec2f(0, 0)),
                distanceSqrd_vec2f(overlaps[1], to_vec2f(0, 0)),
            };

            int smallestOverlapIndex = overlapsDistanceSqrd[1] < overlapsDistanceSqrd[0] ? 1 : 0;

            // Find the minimum overlap and save it
            if (overlapsDistanceSqrd[smallestOverlapIndex] < overlapDistanceSqrd)
            {
                overlapDistanceSqrd = overlapsDistanceSqrd[smallestOverlapIndex];
                overlap = overlaps[smallestOverlapIndex];
            }

            // printf("overlaps: [0] (%.3f, %.3f), [1] (%.3f, %.3f)\n", GET_X(overlaps[0]), GET_Y(overlaps[0]), GET_X(overlaps[1]), GET_Y(overlaps[1]));
            // printf("overlapsDistanceSqrd: [0] (%.3f), [1] (%.3f)\n", overlapsDistanceSqrd[0], overlapsDistanceSqrd[1]);

            // printf("overlap: (%.3f, %.3f)\n", GET_X(overlap), GET_Y(overlap));
            // printf("overlapDistanceSqrd: %.3f\n", overlapDistanceSqrd);
        }

        // printf("\n");
    }

    // Every axis has a project collision, so the triangles are colliding
    return create_collision(true, overlap);
}

collision detectCollision_collider(collider* c1, collider* c2)
{
    if (!c1 || !c2)
    {
        return create_collision(false, to_vec2f(0, 0));;
    }

    // Perfom a bubble collision detection
    if (distance_vec2f(c1->transform->position, c2->transform->position) > c1->radius + c2->radius)
    {
        return create_collision(false, to_vec2f(0, 0));;
    }

    // If the bubbles are colliding, then do polygonal collision checking
    collision c = create_collision(false, to_vec2f(0, 0));;
    for (int c1Index = 0; c1Index < c1->polygonCount && !c.isColliding; c1Index++)
    {
        polygon c1Poly;
        if (!applyTransform_polygon(&c1->polygons[c1Index], c1->transform, &c1Poly))
        {
            // Some memory error happened
            break;
        }

        for (int c2Index = 0; c2Index < c2->polygonCount && !c.isColliding; c2Index++)
        {
            polygon c2Poly;
            if (!applyTransform_polygon(&c2->polygons[c2Index], c2->transform, &c2Poly))
            {
                // Some memory error happened
                break;
            }

            c = _detectCollision_polygon(&c1Poly, &c2Poly);

            free_polygon(&c2Poly);
        }

        free_polygon(&c1Poly);
    }


    return c;
}

#pragma once

#include <stdbool.h>

#include "engine/math/vec.h"

typedef struct _polygon polygon;
typedef struct _transform transform;

typedef struct _collider collider;

typedef struct _collision
{
    bool isColliding;
    vec2f overlap;
} collision;

/*
Creates a new collider from a given transform and polygon

Arguments
    transform* transform: The transform that describes the position, rotation, and scale
        of the collider in world space

    polygon* poly: The polygon that will be used to create the collider. It can be
        concave or convex, but it should never cross through itself (i.e. no figure-8s)

Returns
    Returns a new collider or NULL if any of the arguments were NULL or memory allocation failed
*/
collider* create_collider(transform* transform, polygon* poly);

/*
Frees the collider and all associated memory. The source polygon used to create the collider
is not freed by this function

Arguments
    collider* c: The collider to free

Returns
    Returns false if c == NULL
*/
bool free_collider(collider* c);

/*
Detects if two colliders are colliding.

NOTE: If the colliders are distantly spaced, this function call is performed in O(1) time, otherwise
it takes O(n * m) where n and m are the number of points in each source polygon

Arguments
    collider* c1: The first collider

    collider* c2: The second collider

Returns
    Returns false if either arguments are NULL or if a collision was not detected.
*/
collision detectCollision_collider(collider* c1, collider* c2);


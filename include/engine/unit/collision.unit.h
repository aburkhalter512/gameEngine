#pragma once

#include "util/unit.h"

#include "engine/collision.h"

typedef struct _transform transform;
typedef struct _polygon polygon;

struct _collider {
    transform* transform;

    polygon* polygons;
    int polygonCount;
    float radius; // Used for bubble collision detection (cheap and fast to detect)
};

bool _detectCollision_polygon(polygon* base, polygon* target);

// Internal Unit Tests
PROTOTYPE_TEST(_detectCollision_polygon);

// External Unit Tests
PROTOTYPE_TEST(create_collider_quad);
PROTOTYPE_TEST(detectCollision_collider);

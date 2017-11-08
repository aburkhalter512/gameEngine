#include "engine/unit/collision.unit.h"

#include "engine/math/polygon.h"
#include "engine/math/transform.h"
#include "engine/math/vec.h"
#include "engine/unit/math/polygon.unit.h"

// bool _detectCollision_polygon(polygon* base, polygon* target)
IMPLEMENT_TEST(_detectCollision_polygon)
{
    polygon triangles[2];
    create_polygon(&triangles[0], 3);
    create_polygon(&triangles[1], 3);

    // -------------------------------------------------------------------------
    // 1: 2 triangles, identical, completely overlapping
    // -------------------------------------------------------------------------
    // printf("============ Scenario 1 ================= \n");
    triangles[0].vertices[0] = to_vec2f(0.0f, 0.0f);
    triangles[0].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[0].vertices[2] = to_vec2f(0.5f, 1.0f);

    triangles[1].vertices[0] = to_vec2f(0.0f, 0.0f);
    triangles[1].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[1].vertices[2] = to_vec2f(0.5f, 1.0f);

    if (!_detectCollision_polygon(&triangles[0], &triangles[1]).isColliding)
    {
        FAIL_TEST("Identical polygons that overlap completely failed to collide.");
    }

    // -------------------------------------------------------------------------
    // 2: 2 triangles, diagonal sides touching
    // -------------------------------------------------------------------------
    // printf("============ Scenario 2 ================= \n");
    triangles[0].vertices[0] = to_vec2f(0.0f, 0.0f);
    triangles[0].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[0].vertices[2] = to_vec2f(0.5f, 1.0f);

    triangles[1].vertices[0] = to_vec2f(0.5f, 1.0f);
    triangles[1].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[1].vertices[2] = to_vec2f(1.5f, 1.0f);

    if (!_detectCollision_polygon(&triangles[0], &triangles[1]).isColliding)
    {
        FAIL_TEST("Identical polygons that have 1 side touching failed to collide.");
    }

    // -------------------------------------------------------------------------
    // 3: 2 triangles, vertex touching side
    // -------------------------------------------------------------------------
    // printf("============ Scenario 3 ================= \n");
    triangles[0].vertices[0] = to_vec2f(0.0f, 0.0f);
    triangles[0].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[0].vertices[2] = to_vec2f(0.5f, 1.0f);

    triangles[1].vertices[0] = to_vec2f(0.75, 0.5f);
    triangles[1].vertices[1] = to_vec2f(1.75f, 0.5f);
    triangles[1].vertices[2] = to_vec2f(1.25f, 1.5f);

    if (!_detectCollision_polygon(&triangles[0], &triangles[1]).isColliding)
    {
        FAIL_TEST("Identical polygons that have 1 vertex touching a side failed to collide.");
    }

    // -------------------------------------------------------------------------
    // 4: 2 triangles, partial intersection
    // -------------------------------------------------------------------------
    // printf("============ Scenario 4 ================= \n");
    triangles[0].vertices[0] = to_vec2f(0.0f, 0.0f);
    triangles[0].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[0].vertices[2] = to_vec2f(0.5f, 1.0f);

    triangles[1].vertices[0] = to_vec2f(0.5, 0.5f);
    triangles[1].vertices[1] = to_vec2f(1.5f, 0.5f);
    triangles[1].vertices[2] = to_vec2f(1.0f, 1.5f);

    if (!_detectCollision_polygon(&triangles[0], &triangles[1]).isColliding)
    {
        FAIL_TEST("Identical polygons that partially intersect failed to collide.");
    }

    // -------------------------------------------------------------------------
    // 5: 2 triangles, intersecting axis, but not the whole polygon
    // -------------------------------------------------------------------------
    // printf("============ Scenario 5 ================= \n");
    triangles[0].vertices[0] = to_vec2f(0.0f, 0.0f);
    triangles[0].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[0].vertices[2] = to_vec2f(0.5f, 1.0f);

    triangles[1].vertices[0] = to_vec2f(0.5, 1.1f);
    triangles[1].vertices[1] = to_vec2f(1.5f, 1.1f);
    triangles[1].vertices[2] = to_vec2f(1.0f, 2.1f);

    if (_detectCollision_polygon(&triangles[0], &triangles[1]).isColliding)
    {
        FAIL_TEST("Identical polygons intersect some axes, but not all, but a collision was detected.");
    }

    // -------------------------------------------------------------------------
    // 6: 2 triangles, no intersecting axis, not intersecting polygon
    // -------------------------------------------------------------------------
    // printf("============ Scenario 6 ================= \n");
    triangles[0].vertices[0] = to_vec2f(0.0f, 0.0f);
    triangles[0].vertices[1] = to_vec2f(1.0f, 0.0f);
    triangles[0].vertices[2] = to_vec2f(0.5f, 1.0f);

    triangles[1].vertices[0] = to_vec2f(100.0, 100.0f);
    triangles[1].vertices[1] = to_vec2f(101.0f, 100.f);
    triangles[1].vertices[2] = to_vec2f(100.5f, 101.0f);

    if (_detectCollision_polygon(&triangles[0], &triangles[1]).isColliding)
    {
        FAIL_TEST("Identical polygons that are very distant are should not be colliding, but are detected as so.");
    }

    PASS_TEST();
}

DEFINE_TEST(create_collider_quad)
{
    transform zeroTransform = {
        to_vec2f(0.0f, 0.0f), // position
        0.0f,                 // rotation
        to_vec2f(1.0f, 1.0f), // scale
    };

    vec2f vertices[] = {
        {{{ 0.0f }, { -0.5f }}}, 
        {{{ 0.5f }, { 0.0f }}}, 
        {{{ 0.0f }, { 0.5f }}}, 
        {{{ -0.5f }, { 0.0f }}}
    };
    polygon quad;
    quad.vertexCount = 4;
    quad.vertices = vertices;

    collider* c = create_collider(&zeroTransform, &quad);
    if (!c)
    {
        FAIL_TEST("Could not create collider for convex quad");
    }
    else if (c->polygonCount == 0)
    {
        free_collider(c);
        FAIL_TEST("Created a collider with 0 polygons!");
    }
    else if (c->polygonCount > 1)
    {
        free_collider(c);
        FAIL_TEST("Convex quad polygon decomposed into more than 1 internal polygons!");
    }
    else if (c->radius != 0.5f)
    {
        free_collider(c);
        FAIL_TEST("Quad does not have a radius of 0.5!");
    }

    PASS_TEST();
}

DEFINE_TEST(detectCollision_collider)
{
    FAIL_TEST("stub");
}

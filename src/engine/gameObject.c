#include "engine/gameObject.h"

#include <stdlib.h>

#include "engine/collision.h"
#include "engine/math/polygon.h"
#include "engine/math/vec.h"
#include "engine/render.h"

struct _gameObject {
    uint16_t type;
    void* userdata;
    transform t;
    collider* c;
    render* r;
};

gameObject* create_gameObject(uint16_t type)
{
    gameObject* g = malloc(sizeof(gameObject));
    g->t.position = to_vec2f(0.0f, 0.0f);
    g->t.scale = to_vec2f(1.0f, 1.0f);
    g->t.rotation = 0.0f;

    g->c = NULL;
    g->r = NULL;

    g->type = type;
    g->userdata = NULL;

    return g;
}

bool free_gameObject(gameObject* g)
{
    if (!g)
    {
        return false;
    }

    free_collider(g->c);

    free(g);

    return true;
}

uint16_t getType_gameObject(gameObject* g)
{
    return g->type;
}

void* getUserdata_gameObject(gameObject* g)
{
    if (!g)
    {
        return NULL;
    }

    return g->userdata;
}

bool setUserdata_gameObject(gameObject* g, void* userdata)
{
    if (!g)
    {
        return false;
    }

    g->userdata = userdata;

    return true;
}

transform getTransform_gameObject(gameObject* g)
{
    return g->t;
}

bool setTransform_gameObject(gameObject* g, transform t)
{
    if (!g)
    {
        return false;
    }

    g->t = t;

    return true;
}

bool setCollider_gameObject(gameObject* g, polygon* p)
{
    if (!g || !p || g->c)
    {
        return false;
    }

    g->c = create_collider(&g->t, p);

    return g->c != NULL;
}

collider* getCollider_gameObject(gameObject* g)
{
    if (!g)
    {
        return NULL;
    }

    return g->c;
}

render* getRender_gameObject(gameObject* g)
{
    if (!g)
    {
        return NULL;
    }

    return g->r;
}

bool setRender_gameObject(gameObject* g, renderInfo rI)
{
    if (!g || g->r)
    {
        return false;
    }

    g->r = create_render(&g->t, rI);

    return g->r != NULL;
}

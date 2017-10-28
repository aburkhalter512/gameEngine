#include "engine/gameEnvironment.h"

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "datastructures/hashtable.h"
#include "engine/collision.h"
#include "engine/gameObject.h"
#include "engine/render.h"
#include "engine/util.h"

#include "util/msTimer.h"

const size_t DEFAULT_GAME_OBJECTS_CAPACITY = 1024;

struct _gameEnvironment
{
    gameEvents events;
    gameSettings settings;

    hashtable* gameObjects;
    hashtable* gameObjectQueue; // The queue holds all new gameObjects until run_gameEnvironment() is called
};

gameEnvironment* create_gameEnvironment(gameEvents ge, gameSettings gs)
{
    // All event handlers are required
    if (!ge.onUpdate || !ge.onCollision || !ge.onRenderStart || !ge.onRenderEnd)
    {
        return NULL;
    }

    gameEnvironment* env = malloc(sizeof(gameEnvironment));
    if (!env)
    {
        return NULL;
    }

    env->gameObjects = create_hashtable(DEFAULT_GAME_OBJECTS_CAPACITY, hasher_ptr, comparator_ptr);
    if (!env->gameObjects)
    {
        free_gameEnvironment(env);
        return NULL;
    }

    env->gameObjectQueue = create_hashtable(DEFAULT_GAME_OBJECTS_CAPACITY, hasher_ptr, comparator_ptr);
    if (!env->gameObjectQueue)
    {
        free_gameEnvironment(env);
        return NULL;
    }

    env->events = ge;
    env->settings = gs;

    return env;
}

bool free_gameEnvironment(gameEnvironment* env)
{
    if (!env)
    {
        return false;
    }

    free_hashtable(env->gameObjects);
    free_hashtable(env->gameObjectQueue);
    free(env);

    return true;
}

bool addGameObject_gameEnvironment(gameEnvironment* env, gameObject* g)
{
    if (!env || !g)
    {
        return false;
    }

    return set_hashtable(env->gameObjectQueue, g, g);
}

/*
Calls onUpdate for each gameObject

Arguments
    void (*onUpdate)(gameObject*): The update handler

    gameObject** allGameObjects: The game objects to update

    size_t gameObjectsCount: The size of allGameObjects
*/
void _updateGameObjects_env(void (*onUpdate)(gameObject*), gameObject** allGameObjects, size_t gameObjectsCount)
{
    // printf("_updateGameObjects_env()\n");

    for (size_t i = 0; i < gameObjectsCount; i++)
    {
        onUpdate(allGameObjects[i]);
    }
}

/*
Detects collisions between any two gameObjects and calls onCollision exactly once for each collision

Arguments
    void (*onCollision)(gameObject*, gameObject*): The collision handler

    gameObject** allGameObjects: The game objects to detect collisions with

    size_t gameObjectsCount: The size of allGameObjects
*/
void _detectCollisions_env(void (*onCollision)(gameObject*, gameObject*, collision*), gameObject** allGameObjects, size_t gameObjectsCount)
{
    // printf("_detectCollisions_env()\n");

    if (gameObjectsCount <= 1)
    {
        return;
    }

    // Detect collision between each gameobject exactly once
    for (size_t i = 0; i < gameObjectsCount - 1; i++)
    {
        gameObject* g1 = allGameObjects[i];
        collider* c1 = getCollider_gameObject(g1);
        if (!c1)
        {
            continue;
        }

        // Start at the next index so that we don't check any gameObjects for collisons twice
        for (size_t j = i + 1; j < gameObjectsCount; j++)
        {
            gameObject* g2 = allGameObjects[j];
            collider* c2 = getCollider_gameObject(g2);
            if (!c2)
            {
                continue;
            }

            // We want to order the collision by type, so the callback arguments are consistent between different runs
            if (getType_gameObject(g1) > getType_gameObject(g2))
            {
                SWAP(g1, g2);
                SWAP(c1, c2);
            }

            collision c = detectCollision_collider(c1, c2);
            if (c.isColliding)
            {
                onCollision(g1, g2, &c);
            }
        }
    }
}

/*
Renders all gameObjects

Arguments
    gameObject** allGameObjects: The game objects to render

    size_t gameObjectsCount: The size of allGameObjects
*/
void _render_env(void (*onRenderStart)(), void (*onRenderEnd)(), gameObject** allGameObjects, size_t gameObjectsCount, float aspect)
{
    // printf("_render_env()\n");

    struct timespec startTime = start_msTimer();
    onRenderStart();
    // printf("[TIMER]: render onRenderStart: %llu ms\n", diff_msTimer(&startTime));

    startTime = start_msTimer();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // printf("[TIMER]: render clear: %llu ms\n", diff_msTimer(&startTime));

    startTime = start_msTimer();
    for (size_t i = 0; i < gameObjectsCount; i++)
    {
        render_render(getRender_gameObject(allGameObjects[i]), aspect);
    }
    // printf("[TIMER]: render allGameObjects: %llu ms\n", diff_msTimer(&startTime));

    startTime = start_msTimer();
    onRenderEnd();
    // printf("[TIMER]: render onRenderEnd: %llu ms\n", diff_msTimer(&startTime));
}

void run_gameEnvironment(gameEnvironment* env)
{
    if (!env)
    {
        return;
    }

    struct timespec startTime = start_msTimer();

    // Get all queued gameObjects
    size_t gameObjectQueueCount = getCount_hashtable(env->gameObjectQueue);
    gameObject** gameObjectQueue = (gameObject**) getAll_hashtable(env->gameObjectQueue);

    // Add the queued gameObjects to the main gameObject hashtable
    for (size_t i = 0; i < gameObjectQueueCount; i++)
    {
        set_hashtable(env->gameObjects, gameObjectQueue[i], gameObjectQueue[i]);
    }

    free(gameObjectQueue);

    // Clear the gameObject queue
    clear_hashtable(env->gameObjectQueue);

    size_t gameObjectsCount = getCount_hashtable(env->gameObjects);
    gameObject** allGameObjects = (gameObject**) getAll_hashtable(env->gameObjects);
    // printf("[TIMER]: allGameObjects: %llu ms\n", diff_msTimer(&startTime));

    startTime = start_msTimer();
    _updateGameObjects_env(env->events.onUpdate, allGameObjects, gameObjectsCount);
    // printf("[TIMER]: allGameObjects update: %llu ms\n", diff_msTimer(&startTime));

    startTime = start_msTimer();
    _detectCollisions_env(env->events.onCollision, allGameObjects, gameObjectsCount);
    // printf("[TIMER]: allGameObjects detect collisions: %llu ms\n", diff_msTimer(&startTime));

    startTime = start_msTimer();
    _render_env(env->events.onRenderStart, env->events.onRenderEnd, allGameObjects, gameObjectsCount, env->settings.aspect);
    // printf("[TIMER]: allGameObjects render: %llu ms\n", diff_msTimer(&startTime));

    free(allGameObjects);
}

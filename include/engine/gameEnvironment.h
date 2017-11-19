#pragma once

#include <stdbool.h>

typedef struct _gameEnvironment gameEnvironment;
typedef struct _gameObject gameObject;
typedef struct _collision collision;

typedef void (*onUpdateHandler)(gameEnvironment*, gameObject*);
typedef void (*onCollisionHandler)(gameEnvironment*, gameObject*, gameObject*, collision* c);
typedef void (*onRenderStartHandler)(gameEnvironment*);
typedef void (*onRenderEndHandler)(gameEnvironment*);

typedef struct _gameEvents
{
    onUpdateHandler onUpdate;

    /*
    The handler for all gameobject collisions. It is guaranteed that 
    all arguments are not NULL and that the first gameObject (g1) has a type
    less than or equal to the second gameObject (g2)
    */
    onCollisionHandler onCollision;
    onRenderStartHandler onRenderStart;
    onRenderEndHandler onRenderEnd;
} gameEvents;

typedef struct _gameSettings
{
    float aspect; // height / width
} gameSettings;

/*
Creates a new gameEnvironment.

A gameEnvironment is an isolated context for a game, in that you can have multiple gameEnvironments
running side by side without any issue.

Arguments
    gameEvents ge: The global event handlers used by the gameEnvironment. Each event handler
        is required and should not be NULL

Returns
    Returns the new gameEnvironment or NULL if memory allocation fails, if ge == NULL, or
    if any of the fields in ge are NULL
*/
gameEnvironment* create_gameEnvironment(gameEvents ge, gameSettings gs);

/*
Frees a gameEnvironment

Deallocates all memory associated with a gameEnvironment, rendering it invalid.

NOTE: Any gameObject added to the gameEnvironment is not freed when calling this method.
    You must free them yourself

Arguments
    gameEnvironment* env: The environment to free

Returns
    Returns false if env is NULL
*/
bool free_gameEnvironment(gameEnvironment* env);

/*
Adds a gameObject to the gameEnvironment.

Every gameObject added to the gameEnvironment gets processed when run_gameEnvironment() is called.

Arguments
    gameEnvironment* env: The gameEnvironment to add the gameObject to

    gameObject* g: The gameObject to add.

Returns
    Returns false if the gameObject could not be added to the gameEnvironment.
    This occurs if any of the arguments are NULL or if memory allocation failed
*/
bool addGameObject_gameEnvironment(gameEnvironment* env, gameObject* g);

/*
Removes a gameObject from the gameEnvironment

Every gameObject removed from the gameEnvironment get processed when run_gameEnvironment() is called.

Arguments
    gameEnvironment* env: The gameEnvironment to add the gameObject to

    gameObject* g: The gameObject to remove.

Returns
    Returns the gameObject that will be removed
*/
gameObject* removeGameObject_gameEnvironment(gameEnvironment* env, gameObject* g);

/*
Runs the gameEnvironment a single step

The following actions are performed, in this order:
1. Each gameObject is passed to onUpdate
2. Detect collision between gameObjects, calling onCollsion as necessary
3. The world is rendered to the screen
*/
void run_gameEnvironment(gameEnvironment* env);

#pragma once

#include <stdint.h>

#include "engine/math/transform.h"
#include "engine/render.h"

typedef struct _collider collider;
typedef struct _gameObject gameObject;
typedef struct _polygon polygon;

/*
Creates a new gameObject.

A gameObject is the primary object used by the gameEnvironemnt. It contains the following data:
  * transform: used to position, rotate, and scale the gameObject in the world
  * collider: used for collision detection with other gameObjects

Arguments
    uint16_t type: The type of the gameObject, which can be used to differentiate
        gameObjects.

Returns
    Returns the newly created gameObject, or NULL is memory allocation failed
*/
gameObject* create_gameObject(uint16_t type);

/*
Frees the memory of a gameObject

Arguments
    gameObject* g: The gameObject to free.

Returns
    Returns false if the gameObject was not freed. This usually occurs if g == NULL
*/
bool free_gameObject(gameObject* g);

/*
Returns the type of the gameObject

Arguments
    gameObject* g: The gameObject to get the type from

Returns
    Returns the type of the gameObject
*/
uint16_t getType_gameObject(gameObject* g);

/*
Returns the userdata associated with the game object. Userdata is any arbitrary
data set by the user, that is only used by the user. No library functions ever modify userdata.

Arguments
    gameObject* g: The gameObject to get the userdata from
*/
void* getUserdata_gameObject(gameObject* g);

/*
Sets the userdata for the gameObject. . Userdata is any arbitrary
data set by the user, that is only used by the user. No library functions ever modify userdata.

Arguments
    gameObject* g: The gameObject to set the userdata on

    void* userdata: The userdata to set for the game object

Returns
    Returns false if g is NULL;
*/
bool setUserdata_gameObject(gameObject* g, void* userdata);

/*
Returns the transform for the gameObject

Arguments
    gameObject* g: The gameObject to get the transform from

Returns
    Returns the transform of the gameObject
*/
transform getTransform_gameObject(gameObject* g);

/*
Sets the transform for a given gameObject

Arguments
    gameObject* g: The gameObject for which to set the transform.

    transform t: The transform to set

Returns
    Returns false if the transform was not set. This usually occurs if g == NULL
*/
bool setTransform_gameObject(gameObject* g, transform t);

/*
Returns the colloder for the gameObject

Arguments
    gameObject* g: The gameObject to get the collider from

Returns
    Returns the collider of the gameObject or NULL if it is not set
*/
collider* getCollider_gameObject(gameObject* g);


/*
Creates and sets the collider for the gameObject from a given polygon.

Arguments
    gameObject* g: The gameObject for which to set the collider.

    polygon* p: The polygon from which to create and set the collider. See create_collider() for more info.

Returns
    Returns false if the collider was not set. This usually occurs if g == NULL or
    if internal memory allocation fails
*/
bool setCollider_gameObject(gameObject* g, polygon* p);

/*
Returns the render for the gameObject

Arguments
    gameObject* g: The gameObject to get the render from

Returns
    Returns the render of the gameObject or NULL if it is not set
*/
render* getRender_gameObject(gameObject* g);

/*
Creates and sets the render for the gameObject

Arguments
    gameObject* g: The gameObject for which to set the render

    renderInfo rI: The render info to create the render from. See create_render() for more info.

Returns
    Returns false if the render was not set or could not be created.
*/
bool setRender_gameObject(gameObject* g, renderInfo rI);

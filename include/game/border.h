#pragma once

#include "engine/math/vec.h"

typedef struct _gameObject gameObject;

typedef struct
{
	gameObject *left, *right, *top;
} border;

border create_border();
vec2f getSize_border(gameObject* border);
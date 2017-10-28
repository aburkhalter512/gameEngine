#pragma once

#include "engine/math/vec.h"

#include <stdlib.h>

typedef struct _render render;
typedef struct _transform transform;

typedef struct _renderInfo
{
    vec2f size;

    const char* vertexShader;
    const char* fragmentShader;

    uint32_t textureId;
} renderInfo;
render* create_render(transform* t, renderInfo rI);

void render_render(render* r, float aspect);

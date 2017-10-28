#include "game/paddle.h"

#include <stdlib.h>

#include "engine/collision.h"
#include "engine/gameObject.h"
#include "engine/render.h"
#include "engine/texture.h"

#include "engine/math/polygon.h"
#include "engine/math/vec.h"

#include "game/gameObjectTypes.h"

#include "SDL2/SDL.h"

static vec2f colliderTemplateVertices[4] = {to_vec2f(-0.1f, -0.05f), to_vec2f(0.1f, -0.05f), to_vec2f(0.1f, 0.05f), to_vec2f(-0.1f, 0.05f)};
static polygon colliderTemplate = {
	colliderTemplateVertices, // vertices
	sizeof(colliderTemplateVertices) / sizeof(vec2f), // vertexCount
};

gameObject* create_paddle()
{
	gameObject* paddle = create_gameObject(GameObject_Paddle);
	transform t = getTransform_gameObject(paddle);
	GET_Y(t.position) = -0.85f;
	setTransform_gameObject(paddle, t);

    renderInfo rI = {
        to_vec2f(0.2f, 0.1f), // size

        "resources/shaders/triangles.vert",
        "resources/shaders/triangles.frag",

        get_texture("resources/media/test.png"),
    };

    setRender_gameObject(paddle, rI);

    setCollider_gameObject(paddle, &colliderTemplate);

    return paddle;
}

void update_paddle(gameObject* paddle)
{
	// printf("update_paddle()\n");

	const uint8_t *state = SDL_GetKeyboardState(NULL);
	float dx = 0.0f;
	float dvx = 0.02f;
	dx -= state[SDL_SCANCODE_A] ? dvx : 0.0f;
	dx += state[SDL_SCANCODE_D] ? dvx : 0.0f;

	// printf("     dx: %.1f\n", dx);

	transform t = getTransform_gameObject(paddle);
	GET_X(t.position) += dx;

	setTransform_gameObject(paddle, t);
}

void onCollision_paddle(gameObject* paddle, gameObject* g2, collision* c)
{
    // printf("onCollision with gameObjectType: %d\n", getType_gameObject(g2));
    transform t = getTransform_gameObject(g2);
    // printf("transform: position: (%.3f, %.3f)\n", GET_X(t.position), GET_Y(t.position));
    // printf("overlap: (%.3f, %.3f)\n", GET_X(c->overlap), GET_Y(c->overlap));

    t = getTransform_gameObject(paddle);
    t.position = add_vec2f(t.position, c->overlap);

    setTransform_gameObject(paddle, t);
}

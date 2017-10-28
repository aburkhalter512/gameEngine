#include "game/ball.h"

#include <stdlib.h>

#include "engine/collision.h"
#include "engine/gameObject.h"
#include "engine/render.h"
#include "engine/texture.h"

#include "engine/math/float.h"
#include "engine/math/polygon.h"
#include "engine/math/vec.h"

#include "game/gameObjectTypes.h"
#include "game/border.h"

static vec2f colliderTemplateVertices[4] = {to_vec2f(-0.01f, -0.01f), to_vec2f(0.01f, -0.01f), to_vec2f(0.01f, 0.01f), to_vec2f(-0.01f, 0.01f)};
static polygon colliderTemplate = {
	colliderTemplateVertices, // vertices
	sizeof(colliderTemplateVertices) / sizeof(vec2f), // vertexCount
};

vec2f ballDirection;
float ballMagnitude;
const float ballAcceleration = 0.001f;

gameObject* create_ball()
{
	ballDirection = normalize_vec2f(to_vec2f(1.0f, -1.0f));
	ballMagnitude = 0.0075f;

	gameObject* ball = create_gameObject(GameObject_Ball);
	transform t = getTransform_gameObject(ball);
	t.position = to_vec2f(0.0f, 0.0f);
	setTransform_gameObject(ball, t);

    renderInfo rI = {
        to_vec2f(0.02f, 0.02f), // size

        "resources/shaders/triangles.vert",
        "resources/shaders/triangles.frag",

        get_texture("resources/media/test.png"),
    };

    setRender_gameObject(ball, rI);

    setCollider_gameObject(ball, &colliderTemplate);

    return ball;
}

void update_ball(gameObject* ball)
{
	vec2f ballVelocity = mul_vec2f(ballDirection, ballMagnitude);

	transform t = getTransform_gameObject(ball);
	t.position = add_vec2f(t.position, ballVelocity);

	setTransform_gameObject(ball, t);
}

void onCollision_ball(gameObject* ball, gameObject* g, collision* c)
{
	if (equal_f(GET_X(c->overlap), 0.0f, DEFAULT_TOLERANCE) && equal_f(GET_Y(c->overlap), 0.0f, DEFAULT_TOLERANCE))
	{
		return;
	}

	transform ballTransform = getTransform_gameObject(ball);

	ballTransform.position = add_vec2f(ballTransform.position, c->overlap);
	setTransform_gameObject(ball, ballTransform);

	ballMagnitude += ballAcceleration;
	switch (getType_gameObject(g))
	{
		case GameObject_Paddle:
		{
			if (GET_Y(c->overlap) > 0.0f)
			{
				ballDirection = normalize_vec2f(sub_vec2f(ballTransform.position, getTransform_gameObject(g).position));

				break;
			}
		}
		default:
			ballDirection = normalize_vec2f(reflect_vec2f(ballDirection, c->overlap));
			break;
	}

	printf("ballDirection: %.3f, %.3f\n", GET_X(ballDirection), GET_Y(ballDirection));
}

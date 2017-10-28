#include "game/border.h"

#include <stdlib.h>

#include "engine/gameObject.h"
#include "engine/texture.h"

#include "engine/math/polygon.h"

#include "game/gameObjectTypes.h"

const float aspect = 1024.0f / 768.0f;

vec2f verticalBorderVertices[4] = {to_vec2f(-0.1f, -1.1f), to_vec2f(0.1f, -1.1f), to_vec2f(0.1f, 1.1f), to_vec2f(-0.1f, 1.1f)};
vec2f horizontalBorderVertices[4] = {to_vec2f(-0.1f - aspect, -0.1f), to_vec2f(0.1f + aspect, -0.1f), to_vec2f(0.1f + aspect, 0.1f), to_vec2f(-0.1f - aspect, 0.1f)};

polygon verticalBorder = {
	verticalBorderVertices, // vertices
	sizeof(verticalBorderVertices) / sizeof(vec2f), // vertexCount
};

polygon horizontalBorder = {
	horizontalBorderVertices, // vertices
	sizeof(horizontalBorderVertices) / sizeof(vec2f), // vertexCount
};

vec2f verticalBorderSize;
vec2f horizontalBorderSize;

border create_border()
{
	verticalBorderSize = add_vec2f(verticalBorderVertices[2], verticalBorderVertices[0]);
	horizontalBorderSize = add_vec2f(horizontalBorderVertices[2], horizontalBorderVertices[0]);

	border b;
	transform t;

	// Left border
	b.left = create_gameObject(GameObject_Border);
	t = getTransform_gameObject(b.left);
	t.position = to_vec2f(-0.1f - aspect, 0.0f);
	setTransform_gameObject(b.left, t);
    setCollider_gameObject(b.left, &verticalBorder);
    setUserdata_gameObject(b.left, &verticalBorderSize);

	// Right border
	b.right = create_gameObject(GameObject_Border);
	t = getTransform_gameObject(b.right);
	t.position = to_vec2f(0.1f + aspect, 0.0f);
	setTransform_gameObject(b.right, t);
    setCollider_gameObject(b.right, &verticalBorder);
    setUserdata_gameObject(b.left, &verticalBorderSize);

	// Top border
	b.top = create_gameObject(GameObject_Border);
	t = getTransform_gameObject(b.top);
	t.position = to_vec2f(0.0f, 1.1f);
	setTransform_gameObject(b.top, t);
    setCollider_gameObject(b.top, &horizontalBorder);
    setUserdata_gameObject(b.left, &horizontalBorderSize);

    return b;
}

vec2f getSize_border(gameObject* border)
{
	vec2f* size = (vec2f*) getUserdata_gameObject(border);
	return *size;
}
#pragma once

typedef struct _gameObject gameObject;
typedef struct _collision collision;

gameObject* create_ball();

void update_ball(gameObject* ball);
void onCollision_ball(gameObject* ball, gameObject* g, collision* c);
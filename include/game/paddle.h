#pragma once

typedef struct _gameObject gameObject;
typedef struct _collision collision;

gameObject* create_paddle();

void update_paddle(gameObject* paddle);
void onCollision_paddle(gameObject* paddle, gameObject* g2, collision* c);
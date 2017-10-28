#pragma once

// Don't change the order of this enum without understanding the implications
// The order of gameObjects in the onCollision handler is affected by the values here.
enum GameObjectTypes
{
	GameObject_Ball = 0,
	GameObject_Paddle,
	GameObject_Border,
};
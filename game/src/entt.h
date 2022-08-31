#ifndef _ENTT_H_
#define _ENTT_H_

#include "raylib.h"
#include <string>

struct world;

#define MAX_ENTITIES_IN_WORLD 360

enum entts {mainCube, cam, camSetCurrentlyRendering, light};

struct entt_transform
{
	Vector3 pos, rot, scale;
};

struct entt
{
	std::string id;
	world* containingWorld;

	entt_transform enttTransform; // WARNING: Do not set manually! Always use functions!

	entt() {};
	~entt() {};

	virtual void on_make() {};
	virtual void on_destroy() {};
	virtual void on_update() {};
	virtual void on_draw_2d() {};
	virtual void on_draw_3d() {}; //(Levente): Used when drawing 3d only components!
	
	virtual void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation) {};
	virtual void update_spatial_props(entt_transform inNewEnttTransform) {};

	virtual entt* try_select(Ray inRay, RayCollision inRayCollision) { return nullptr; };
};

#else

#endif
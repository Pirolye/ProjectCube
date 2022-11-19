#ifndef _ENTT_H_
#define _ENTT_H_

#include "raylib.h"
#include <string>

struct world;

#define MAX_ENTITIES_IN_WORLD 360

enum entts {mainCube, cam, light, mainCube_Static};

struct entt_transform
{
	Vector3 pos, rot, scale;
	Quaternion q;
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

	//void draw_gizmo();
	
	//(Levente): We always manually update all entites spatial properties because we need to apply it to whatever matrixes might exist plus it lets us control
	//what component we want where.
	virtual void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation) {};
	//virtual void update_spatial_props(entt_transform inNewEnttTransform) {};

	//(Levente): Prototype of editor entity selection. Sends out a request to all entities to check for collisions between whatever they want to be selectable and the ray 
	//shot out by the currently rendering camera.
	virtual entt* editor_try_select(Ray inRay, RayCollision inRayCollision) { return nullptr; };
	//int currentlyEditingAxis; //(Levente): This is used for initial entt movement.
};

#else

#endif
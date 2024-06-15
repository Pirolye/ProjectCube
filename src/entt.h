#ifndef _ENTT_H_
#define _ENTT_H_

#include "raylib.h"
#include <string>
#include "graphene.h"
#include <typeinfo>

struct world;

#define MAX_ENTITIES_IN_WORLD 1024


struct entt_transform
{
	Vector3 pos, scale;
	graphene_quaternion_t* rot;
};

struct entity_pointer
{
	void* pointer;
	const type_info* type;
};

struct entt
{
	std::string id;
	world* containingWorld;

	entity_pointer* thisInArray;

	entt_transform transform; // WARNING: Do not set manually! Always use functions!


};

void on_make(entt* inEntt);

void on_update(entt* inEntt);
void on_draw_2d(entt* inEntt);
void on_draw_3d(entt* inEntt);

void update_spatial_props(entt* inEntt);

void on_destroy(entt* inEntt);

void editor_try_select(entt* inEntt);

#else

#endif
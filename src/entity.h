#ifndef _ENTITY_H_
#define _ENTITY_H_

//#include "physics.h"
#include "raylib.h"
#include <string>
#include "graphene.h"
#include <typeinfo>
#include "rlights.h"

struct world;

#define MAX_ENTITIES_IN_WORLD 1024

;
struct entt_transform
{
	Vector3 pos, scale;
	graphene_quaternion_t* rot;
};

struct entity_pointer
{
	public: void* entity;
	public: type_info type;
	public: std::string id;

};

struct entity_info
{
	world* containingWorld;

	entt_transform transform; // WARNING: Do not set manually! Always use functions!

	entity_pointer* thisInArray;


};




#else

#endif

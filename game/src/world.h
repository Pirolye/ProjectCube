#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"

#define MAX_ENTITIES_IN_WORLD 360

typedef struct
{
	//(Levente): Maybe we can add bg color here and such...

	entt* entityArray[MAX_ENTITIES_IN_WORLD];
} world;

//init_world_from_file

void make_entt(enttType desiredType, Vector3 startingPos, world inWorld);
void destroy_entt();

void step_world(world inWorld);

void empty_world(world inWorld);

#endif
#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"

#define MAX_ENTITIES_IN_WORLD 360;

struct world
{
	world();
	virtual ~world();

	entt* entityArray[MAX_ENTITIES_IN_WORLD];

	void make_desired_entity(entts inDesiredEntity, Vector3 inStartingPosition);

	void update();
	void draw();
}


#endif

#ifndef _H_PHYSICS_
#define _H_PHYSICS_

#include "q3.h"
#include "raylib.h"
#include "entt.h"

struct dynamic_body
{
	dynamic_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, q3Scene* inContainingPhysicsSpace, bool e);
	~dynamic_body();
	
	q3Scene* containingPhysicsSpace;
	
	q3BodyDef bodyDef;
	q3Body* body;
	q3BoxDef boxDef; // See q3Box.h for settings details
	q3Transform localSpace; // Contains position and orientation, see q3Transform.h for details
	
	bool everEnable;

	entt_transform t;

	void enable();
	void disable();

	void update();

	entt_transform get_updated_spatial_props();
	void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRot);

};



#else

#endif
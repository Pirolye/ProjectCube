#ifndef _H_PHYSICS_
#define _H_PHYSICS_

#include "PxPhysicsAPI.h"
#include "raylib.h"
#include "entity.h"


using namespace physx;

struct dynamic_body
{
	dynamic_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, PxScene* inContainingPhysicsSpace, world* inContainingWorld);
	~dynamic_body();

	PxMaterial* testMaterial;
	PxRigidDynamic* rigidDynamic;

	PxScene* containingPhysicsSpace;
	world* containingWorld;

	transform t;

	void enable();
	void disable();

	void update();

	void get_updated_spatial_props();
	void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot);
	void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale);

};

struct static_body
{
	static_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, PxScene* inContainingPhysicsSpace, world* inContainingWorld);
	~static_body();

	PxMaterial* testMaterial;
	PxRigidStatic* rigidStatic;

	PxScene* containingPhysicsSpace;
	world* containingWorld;

	bool everEnable;

	transform t;

	void enable();
	void disable();

	void update();

	bool shouldBeSleeping;

	void get_updated_spatial_props();
	void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot);
	void update_spatial_props(Vector3 inNewPos, Vector3 inNewScale);

};




#else

#endif
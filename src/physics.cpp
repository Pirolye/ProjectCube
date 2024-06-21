#include "physics.h"
#include "game_instance.h"
#include "assert.h"

#include "stdio.h"

;
#include <iostream>

#include "graphene.h"

dynamic_body::dynamic_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, PxScene* inContainingPhysicsSpace, world* inContainingWorld)
{
	containingPhysicsSpace = inContainingPhysicsSpace;
	containingWorld = inContainingWorld;

	testMaterial = containingWorld->globalPhysics->createMaterial(1.0f, 1.0f, 1.0f);
	rigidDynamic = containingWorld->globalPhysics->createRigidDynamic(PxTransform(PxVec3(inInitialPos.x, inInitialPos.y, inInitialPos.z)));
	PxRigidActorExt::createExclusiveShape(*rigidDynamic, PxBoxGeometry(inInitialDimensions.x, inInitialDimensions.y, inInitialDimensions.z), *testMaterial);


	t.pos = inInitialPos;
	t.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_from_angles(t.rot, inInitialRot.x, inInitialRot.y, inInitialRot.z);
	t.scale = inInitialDimensions;

	containingPhysicsSpace->addActor(*rigidDynamic);
	
	disable();


}

dynamic_body::~dynamic_body()
{
	//containingPhysicsSpace->RemoveBody(body);

	graphene_quaternion_free(t.rot);
}

void dynamic_body::get_updated_spatial_props()
{
	PxTransform physxReturns = rigidDynamic->getGlobalPose();

	t.pos.x = physxReturns.p.x;
	t.pos.y = physxReturns.p.y;
	t.pos.z = physxReturns.p.z;

	graphene_quaternion_init(t.rot, physxReturns.q.x, physxReturns.q.y, physxReturns.q.z, physxReturns.q.w);

}

void dynamic_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{

	t.pos = inNewPos;
	t.scale = inNewScale;
	graphene_quaternion_init_from_quaternion(t.rot, inNewRot);

	graphene_vec4_t* v4 = graphene_vec4_alloc(); graphene_quaternion_to_vec4(t.rot, v4);
	
	PxQuat q(graphene_vec4_get_x(v4), graphene_vec4_get_y(v4), graphene_vec4_get_z(v4), graphene_vec4_get_w(v4));
	
	PxTransform phsyxTransformNew(t.pos.x, t.pos.y, t.pos.z, (q));

	PxShape** currentShape = reinterpret_cast<PxShape**>(malloc(sizeof(PxShape)));

	rigidDynamic->getShapes(currentShape, sizeof(PxShape));

	rigidDynamic->detachShape(*currentShape[0]);

	PxRigidActorExt::createExclusiveShape(*rigidDynamic, PxBoxGeometry(inNewScale.x, inNewScale.y, inNewScale.z), *testMaterial);

	rigidDynamic->setGlobalPose(phsyxTransformNew, false);

	graphene_vec4_free(v4);

}

void dynamic_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale)
{
	t.pos = inNewPos;
	t.scale = inNewScale;

	graphene_vec4_t* v4 = graphene_vec4_alloc(); graphene_quaternion_to_vec4(t.rot, v4);

	PxQuat q(graphene_vec4_get_x(v4), graphene_vec4_get_y(v4), graphene_vec4_get_z(v4), graphene_vec4_get_w(v4));

	PxTransform phsyxTransformNew(t.pos.x, t.pos.y, t.pos.z, (q));

	PxShape** currentShape = reinterpret_cast<PxShape**>(malloc(sizeof(PxShape)));

	rigidDynamic->getShapes(currentShape, sizeof(PxShape));

	rigidDynamic->detachShape(*currentShape[0]);

	PxRigidActorExt::createExclusiveShape(*rigidDynamic, PxBoxGeometry(inNewScale.x, inNewScale.y, inNewScale.z), *testMaterial);

	rigidDynamic->setGlobalPose(phsyxTransformNew, false);

	graphene_vec4_free(v4);

}


void dynamic_body::enable()
{
	rigidDynamic->wakeUp();

}

void dynamic_body::disable()
{
	rigidDynamic->putToSleep();
	
	/*body->SetToSleep();
	shouldBeSleeping = true;*/
}

void dynamic_body::update()
{
	//Get the PhysX results
	PxTransform physxReturns = rigidDynamic->getGlobalPose();

	t.pos.x = physxReturns.p.x;
	t.pos.y = physxReturns.p.y;
	t.pos.z = physxReturns.p.z;

	graphene_quaternion_init(t.rot, physxReturns.q.x, physxReturns.q.y, physxReturns.q.z, physxReturns.q.w);

}



//////////////////////////////////////////////////



static_body::static_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, PxScene* inContainingPhysicsSpace, world* inContainingWorld)
{


	containingPhysicsSpace = inContainingPhysicsSpace;
	containingWorld = inContainingWorld;

	testMaterial = containingWorld->globalPhysics->createMaterial(1.0f, 1.0f, 1.0f);
	rigidStatic = containingWorld->globalPhysics->createRigidStatic(PxTransform(PxVec3(inInitialPos.x, inInitialPos.y, inInitialPos.z)));
	PxRigidActorExt::createExclusiveShape(*rigidStatic, PxBoxGeometry(inInitialDimensions.x, inInitialDimensions.y, inInitialDimensions.z), *testMaterial);


	t.pos = inInitialPos;
	t.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_from_angles(t.rot, inInitialRot.x, inInitialRot.y, inInitialRot.z);
	t.scale = inInitialDimensions;

	containingPhysicsSpace->addActor(*rigidStatic);

	disable();
}

static_body::~static_body()
{
	//containingPhysicsSpace->RemoveBody(body);
	graphene_quaternion_free(t.rot);

}

void static_body::get_updated_spatial_props()
{
	
	PxTransform physxReturns = rigidStatic->getGlobalPose();

	t.pos.x = physxReturns.p.x;
	t.pos.y = physxReturns.p.y;
	t.pos.z = physxReturns.p.z;

	graphene_quaternion_init(t.rot, physxReturns.q.x, physxReturns.q.y, physxReturns.q.z, physxReturns.q.w);

}

void static_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{
	t.pos = inNewPos;
	t.scale = inNewScale;
	graphene_quaternion_init_from_quaternion(t.rot, inNewRot);

	graphene_vec4_t* v4 = graphene_vec4_alloc(); graphene_quaternion_to_vec4(t.rot, v4);

	PxQuat q(graphene_vec4_get_x(v4), graphene_vec4_get_y(v4), graphene_vec4_get_z(v4), graphene_vec4_get_w(v4));

	PxTransform phsyxTransformNew(t.pos.x, t.pos.y, t.pos.z, (q));

	PxShape** currentShape = reinterpret_cast<PxShape**>(malloc(sizeof(PxShape)));

	rigidStatic->getShapes(currentShape, sizeof(PxShape));

	rigidStatic->detachShape(*currentShape[0]);

	PxRigidActorExt::createExclusiveShape(*rigidStatic, PxBoxGeometry(inNewScale.x, inNewScale.y, inNewScale.z), *testMaterial);
	
	rigidStatic->setGlobalPose(phsyxTransformNew, false);

	graphene_vec4_free(v4);

}

void static_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale)
{
	t.pos = inNewPos;
	t.scale = inNewScale;

	PxVec3 x(t.pos.x, t.pos.y, t.pos.z);
	
	PxTransform phsyxTransformNew(x);

	PxShape** currentShape = reinterpret_cast<PxShape**>(malloc(sizeof(PxShape)));

	rigidStatic->getShapes(currentShape, sizeof(PxShape));

	rigidStatic->detachShape(*currentShape[0]);

	PxRigidActorExt::createExclusiveShape(*rigidStatic, PxBoxGeometry(inNewScale.x, inNewScale.y, inNewScale.z), *testMaterial);

	rigidStatic->setGlobalPose(phsyxTransformNew, false);

}


void static_body::enable()
{
	//rigidStatic->wakeUp()
	shouldBeSleeping = false;
}

void static_body::disable()
{
	//rigidStatic->putToSleep()
	shouldBeSleeping = true;
}

void static_body::update()
{
	get_updated_spatial_props();
}
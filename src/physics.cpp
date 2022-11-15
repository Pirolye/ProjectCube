#include "physics.h"
#include "game_instance.h"

dynamic_body::dynamic_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, PxScene* inContainingPhysicsSpace, world* inContainingWorld)
{
	containingPhysicsSpace = inContainingPhysicsSpace;
	containingWorld = inContainingWorld;

	testMaterial = containingWorld->globalPhysics->createMaterial(1.0f, 1.0f, 1.0f);
	rigidDynamic = containingWorld->globalPhysics->createRigidDynamic(PxTransform(PxVec3(inInitialPos.x, inInitialPos.y, inInitialPos.z)));
	PxRigidActorExt::createExclusiveShape(*rigidDynamic, PxBoxGeometry(inInitialDimensions.x, inInitialDimensions.y, inInitialDimensions.z), *testMaterial);


	t.pos = inInitialPos;
	t.rot = inInitialRot;
	t.scale = inInitialDimensions;

	containingPhysicsSpace->addActor(*rigidDynamic);
	
	/*
	

	everEnable = !neverEnable;

	bodyDef.bodyType = eDynamicBody;
	body = containingPhysicsSpace->CreateBody(bodyDef);

	q3Identity(localSpace); // Specify the origin, and identity orientation

	// Create a box at the origin with width, height, depth = (1.0, 1.0, 1.0)
	// and add it to a rigid body. The transform is defined relative to the owning body
	boxDef.Set(localSpace, q3Vec3(inInitialDimensions.x * 2.0f, inInitialDimensions.y * 2.0f, inInitialDimensions.z * 2.0f));
	body->AddBox(boxDef);

	body->SetTransform(q3Vec3(inInitialPos.x, inInitialPos.y, inInitialPos.z));
	//body->SetTransform(q3Vec3(inInitialPos.x, inInitialPos.y, inInitialPos.z), AXIS, ANGLE);
	//(Levente): q3's body only has a raylib type transform setter for the angle (axis, angle as arguments). Will have to experiment with it.

	disable();*/

}

dynamic_body::~dynamic_body()
{
	//containingPhysicsSpace->RemoveBody(body);
}

entt_transform dynamic_body::get_updated_spatial_props()
{
	PxTransform newT = rigidDynamic->getGlobalPose();
	
	t.pos.x = newT.p.x;
	t.pos.y = newT.p.y;
	t.pos.z = newT.p.z;

	t.rot.x = newT.q.getBasisVector0().x;
	t.rot.y = newT.q.getBasisVector0().y;
	t.rot.z = newT.q.getBasisVector0().z;

	return t;
	
	/*t.pos = Vector3{body->GetTransform().position.x, body->GetTransform().position.y, body->GetTransform().position.z};
	
	float x = body->GetTransform().rotation[0].x;

	t.rot = Vector3{ body->GetTransform().rotation[0].x, body->GetTransform().rotation[1].y, body->GetTransform().rotation[2].z };

	//(Levente): Current problem: After spawning, after a random amount of time, the body's rotation gets set to 1. Also when you set it to awake. 
	//There seems to be a correlation between how fast it gets reset on spawn and how close it is to [0;0;0] 

	return t;*/
}

void dynamic_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRot)
{
	t.pos = inNewPos;
	t.scale = inNewScale;
	t.rot = inNewRot;

	PxVec3 c1(0.0f, t.rot.x, 0.0f);
	PxVec3 c2(0.0f, t.rot.y, 0.0f);
	PxVec3 c3(0.0f, t.rot.z, 0.0f);

	PxMat33 m(c1, c2, c3);

	PxTransform newT(t.pos.x, t.pos.y, t.pos.z, PxQuat(m));

	rigidDynamic->setGlobalPose(newT);

	//boxDef.Set(t1, q3Vec3(t.scale.x * 2.0f, t.scale.y * 2.0f, t.scale.z * 2.0f));



	/*t.pos = inNewPos;
	t.scale = inNewScale;
	t.rot = inNewRot;

	q3Transform t1;

	q3Vec3 vx = q3Vec3(t.rot.x, 0.0f, 0.0f);
	q3Vec3 vy = q3Vec3(0.0f, t.rot.y, 0.0f);
	q3Vec3 vz = q3Vec3(0.0f, 0.0f, t.rot.z);

	t1.position = q3Vec3(t.pos.x, t.pos.y, t.pos.z);
	t1.rotation = q3Mat3(vx, vy, vz);

	body->SetTransform(q3Vec3(t.pos.x, t.pos.y, t.pos.z));
	body->SetTransformRotation(t1.rotation);

	if (shouldBeSleeping == true)
	{
		body->SetToSleep();
	}

}
*/
}
void dynamic_body::enable()
{
	/*body->SetToAwake();
	shouldBeSleeping = false;*/
}

void dynamic_body::disable()
{
	/*body->SetToSleep();
	shouldBeSleeping = true;*/
}

void dynamic_body::update()
{
	/*if (everEnable == false)
	{
		disable();
	}
	else
	{

	}*/
}



//////////////////////////////////////////////////

/*

static_body::static_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, q3Scene* inContainingPhysicsSpace, bool neverEnable)
{
	t.pos = inInitialPos;
	t.scale = inInitialDimensions;
	t.rot = inInitialRot;

	everEnable = !neverEnable;

	containingPhysicsSpace = inContainingPhysicsSpace;

	bodyDef.bodyType = eKinematicBody;
	body = containingPhysicsSpace->CreateBody(bodyDef);

	q3Identity(localSpace); // Specify the origin, and identity orientation

	// Create a box at the origin with width, height, depth = (1.0, 1.0, 1.0)
	// and add it to a rigid body. The transform is defined relative to the owning body
	boxDef.Set(localSpace, q3Vec3(inInitialDimensions.x * 2.0f, inInitialDimensions.y * 2.0f, inInitialDimensions.z * 2.0f));
	body->AddBox(boxDef);

	body->SetTransform(q3Vec3(inInitialPos.x, inInitialPos.y, inInitialPos.z));
	//body->SetTransform(q3Vec3(inInitialPos.x, inInitialPos.y, inInitialPos.z), AXIS, ANGLE);
	//(Levente): q3's body only has a raylib type transform setter for the angle (axis, angle as arguments). Will have to experiment with it.

	//disable();

}

static_body::~static_body()
{
	containingPhysicsSpace->RemoveBody(body);
}

entt_transform static_body::get_updated_spatial_props()
{
	t.pos = Vector3{ body->GetTransform().position.x, body->GetTransform().position.y, body->GetTransform().position.z };

	t.rot = Vector3{ body->GetTransform().rotation[0].x, body->GetTransform().rotation[1].y, body->GetTransform().rotation[2].z };

	return t;
}

void static_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRot)
{
	t.pos = inNewPos;
	t.scale = inNewScale;
	t.rot = inNewRot;

	q3Transform t1;

	q3Vec3 vx = q3Vec3(t.rot.x, 0.0f, 0.0f);
	q3Vec3 vy = q3Vec3(0.0f, t.rot.y, 0.0f);
	q3Vec3 vz = q3Vec3(0.0f, 0.0f, t.rot.z);

	t1.position = q3Vec3(t.pos.x, t.pos.y, t.pos.z);
	t1.rotation = q3Mat3(vx, vy, vz);

	body->SetTransform(q3Vec3(t.pos.x, t.pos.y, t.pos.z));
	body->SetTransformRotation(t1.rotation);

	if (shouldBeSleeping == true)
	{
		body->SetToSleep();
	}

	boxDef.Set(t1, q3Vec3(t.scale.x * 2.0f, t.scale.y * 2.0f, t.scale.z * 2.0f));
}

void static_body::enable()
{
	body->SetToAwake();
	shouldBeSleeping = false;
}

void static_body::disable()
{
	body->SetToSleep();
	shouldBeSleeping = true;
}

void static_body::update()
{
	if (everEnable == false)
	{
		disable();
	}
	else
	{

	}
}*/
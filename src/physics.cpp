#include "physics.h"

dynamic_body::dynamic_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, q3Scene* inContainingPhysicsSpace, bool neverEnable)
{
	t.pos = inInitialPos;
	t.scale = inInitialDimensions;
	t.rot = inInitialRot;

	everEnable = !neverEnable;
	
	containingPhysicsSpace = inContainingPhysicsSpace;

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

	disable();

}

dynamic_body::~dynamic_body()
{
	containingPhysicsSpace->RemoveBody(body);
}

entt_transform dynamic_body::get_updated_spatial_props()
{
	t.pos = Vector3{ body->GetTransform().position.x, body->GetTransform().position.y, body->GetTransform().position.z};
	
	float x = body->GetTransform().rotation[1].y;

	t.rot = Vector3{ body->GetTransform().rotation[0].x, body->GetTransform().rotation[1].y, body->GetTransform().rotation[2].z };

	return t;
}

void dynamic_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRot)
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

void dynamic_body::enable()
{
	body->SetToAwake();
	shouldBeSleeping = false;
}

void dynamic_body::disable()
{
	body->SetToSleep();
	shouldBeSleeping = true;
}

void dynamic_body::update()
{
	if (everEnable == false)
	{
		disable();
	}
	else
	{

	}
}



//////////////////////////////////////////////////



static_body::static_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, q3Scene* inContainingPhysicsSpace, bool neverEnable)
{
	t.pos = inInitialPos;
	t.scale = inInitialDimensions;
	t.rot = inInitialRot;

	everEnable = !neverEnable;

	containingPhysicsSpace = inContainingPhysicsSpace;

	bodyDef.bodyType = eStaticBody;
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
}
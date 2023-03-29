#include "physics.h"
#include "game_instance.h"
#include "assert.h"

#include "stdio.h"
//#include "stringstream.h"
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
	t.rot = inInitialRot;
	t.scale = inInitialDimensions;

	containingPhysicsSpace->addActor(*rigidDynamic);
	
	disable();


}

dynamic_body::~dynamic_body()
{
	//containingPhysicsSpace->RemoveBody(body);
}

entt_transform dynamic_body::get_updated_spatial_props()
{
	PxTransform newT = rigidDynamic->getGlobalPose();
	
	//std::cout << "get_updated_spatial_props begins..." << "\n";

	t.pos.x = newT.p.x;
	t.pos.y = newT.p.y;
	t.pos.z = newT.p.z;
	
	
	graphene_quaternion_t* gq = &graphene_quaternion_t{}; // = graphene_quaternion_init_identity(gq);
	gq = graphene_quaternion_init(gq, newT.q.x, newT.q.y, newT.q.z, newT.q.w);
	
	float x = 0.0f;// = float{};
	float y = 0.0f;// = float{};
	float z = 0.0f;// = float{};

	graphene_quaternion_to_angles(gq, &x, &y, &z);

	float x1 = ceil(x * 100.0f) / 100.0f;
	float y1 = ceil(y * 100.0f) / 100.0f;
	float z1 = ceil(z * 100.0f) / 100.0f;

	t.rot.x = x1;
	t.rot.y = y1;
	t.rot.z = z1;

	//delete x, y, z;

	/*
	float x = RAD2DEG * (newT.q.getBasisVector0().x + newT.q.getBasisVector0().y + newT.q.getBasisVector0().z);

	t.rot.x = newT.q.getBasisVector0().x;
	t.rot.y = newT.q.getBasisVector1().y;
	t.rot.z = newT.q.getBasisVector2().z;
	*/

	/*
	Quaternion q1 = QuaternionIdentity();
	q1.x = newT.q.x;
	q1.y = newT.q.y;
	q1.z = newT.q.z;
	q1.w = newT.q.w;
	*/

	//Vector3 xyzRad = QuaternionToEuler(q1);

	//t.rot.x = xyzRad.x * RAD2DEG;
	//t.rot.y = xyzRad.y * RAD2DEG;
	//t.rot.z = xyzRad.z * RAD2DEG;

	/*
	float angle = 0.0f;
	PxVec3 axis(0.0f, 0.0f, 0.0f);
	newT.q.toRadiansAndUnitAxis(angle, axis);

	Vector3 b = toEuler(axis.x, axis.y, axis.z, RAD2DEG*angle); //Vector3RotateByAxisAngle(t.rot, Vector3{axis.x, axis.y, axis.z}, RAD2DEG*angle);

	t.rot.x = b.x;
	t.rot.y = b.y;
	t.rot.z = b.z;
	*/

	//t.rot.x = QuatGetBasisVector0(newT.q);
	//t.rot.y = QuatGetBasisVector1(newT.q);
	//t.rot.z = QuatGetBasisVector2(newT.q);
	
	//std::cout << "get_updated_spatial_props ended." << "\n";

	//t.rot = final;

	//t.rot.x = r.x;
	//t.rot.y = r.y;
	//t.rot.z = r.z;

	//delete angle;
	//delete axis;

	return t;
	
	/*t.pos = Vector3{body->GetTransform().position.x, body->GetTransform().position.y, body->GetTransform().position.z};
	
	float x = body->GetTransform().rotation[0].x;

	t.rot = Vector3{ body->GetTransform().rotation[0].x, body->GetTransform().rotation[1].y, body->GetTransform().rotation[2].z };

	//(Levente): Current problem: After spawning, after a random amount of time, the body's rotation gets set to 1. Also when you set it to awake. 
	//There seems to be a correlation between how fast it gets reset on spawn and how close it is to [0;0;0] 

	return t;*/
}

/*
Quaternion quat_from_euler(float x, float y, float z)
{
	float yaw = z;
	float pitch = y;
	float roll = x;

	Quaternion q;

	q.x = sinf(roll / 2) * cosf(pitch / 2) * cosf(yaw / 2) - cosf(roll / 2) * sinf(pitch / 2) * sinf(yaw / 2);
		q.y = cosf(roll / 2) * sinf(pitch / 2) * cosf(yaw / 2) + sinf(roll / 2) * cosf(pitch / 2) * sinf(yaw / 2)
		q.z = cosf(roll / 2) * cosf(pitch / 2) * sinf(yaw / 2) - sinf(roll / 2) * sinf(pitch / 2) * cosf(yaw / 2)
		q.w = cosf(roll / 2) * cosf(pitch / 2) * np.cos(yaw / 2) + sinf(roll / 2) * sinf(pitch / 2) * sinf(yaw / 2)

	/*
	(yaw, pitch, roll) = (r[0], r[1], r[2])
		qx = sinf(roll / 2) * np.cos(pitch / 2) * np.cos(yaw / 2) - np.cos(roll / 2) * sinf(pitch / 2) * sinf(yaw / 2)
		qy = np.cos(roll / 2) * sinf(pitch / 2) * np.cos(yaw / 2) + sinf(roll / 2) * np.cos(pitch / 2) * sinf(yaw / 2)
		qz = np.cos(roll / 2) * np.cos(pitch / 2) * sinf(yaw / 2) - sinf(roll / 2) * sinf(pitch / 2) * np.cos(yaw / 2)
		qw = np.cos(roll / 2) * np.cos(pitch / 2) * np.cos(yaw / 2) + sinf(roll / 2) * sinf(pitch / 2) * sinf(yaw / 2)
		return[qx, qy, qz, qw]

		float   c = cosf(a.x / 2.0);
		float	d = cosf(a.y / 2.0);
        float	e = cosf(a.z / 2.0);
	    float	f = sinf(a.x / 2.0);
		float	g = sinf(a.y / 2.0);
		float	h = sinf(a.z / 2.0);
		
		q.x = f * d * e + c * g * h;
		q.y = c * g * e - f * d * h; 
		q.z = c * d * h + f * g * e;
		q.w = c * d * e - f * g * h;


	*/
/*

}
*/

void dynamic_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRot)
{
	//std::cout << "update_spatial_props begins...\n";

	t.pos = inNewPos;
	t.scale = inNewScale;
	t.rot = inNewRot;

	PxTransform oldT = rigidDynamic->getGlobalPose();
	//Matrix m = MatrixIdentity();

	//std::cout << "Input is " << std::to_string(t.rot.x) + " " << std::to_string(t.rot.y) + " " << std::to_string(t.rot.z) + " " << "\n";

	//m = MatrixRotateXYZ(Vector3{ DEG2RAD * t.rot.x, DEG2RAD * t.rot.y, DEG2RAD * t.rot.z });

	//Quaternion qa = QuaternionFromMatrix(m);
    /*
	Quaternion qa = QuaternionFromEuler(DEG2RAD*t.rot.z, DEG2RAD * t.rot.y, DEG2RAD * t.rot.x);
	qa.x = qa.y;
	qa.y = qa.z;
	qa.z = qa.x;
	qa.w = cosf(t.rot.x / 2.0f) * cosf(t.rot.y / 2.0f) * cosf(t.rot.z / 2.0f) + sinf(t.rot.x / 2.0f) * sinf(t.rot.y / 2.0f) * sinf(t.rot.z / 2.0f);
	*/

	
	Quaternion qa = QuaternionIdentity();

	Vector3 a = Vector3{ DEG2RAD * t.rot.x, DEG2RAD * t.rot.y, DEG2RAD * t.rot.z };

	//(Levente): Shamelessly stolen from http://www.andre-gaschler.com/rotationconverter/
	float   c = cosf(a.x / 2.0);
	float	d = cosf(a.y / 2.0);
	float	e = cosf(a.z / 2.0);
	float	f = sinf(a.x / 2.0);
	float	g = sinf(a.y / 2.0);
	float	h = sinf(a.z / 2.0);

	qa.x = f * d * e + c * g * h;
	qa.y = c * g * e - f * d * h;
	qa.z = c * d * h + f * g * e;
	qa.w = c * d * e - f * g * h;
	

	//std::cout << "Calculated quat is " + std::to_string(qa.x) + " " << std::to_string(qa.y) + " " << std::to_string(qa.z) + " " << std::to_string(qa.w) + " " << "\n";


	PxQuat q(qa.x, qa.y, qa.z, qa.w);

	PxTransform newT(t.pos.x, t.pos.y, t.pos.z, (q));

	//assert( (newT.isSane() == true) && "New spatial properties of dynamic rigid body must be sane! (PhysX)");
	
	rigidDynamic->setGlobalPose(newT, false);

}


void dynamic_body::enable()
{
	rigidDynamic->wakeUp();

	/*body->SetToAwake();
	shouldBeSleeping = false;*/
}

void dynamic_body::disable()
{
	rigidDynamic->putToSleep();
	
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



static_body::static_body(Vector3 inInitialPos, Vector3 inInitialDimensions, Vector3 inInitialRot, PxScene* inContainingPhysicsSpace, world* inContainingWorld)
{


	containingPhysicsSpace = inContainingPhysicsSpace;
	containingWorld = inContainingWorld;

	testMaterial = containingWorld->globalPhysics->createMaterial(1.0f, 1.0f, 1.0f);
	rigidStatic = containingWorld->globalPhysics->createRigidStatic(PxTransform(PxVec3(inInitialPos.x, inInitialPos.y, inInitialPos.z)));
	PxRigidActorExt::createExclusiveShape(*rigidStatic, PxBoxGeometry(inInitialDimensions.x, inInitialDimensions.y, inInitialDimensions.z), *testMaterial);


	t.pos = inInitialPos;
	t.rot = inInitialRot;
	t.scale = inInitialDimensions;

	containingPhysicsSpace->addActor(*rigidStatic);

	disable();


/*

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
	*/
}

static_body::~static_body()
{
	//containingPhysicsSpace->RemoveBody(body);
}

entt_transform static_body::get_updated_spatial_props()
{
	PxTransform newT = rigidStatic->getGlobalPose();

	//std::cout << "get_updated_spatial_props begins..." << "\n";

	t.pos.x = newT.p.x;
	t.pos.y = newT.p.y;
	t.pos.z = newT.p.z;


	//PxQuat qa = newT.q;

	//PxVec3 r = newT.q.rotate(PxVec3(t.rot.x, t.rot.x, t.rot.x));

	/*
	float* angle = new float(0.0f);
	PxVec3* axis = new PxVec3(0.0f, 0.0f, 0.0f);
	Vector3 pre_final = Vector3{ 1.0f, 1.0f, 1.0f };
	Vector3 final = Vector3{ 0.0f, 0.0f, 0.0f };

	qa.toRadiansAndUnitAxis(*angle, *axis);

	final = Vector3RotateByAxisAngle(pre_final, Vector3{ axis->x, axis->y, axis->z }, *angle);
	*/

	graphene_quaternion_t* gq = &graphene_quaternion_t{}; // = graphene_quaternion_init_identity(gq);
	gq = graphene_quaternion_init(gq, newT.q.x, newT.q.y, newT.q.z, newT.q.w);

	float x = 0.0f;// = float{};
	float y = 0.0f;// = float{};
	float z = 0.0f;// = float{};

	graphene_quaternion_to_angles(gq, &x, &y, &z);

	float x1 = ceil(x * 100.0f) / 100.0f;
	float y1 = ceil(y * 100.0f) / 100.0f;
	float z1 = ceil(z * 100.0f) / 100.0f;

	t.rot.x = x1;
	t.rot.y = y1;
	t.rot.z = z1;

	//delete x, y, z;

	/*
	float x = RAD2DEG * (newT.q.getBasisVector0().x + newT.q.getBasisVector0().y + newT.q.getBasisVector0().z);

	t.rot.x = newT.q.getBasisVector0().x;
	t.rot.y = newT.q.getBasisVector1().y;
	t.rot.z = newT.q.getBasisVector2().z;
	*/

	/*
	Quaternion q1 = QuaternionIdentity();
	q1.x = newT.q.x;
	q1.y = newT.q.y;
	q1.z = newT.q.z;
	q1.w = newT.q.w;
	*/

	//Vector3 xyzRad = QuaternionToEuler(q1);

	//t.rot.x = xyzRad.x * RAD2DEG;
	//t.rot.y = xyzRad.y * RAD2DEG;
	//t.rot.z = xyzRad.z * RAD2DEG;

	/*
	float angle = 0.0f;
	PxVec3 axis(0.0f, 0.0f, 0.0f);
	newT.q.toRadiansAndUnitAxis(angle, axis);

	Vector3 b = toEuler(axis.x, axis.y, axis.z, RAD2DEG*angle); //Vector3RotateByAxisAngle(t.rot, Vector3{axis.x, axis.y, axis.z}, RAD2DEG*angle);

	t.rot.x = b.x;
	t.rot.y = b.y;
	t.rot.z = b.z;
	*/

	//t.rot.x = QuatGetBasisVector0(newT.q);
	//t.rot.y = QuatGetBasisVector1(newT.q);
	//t.rot.z = QuatGetBasisVector2(newT.q);

	//std::cout << "get_updated_spatial_props ended." << "\n";

	//t.rot = final;

	//t.rot.x = r.x;
	//t.rot.y = r.y;
	//t.rot.z = r.z;

	//delete angle;
	//delete axis;

	return t;

	/*t.pos = Vector3{body->GetTransform().position.x, body->GetTransform().position.y, body->GetTransform().position.z};

	float x = body->GetTransform().rotation[0].x;

	t.rot = Vector3{ body->GetTransform().rotation[0].x, body->GetTransform().rotation[1].y, body->GetTransform().rotation[2].z };

	//(Levente): Current problem: After spawning, after a random amount of time, the body's rotation gets set to 1. Also when you set it to awake.
	//There seems to be a correlation between how fast it gets reset on spawn and how close it is to [0;0;0]

	return t;*/
}

void static_body::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRot)
{
	//std::cout << "update_spatial_props begins...\n";

	t.pos = inNewPos;
	t.scale = inNewScale;
	t.rot = inNewRot;

	PxTransform oldT = rigidStatic->getGlobalPose();
	//Matrix m = MatrixIdentity();

	//std::cout << "Input is " << std::to_string(t.rot.x) + " " << std::to_string(t.rot.y) + " " << std::to_string(t.rot.z) + " " << "\n";

	//m = MatrixRotateXYZ(Vector3{ DEG2RAD * t.rot.x, DEG2RAD * t.rot.y, DEG2RAD * t.rot.z });

	//Quaternion qa = QuaternionFromMatrix(m);
	/*
	Quaternion qa = QuaternionFromEuler(DEG2RAD*t.rot.z, DEG2RAD * t.rot.y, DEG2RAD * t.rot.x);
	qa.x = qa.y;
	qa.y = qa.z;
	qa.z = qa.x;
	qa.w = cosf(t.rot.x / 2.0f) * cosf(t.rot.y / 2.0f) * cosf(t.rot.z / 2.0f) + sinf(t.rot.x / 2.0f) * sinf(t.rot.y / 2.0f) * sinf(t.rot.z / 2.0f);
	*/


	Quaternion qa = QuaternionIdentity();

	Vector3 a = Vector3{ DEG2RAD * t.rot.x, DEG2RAD * t.rot.y, DEG2RAD * t.rot.z };

	//(Levente): Shamelessly stolen from http://www.andre-gaschler.com/rotationconverter/
	float   c = cosf(a.x / 2.0);
	float	d = cosf(a.y / 2.0);
	float	e = cosf(a.z / 2.0);
	float	f = sinf(a.x / 2.0);
	float	g = sinf(a.y / 2.0);
	float	h = sinf(a.z / 2.0);

	qa.x = f * d * e + c * g * h;
	qa.y = c * g * e - f * d * h;
	qa.z = c * d * h + f * g * e;
	qa.w = c * d * e - f * g * h;


	//std::cout << "Calculated quat is " + std::to_string(qa.x) + " " << std::to_string(qa.y) + " " << std::to_string(qa.z) + " " << std::to_string(qa.w) + " " << "\n";


	PxQuat q(qa.x, qa.y, qa.z, qa.w);

	PxTransform newT(t.pos.x, t.pos.y, t.pos.z, (q));

	assert((newT.isSane() == true) && "New spatial properties of dynamic rigid body must be sane! (PhysX)");

	rigidStatic->setGlobalPose(newT, false);

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
	if (everEnable == false)
	{
		disable();
	}
	else
	{

	}
}
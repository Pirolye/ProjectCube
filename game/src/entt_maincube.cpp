#include "entt_maincube.h"
#include "rlgl.h"
#include "raymath.h"
#include "assert.h"

;
void entt_maincube::on_make()
{
	cubeModel = LoadModel("../../game/content/model/smallCube/smallCube.obj");                 // Load model
	cubeTexture = LoadTexture("../../game/content/model/smallCube/smallCube_albedo.png"); // Load model texture
	cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = cubeTexture;

	cubeShader = containingWorld->make_shader("../../game/content/model/smallCube/base_lighting.vs", "../../game/content/model/smallCube/smallCube_lighting.fs");
	cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(cubeShader, "matModel");
	int ambientLoc = GetShaderLocation(cubeShader, "ambient");
	SetShaderValue(cubeShader, ambientLoc, containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
	cubeModel.materials[0].shader = cubeShader;

	/*
	collisionBox = dBodyCreate(containingWorld->physicsWorld);

	dMassSetBoxTotal(&collisionBoxMass, 1.0, 0.5, 0.5, 0.5);
	dBodySetMass(collisionBox, &collisionBoxMass);

	collisionBoxGeom = dCreateBox(containingWorld->physicsSpace, 1.0, 1.0, 1.0);

	dGeomSetBody(collisionBoxGeom, collisionBox);
	*/

	/*
	
	btTransform startingTransform;
	startingTransform.setIdentity();
	startingTransform.setOrigin(btVector3(0.0f, 100.0f, 0.0f));
	btDefaultMotionState* startingMotionState = new btDefaultMotionState(startingTransform);

	btVector3* boxHalfSize = new btVector3(btVector3(btScalar(0.5f), btScalar(0.5f), btScalar(0.5f)));

	btBoxShape* collisionShape = new btBoxShape(*boxHalfSize);
	dynamic_castcollisionShape->InitializePolyhedralFeatures();
	//m_CollisionShapes.push_back(collisionShape);

	btRigidBody::btRigidBodyConstructionInfo* info = new btRigidBody::btRigidBodyConstructionInfo(btScalar(10.0f), startingMotionState, collisionShape, btVector3(0.0f, 0.0f, 0.0f));
	btRigidBody* collisionObject = new btRigidBody(*info);

	containingWorld->dynamicsWorld->addRigidBody(collisionObject);

	*/

	bodyDef.bodyType = eDynamicBody;
	body = containingWorld->physicsSpace->CreateBody(bodyDef);
		
	q3BoxDef boxDef; // See q3Box.h for settings details
	q3Transform localSpace; // Contains position and orientation, see q3Transform.h for details
	q3Identity(localSpace); // Specify the origin, and identity orientation

	// Create a box at the origin with width, height, depth = (1.0, 1.0, 1.0)
	// and add it to a rigid body. The transform is defined relative to the owning body
	boxDef.Set(localSpace, q3Vec3(1.0, 1.0, 1.0));
	body->AddBox(boxDef);

	body->SetToSleep();

	update_spatial_props(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero());

}

void entt_maincube::on_destroy()
{
	UnloadTexture(cubeTexture);
	UnloadModel(cubeModel);
	UnloadShader(cubeShader); // REVISIT!!!!

	//delete body; WILL BE REVISITED
}

void entt_maincube::on_draw_3d()
{
	//rlEnableFrontfaceCulling();
	if (containingWorld->currentlySelectedEntt == this && containingWorld->isInEditorMode)
	{
		DrawModel(cubeModel, Vector3Zero(), 1.0f, WHITE);
		DrawModelWires(cubeModel, Vector3Zero(), 1.0f, RED); //ALWAYS DRAW MODEL WITH ZERO PROPS BECAUSE SPATIAL PROPS MANUALLY SET
	}
	else
	{
		DrawModel(cubeModel, Vector3Zero(), 1.0f, WHITE);
	}

	//rlDisableFrontfaceCulling();
}

void entt_maincube::on_draw_2d()
{
	
}

void entt_maincube::on_update()
{
	//float floatType;
	
	//btVector3 pos = collisionObject->getWorldTransform().getOrigin();

	//const dReal* newPos1[3];
	//newPos1[0] = dBodyGetPosition(collisionBox) + sizeof(floatType);
	//newPos1[1] = dBodyGetPosition(collisionBox) + sizeof(floatType) + sizeof(floatType);
	//newPos1[2] = dBodyGetPosition(collisionBox) + sizeof(floatType) + sizeof(floatType) + sizeof(floatType);
	//dVector3 newPos = static_cast<dVector3>(newPos1);
	
	q3Transform pos = body->GetTransform();

	update_spatial_props(Vector3{ pos.position[0], pos.position[1], pos.position[2]}, enttTransform.scale, enttTransform.rot);
	
	if (containingWorld->currentlySelectedEntt == this && containingWorld->isInEditorMode)
	{
		if (IsKeyPressed(KEY_W))
		{
			update_spatial_props(Vector3Add(enttTransform.pos, Vector3{ 0.0f, 2.0f, 0.0f }), enttTransform.scale, enttTransform.rot);
		}
		if (IsKeyPressed(KEY_X))
		{
			body->SetToAwake();
		}

	}
}

void entt_maincube::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation)
{
	enttTransform.pos = inNewPos;
	enttTransform.scale = inNewScale;
	enttTransform.rot = inNewRotation;

	Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ inNewRotation.x * DEG2RAD, inNewRotation.y * DEG2RAD, inNewRotation.z * DEG2RAD });
	Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

	cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	body->SetTransform(q3Vec3(inNewPos.x, inNewPos.y, inNewPos.z));

	//collisionObject->setWorldTransform()

	//dBodySetPosition(collisionBox, enttTransform.pos.x, enttTransform.pos.y, enttTransform.pos.z);

}

/*
*
*
* EDITOR FUNCTION DEFINITIONS
* 
* 
*/

#ifdef DEBUG

entt* entt_maincube::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	// Check ray collision against model meshes
	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < cubeModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(inRay, cubeModel.meshes[m], cubeModel.transform);
		if (meshHitInfo.hit)
		{
			// Save the closest hit mesh
			inRayCollision = meshHitInfo;

			return this;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return NULL;
		}
	}

};

#else

entt* entt_maincube::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#endif
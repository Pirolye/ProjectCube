#include "entt_maincube.h"
#include "rlgl.h"
#include "raymath.h"

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

	collisionBox = dBodyCreate(containingWorld->physicsWorld);

	dMassSetBoxTotal(&collisionBoxMass, 1.0, 0.5, 0.5, 0.5);
	dBodySetMass(collisionBox, &collisionBoxMass);

	collisionBoxGeom = dCreateBox(containingWorld->physicsSpace, 1.0, 1.0, 1.0);

	dGeomSetBody(collisionBoxGeom, collisionBox);
	
	update_spatial_props(Vector3{ 100.0f, 100.0f, 100.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero());

}

void entt_maincube::on_destroy()
{
	UnloadTexture(cubeTexture);
	UnloadModel(cubeModel);
	UnloadShader(cubeShader); // REVISIT!!!!
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
	float floatType;
	
	const dReal* newPos1[3];
	newPos1[0] = dBodyGetPosition(collisionBox) + sizeof(floatType);
	newPos1[1] = dBodyGetPosition(collisionBox) + sizeof(floatType) + sizeof(floatType);
	newPos1[2] = dBodyGetPosition(collisionBox) + sizeof(floatType) + sizeof(floatType) + sizeof(floatType);
	//dVector3 newPos = static_cast<dVector3>(newPos1);
	
	update_spatial_props( Vector3{ *(newPos1[0]), *(newPos1[1]), *(newPos1[2]) }, enttTransform.scale, enttTransform.rot);
	
	if (containingWorld->currentlySelectedEntt == this && containingWorld->isInEditorMode)
	{
		if (IsKeyPressed(KEY_W))
		{
			update_spatial_props(Vector3Add(enttTransform.pos, Vector3{ 0.0f, 100.0f, 0.0f }), enttTransform.scale, enttTransform.rot);
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

	dBodySetPosition(collisionBox, enttTransform.pos.x, enttTransform.pos.y, enttTransform.pos.z);

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
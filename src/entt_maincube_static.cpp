#include "entt_maincube.h"
#include "rlgl.h"
#include "raymath.h"
#include "assert.h"

;
void entt_maincube_static::on_make()
{
	cubeModel = LoadModel("content/model/smallCube/smallCube.obj");                 // Load model
	cubeTexture = LoadTexture("content/model/smallCube/smallCube_albedo.png"); // Load model texture
	cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = cubeTexture;

	cubeShader = containingWorld->make_shader("content/model/smallCube/base_lighting.vs", "content/model/smallCube/smallCube_lighting.fs");
	cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(cubeShader, "matModel");
	int ambientLoc = GetShaderLocation(cubeShader, "ambient");
	SetShaderValue(cubeShader, ambientLoc, containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
	cubeModel.materials[0].shader = cubeShader;

	collisionBox = new static_body(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, containingWorld->gScene, containingWorld);

	transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(transform.rot);
	update_spatial_props(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

}

void entt_maincube_static::on_destroy()
{
	UnloadTexture(cubeTexture);
	UnloadModel(cubeModel);
	UnloadShader(cubeShader); // REVISIT!!!!

	//delete body; WILL BE REVISITED
}

void entt_maincube_static::on_draw_3d()
{
	//rlEnableFrontfaceCulling();
	if (containingWorld->worldEditor.editorCurrentlySelectedEntt == this && containingWorld->worldEditor.isInEditorMode)
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

void entt_maincube_static::on_draw_2d()
{

}

void entt_maincube_static::on_update()
{
	collisionBox->update();
	
	update_spatial_props(collisionBox->t.pos, collisionBox->t.scale, collisionBox->t.rot);

	if (containingWorld->worldEditor.editorCurrentlySelectedEntt == this && containingWorld->worldEditor.isInEditorMode)
	{
		if (IsKeyPressed(KEY_X))
		{
			collisionBox->enable();
		}

	}
}

void entt_maincube_static::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{
	transform.pos = inNewPos;
	transform.scale = inNewScale;
	transform.rot = inNewRot;

	Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
	float x, y, z;
	graphene_quaternion_to_radians(transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });
	Matrix matTranslation = MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z);

	cubeModel.transform = MatrixIdentity();
	cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	collisionBox->update_spatial_props(inNewPos, inNewScale, inNewRot);

}

void entt_maincube_static::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale)
{
	transform.pos = inNewPos;
	transform.scale = inNewScale;

	Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
	Matrix matTranslation = MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z);

	cubeModel.transform = MatrixIdentity();
	cubeModel.transform = MatrixMultiply(matScale, matTranslation);

	collisionBox->update_spatial_props(inNewPos, inNewScale);

}


/*
*
*
* EDITOR FUNCTION DEFINITIONS
*
*
*/

#ifdef DEBUG

entt* entt_maincube_static::editor_try_select(Ray inRay, RayCollision inRayCollision)
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
			return nullptr;
		}
	}

};

#else

entt* entt_maincube_static::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#endif
#include "entt_maincube.h"
#include "rlgl.h"
#include "raymath.h"
#include "assert.h"

#include <iostream>
#include <string>
#include <sstream>

;
void entt_maincube::on_make()
{
	cubeModel = LoadModel("content/model/smallCube/smallCube.obj");                 // Load model
	cubeTexture = LoadTexture("content/model/smallCube/smallCube_albedo.png"); // Load model texture
	cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = cubeTexture;

	cubeShader = containingWorld->make_shader("content/model/smallCube/base_lighting.vs", "content/model/smallCube/smallCube_lighting.fs");
	cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(cubeShader, "matModel");
	int ambientLoc = GetShaderLocation(cubeShader, "ambient");
	SetShaderValue(cubeShader, ambientLoc, containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
	cubeModel.materials[0].shader = cubeShader;

	collisionBox = new dynamic_body(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, containingWorld->gScene, containingWorld);

	transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(transform.rot);
	update_spatial_props(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

}

void entt_maincube::on_destroy()
{
	UnloadTexture(cubeTexture);
	UnloadModel(cubeModel);
	UnloadShader(cubeShader); // REVISIT!!!!

	graphene_quaternion_free(transform.rot);

	//delete body; WILL BE REVISITED
}

void entt_maincube::on_draw_3d()
{
	//rlEnableFrontfaceCulling();
	if (containingWorld->worldEditor.currentlySelectedEntt == this && containingWorld->worldEditor.isInEditorMode)
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
	/*
	std::ostringstream getTheAddress;
	getTheAddress << &(collisionBox->t.rot.x);
	std::string b = getTheAddress.str();

	std::ostringstream getTheAddress1;
	getTheAddress1 << &(collisionBox->t.rot.y);
	std::string b1 = getTheAddress1.str();

	std::ostringstream getTheAddress2;
	getTheAddress2 << &(collisionBox->t.rot.z);
	std::string b2 = getTheAddress2.str();


	std::string a = "Reading rot.x at " + b + " = " + std::to_string(collisionBox->t.rot.x);
	std::string a1 = "Reading rot.y at " + b1 + " = " + std::to_string(collisionBox->t.rot.y);
	std::string a2 = "Reading rot.z at " + b2 + " = " + std::to_string(collisionBox->t.rot.z);

	/*
	DrawText(a.c_str(), 1000, 10  std::stoi(std::to_string(id[14])), 24, WHITE);
	DrawText(a1.c_str(), 1000, 30 * std::stoi(std::to_string(id[14])), 24, WHITE);
	DrawText(a2.c_str(), 1000, 50 * std::stoi(std::to_string(id[14])), 24, WHITE);
	*/

	/*
	DrawText(a.c_str(), 1000, 10, 24, WHITE);
	DrawText(a1.c_str(), 1000, 30, 24, WHITE);
	DrawText(a2.c_str(), 1000, 50, 24, WHITE);
	*/


}

void entt_maincube::on_update()
{

	//(Levente): Not sure this is the way I want to go, but PhysX has a major issue currently where quaternion math fails in certain cases, such as the editor rotation gizmo applying its rotation to the 
	// phsyx object. In order to solve this, in addition to physics not being updated in editor mode already, we also disable the entt transform logic in the editor!
	if (containingWorld->worldEditor.isInEditorMode == false)
	{
		collisionBox->update();
		update_spatial_props(collisionBox->t.pos, collisionBox->t.scale, collisionBox->t.rot);
	}
	
	if (containingWorld->worldEditor.currentlySelectedEntt == this && containingWorld->worldEditor.isInEditorMode)
	{
		if (IsKeyPressed(KEY_X))
		{
			collisionBox->enable();
		}

	}
}

void entt_maincube::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{
	transform.pos = inNewPos;
	transform.scale = inNewScale;
	graphene_quaternion_init_from_quaternion(transform.rot, inNewRotation);

	Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
	Matrix matTranslation = MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z);
	float x, y, z;
	graphene_quaternion_to_radians(transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });

	cubeModel.transform = MatrixIdentity();
	cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	collisionBox->update_spatial_props(inNewPos, inNewScale, inNewRotation);

}

void entt_maincube::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale)
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
			return nullptr;
		}
	}

};

#else

entt* entt_maincube::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#endif
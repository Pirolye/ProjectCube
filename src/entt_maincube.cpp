#include "entity.h"
#include "rlgl.h"
#include "raymath.h"
#include "assert.h"
#include "world.h"

#include <iostream>
#include <string>
#include <sstream>

;
void on_make(entt_maincube* inEntt)
{
	inEntt->cubeModel = LoadModel("content/model/smallCube/smallCube.obj");                 // Load model
	inEntt->cubeTexture = LoadTexture("content/model/smallCube/smallCube_albedo.png"); // Load model texture
	inEntt->cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEntt->cubeTexture;

	inEntt->cubeShader = world_make_shader(inEntt->entityInfo.containingWorld, "content/model/smallCube/base_lighting.vs", "content/model/smallCube/smallCube_lighting.fs");
	inEntt->cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(inEntt->cubeShader, "matModel");
	int ambientLoc = GetShaderLocation(inEntt->cubeShader, "ambient");
	SetShaderValue(inEntt->cubeShader, ambientLoc, inEntt->entityInfo.containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
	inEntt->cubeModel.materials[0].shader = inEntt->cubeShader;

	inEntt->collisionBox = new dynamic_body(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, inEntt->entityInfo.containingWorld->gScene, inEntt->entityInfo.containingWorld);

	inEntt->entityInfo.transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(inEntt->entityInfo.transform.rot);
	update_spatial_props(inEntt, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

}

void on_destroy(entt_maincube* inEntt)
{
	UnloadTexture(inEntt->cubeTexture);
	UnloadModel(inEntt->cubeModel);
	UnloadShader(inEntt->cubeShader); // REVISIT!!!!

	graphene_quaternion_free(inEntt->entityInfo.transform.rot);

	//delete body; WILL BE REVISITED
}

void on_draw_3d(entt_maincube* inEntt)
{
	//rlEnableFrontfaceCulling();
	if (inEntt->entityInfo.containingWorld->worldEditor->currentlySelectedEntt == inEntt && inEntt->entityInfo.containingWorld->worldEditor->isInEditorMode)
	{
		DrawModel(inEntt->cubeModel, Vector3Zero(), 1.0f, WHITE);
		DrawModelWires(inEntt->cubeModel, Vector3Zero(), 1.0f, RED); //ALWAYS DRAW MODEL WITH ZERO PROPS BECAUSE SPATIAL PROPS MANUALLY SET
	}
	else
	{
		DrawModel(inEntt->cubeModel, Vector3Zero(), 1.0f, WHITE);
	}

	//rlDisableFrontfaceCulling();
}

void on_draw_2d(entt_maincube* inEntt)
{
	/*
	std::ostringstream getTheAddress;
	getTheAddress << &(inEntt->collisionBox->t.rot.x);
	std::string b = getTheAddress.str();

	std::ostringstream getTheAddress1;
	getTheAddress1 << &(inEntt->collisionBox->t.rot.y);
	std::string b1 = getTheAddress1.str();

	std::ostringstream getTheAddress2;
	getTheAddress2 << &(inEntt->collisionBox->t.rot.z);
	std::string b2 = getTheAddress2.str();


	std::string a = "Reading rot.x at " + b + " = " + std::to_string(inEntt->collisionBox->t.rot.x);
	std::string a1 = "Reading rot.y at " + b1 + " = " + std::to_string(inEntt->collisionBox->t.rot.y);
	std::string a2 = "Reading rot.z at " + b2 + " = " + std::to_string(inEntt->collisionBox->t.rot.z);

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

void on_update(entt_maincube* inEntt)
{

	//(Levente): Not sure inEntt is the way I want to go, but PhysX has a major issue currently where quaternion math fails in certain cases, such as the editor rotation gizmo applying its rotation to the 
	// phsyx object. In order to solve inEntt, in addition to physics not being updated in editor mode already, we also disable the entt inEntt->entityInfo.transform logic in the editor!
	if (inEntt->entityInfo.containingWorld->worldEditor->isInEditorMode == false)
	{
		inEntt->collisionBox->update();
		update_spatial_props(inEntt, inEntt->collisionBox->t.pos, inEntt->collisionBox->t.scale, inEntt->collisionBox->t.rot);
	}
	
	if (inEntt->entityInfo.containingWorld->worldEditor->currentlySelectedEntt == inEntt && inEntt->entityInfo.containingWorld->worldEditor->isInEditorMode)
	{
		if (IsKeyPressed(KEY_X))
		{
			inEntt->collisionBox->enable();
		}

	}
}

void update_spatial_props(entt_maincube* inEntt, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{
	inEntt->entityInfo.transform.pos = inNewPos;
	inEntt->entityInfo.transform.scale = inNewScale;
	graphene_quaternion_init_from_quaternion(inEntt->entityInfo.transform.rot, inNewRotation);

	Matrix matScale = MatrixScale(inEntt->entityInfo.transform.scale.x, inEntt->entityInfo.transform.scale.y, inEntt->entityInfo.transform.scale.z);
	Matrix matTranslation = MatrixTranslate(inEntt->entityInfo.transform.pos.x, inEntt->entityInfo.transform.pos.y, inEntt->entityInfo.transform.pos.z);
	float x, y, z;
	graphene_quaternion_to_radians(inEntt->entityInfo.transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });

	inEntt->cubeModel.transform = MatrixIdentity();
	inEntt->cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	inEntt->collisionBox->update_spatial_props(inNewPos, inNewScale, inNewRotation);

}

void update_spatial_props(entt_maincube* inEntt, Vector3 inNewPos, Vector3 inNewScale)
{
	inEntt->entityInfo.transform.pos = inNewPos;
	inEntt->entityInfo.transform.scale = inNewScale;

	Matrix matScale = MatrixScale(inEntt->entityInfo.transform.scale.x, inEntt->entityInfo.transform.scale.y, inEntt->entityInfo.transform.scale.z);
	Matrix matTranslation = MatrixTranslate(inEntt->entityInfo.transform.pos.x, inEntt->entityInfo.transform.pos.y, inEntt->entityInfo.transform.pos.z);
	inEntt->cubeModel.transform = MatrixIdentity();
	inEntt->cubeModel.transform = MatrixMultiply(matScale, matTranslation);

	inEntt->collisionBox->update_spatial_props(inNewPos, inNewScale);

}


/*
*
*
* EDITOR FUNCTION DEFINITIONS
* 
* 
*/

#ifdef DEBUG

entt_maincube* editor_try_select(entt_maincube* inEntt)
{
	/*// Check ray collision against model meshes
	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < inEntt->cubeModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(inRay, inEntt->cubeModel.meshes[m], inEntt->cubeModel.inEntt->entityInfo.transform);
		if (meshHitInfo.hit)
		{
			// Save the closest hit mesh
			inRayCollision = meshHitInfo;

			return inEntt;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return nullptr;
		}
	}*/

	return nullptr;
};

#else

entt* editor_try_select(entt_maincube* inEntt)
{
	return nullptr;
};

#endif
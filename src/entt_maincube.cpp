#include "entity.h"
#include "rlgl.h"
#include "raymath.h"
#include "assert.h"
#include "world.h"

#include <iostream>
#include <string>
#include <sstream>

;
void on_make(entity_maincube_data* inData, entity* inEntity)
{
	inData->cubeModel = LoadModel("content/model/smallCube/smallCube.obj");                 // Load model
	inData->cubeTexture = LoadTexture("content/model/smallCube/smallCube_albedo.png"); // Load model texture
	inData->cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inData->cubeTexture;

	inData->cubeShader = world_make_shader(inEntity->containingWorld, "content/model/smallCube/base_lighting.vs", "content/model/smallCube/smallCube_lighting.fs");
	inData->cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(inData->cubeShader, "matModel");
	int ambientLoc = GetShaderLocation(inData->cubeShader, "ambient");
	SetShaderValue(inData->cubeShader, ambientLoc, inEntity->containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
	inData->cubeModel.materials[0].shader = inData->cubeShader;

	inData->collisionBox = new dynamic_body(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, inEntity->containingWorld->gScene, inEntity->containingWorld);

	inEntity->transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(inEntity->transform.rot);
	//update_spatial_props(inEntt, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

	update_spatial_props(inData, inEntity, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

}


void on_destroy(entity_maincube_data* inData, entity* inEntity)
{
	UnloadTexture(inData->cubeTexture);
	UnloadModel(inData->cubeModel);
	UnloadShader(inData->cubeShader); // REVISIT!!!!

	graphene_quaternion_free(inEntity->transform.rot);

	//delete body; WILL BE REVISITED
}



void on_draw_3d(entity_maincube_data* inData, entity* inEntity)
{
	//rlEnableFrontfaceCulling();
	if (inEntity->containingWorld->worldEditor->currentlySelectedEntity == inEntity&& inEntity->containingWorld->worldEditor->isInEditorMode)
	{
		DrawModel(inData->cubeModel, Vector3Zero(), 1.0f, WHITE);
		DrawModelWires(inData->cubeModel, Vector3Zero(), 1.0f, RED); //ALWAYS DRAW MODEL WITH ZERO PROPS BECAUSE SPATIAL PROPS MANUALLY SET
	}
	else
	{
		DrawModel(inData->cubeModel, Vector3Zero(), 1.0f, WHITE);
	}

}

void on_draw_2d(entity_maincube_data* inData, entity* inEntity)
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
	*/
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

void on_update(entity_maincube_data* inData, entity* inEntity)
{

	//(Levente): Not sure inEntt is the way I want to go, but PhysX has a major issue currently where quaternion math fails in certain cases, such as the editor rotation gizmo applying its rotation to the 
	// phsyx object. In order to solve inEntt, in addition to physics not being updated in editor mode already, we also disable the entt inEntt->entityInfo.transform logic in the editor!
	if (inEntity->containingWorld->worldEditor->isInEditorMode == false)
	{
		inData->collisionBox->update();
		update_spatial_props(inData, inEntity, inData->collisionBox->t.pos, inData->collisionBox->t.scale, inData->collisionBox->t.rot);
	}
	else
	{
		if (inEntity->containingWorld->worldEditor->currentlySelectedEntity == inEntity)
		{
			if (IsKeyPressed(KEY_X))
			{
				inData->collisionBox->enable();
			}

		}

	}
}

void update_spatial_props(entity_maincube_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{
	inEntity->transform.pos = inNewPos;
	inEntity->transform.scale = inNewScale;
	graphene_quaternion_init_from_quaternion(inEntity->transform.rot, inNewRotation);

	Matrix matScale = MatrixScale(inEntity->transform.scale.x, inEntity->transform.scale.y, inEntity->transform.scale.z);
	Matrix matTranslation = MatrixTranslate(inEntity->transform.pos.x, inEntity->transform.pos.y, inEntity->transform.pos.z);
	float x, y, z;
	graphene_quaternion_to_radians(inEntity->transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });

	inData->cubeModel.transform = MatrixIdentity();
	inData->cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	inData->collisionBox->update_spatial_props(inNewPos, inNewScale, inNewRotation);

}

void update_spatial_props(entity_maincube_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale)
{
	inEntity->transform.pos = inNewPos;
	inEntity->transform.scale = inNewScale;

	Matrix matScale = MatrixScale(inEntity->transform.scale.x, inEntity->transform.scale.y, inEntity->transform.scale.z);
	Matrix matTranslation = MatrixTranslate(inEntity->transform.pos.x, inEntity->transform.pos.y, inEntity->transform.pos.z);
	inData->cubeModel.transform = MatrixIdentity();
	inData->cubeModel.transform = MatrixMultiply(matScale, matTranslation);

	inData->collisionBox->update_spatial_props(inNewPos, inNewScale);

}


/*
*
*
* EDITOR FUNCTION DEFINITIONS
* 
* 
*/

#ifdef DEBUG

entity* editor_try_select(entity_maincube_data* inData, entity* inEntity)
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
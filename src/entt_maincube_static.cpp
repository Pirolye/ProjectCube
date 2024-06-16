#include "entt_maincube.h"

#include "rlgl.h"
#include "raymath.h"
#include "assert.h"

;
void on_make(entt_maincube_static* inEntt)
{
	inEntt->cubeModel = LoadModel("content/model/smallCube/smallCube.obj");                 // Load model
	inEntt->cubeTexture = LoadTexture("content/model/smallCube/smallCube_albedo.png"); // Load model texture
	inEntt->cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEntt->cubeTexture;

	inEntt->cubeShader = world_make_shader(inEntt->entityInfo.containingWorld, "content/model/smallCube/base_lighting.vs", "content/model/smallCube/smallCube_lighting.fs");
	inEntt->cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(inEntt->cubeShader, "matModel");
	int ambientLoc = GetShaderLocation(inEntt->cubeShader, "ambient");
	SetShaderValue(inEntt->cubeShader, ambientLoc, inEntt->entityInfo.containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
	inEntt->cubeModel.materials[0].shader = inEntt->cubeShader;

	inEntt->collisionBox = new static_body(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, inEntt->entityInfo.containingWorld->gScene, inEntt->entityInfo.containingWorld);

	inEntt->entityInfo.transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(inEntt->entityInfo.transform.rot);
	update_spatial_props(inEntt, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

}

void on_destroy(entt_maincube_static* inEntt)
{
	UnloadTexture(inEntt->cubeTexture);
	UnloadModel(inEntt->cubeModel);
	UnloadShader(inEntt->cubeShader); // REVISIT!!!!

	//delete body; WILL BE REVISITED
}

void on_draw_3d(entt_maincube_static* inEntt)
{
	//rlEnableFrontfaceCulling();
	if (inEntt->entityInfo.containingWorld->worldEditor->currentlySelectedEntity->id == inEntt->entityInfo.thisInArray->id && inEntt->entityInfo.containingWorld->worldEditor->isInEditorMode)
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

void on_draw_2d(entt_maincube_static* inEntt)
{

}

void on_update(entt_maincube_static* inEntt)
{
	inEntt->collisionBox->update();
	
	update_spatial_props(inEntt, inEntt->collisionBox->t.pos, inEntt->collisionBox->t.scale, inEntt->collisionBox->t.rot);

	if (inEntt->entityInfo.containingWorld->worldEditor->currentlySelectedEntity->id == inEntt->entityInfo.thisInArray->id && inEntt->entityInfo.containingWorld->worldEditor->isInEditorMode)
	{
		if (IsKeyPressed(KEY_X))
		{
			inEntt->collisionBox->enable();
		}

	}
}

void update_spatial_props(entt_maincube_static* inEntt, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{
	inEntt->entityInfo.transform.pos = inNewPos;
	inEntt->entityInfo.transform.scale = inNewScale;
	inEntt->entityInfo.transform.rot = inNewRot;

	Matrix matScale = MatrixScale(inEntt->entityInfo.transform.scale.x, inEntt->entityInfo.transform.scale.y, inEntt->entityInfo.transform.scale.z);
	float x, y, z;
	graphene_quaternion_to_radians(inEntt->entityInfo.transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });
	Matrix matTranslation = MatrixTranslate(inEntt->entityInfo.transform.pos.x, inEntt->entityInfo.transform.pos.y, inEntt->entityInfo.transform.pos.z);

	inEntt->cubeModel.transform = MatrixIdentity();
	inEntt->cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	inEntt->collisionBox->update_spatial_props(inNewPos, inNewScale, inNewRot);

}

void update_spatial_props(entt_maincube_static* inEntt, Vector3 inNewPos, Vector3 inNewScale)
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

entity_pointer* editor_try_select(entt_maincube_static* inEntt)
{
	
	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEntt->entityInfo.containingWorld->currentlyRenderingCamera->rayCam);

	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < inEntt->cubeModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEntt->cubeModel.meshes[m], inEntt->cubeModel.transform);
		if (meshHitInfo.hit)
		{
			return inEntt->entityInfo.thisInArray;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return nullptr;
		}
	}

};

#else

entt* editor_try_select(entt_maincube_static* inEntt)
{
	return nullptr;
};

#endif
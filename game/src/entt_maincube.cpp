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

	//update_spatial_props(Vector3Zero(), 1.0f, Vector3Zero());

	//selectionBoundingBox = GetModelBoundingBox(cubeModel);
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
	if (containingWorld->currentlySelectedEntt == this)
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
	if (containingWorld->currentlySelectedEntt == this)
	{
		if (IsKeyPressed(KEY_W))
		{
			update_spatial_props(Vector3Add(enttTransform.pos, Vector3{ -0.3f, 0.0f, 0.0f }), enttTransform.scale, enttTransform.rot);
		}
	}
}

void entt_maincube::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation)
{
	enttTransform.pos = inNewPos;
	enttTransform.scale = inNewScale;
	enttTransform.rot = inNewRotation;

	//cubeModel.transform = MatrixIdentity();

	Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ inNewRotation.x * DEG2RAD, inNewRotation.y * DEG2RAD, inNewRotation.z * DEG2RAD });
	Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

	cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);



	/*
	Matrix matScale = MatrixScale(scale.x, scale.y, scale.z);
	cubeModel.transform = MatrixMultiply(MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z), MatrixRotateXYZ(Vector3{ inNewRotation.x, inNewRotation.y, inNewRotation.z } ));
	cubeModel.transform = MatrixMultiply(MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z), MatrixRotateXYZ(Vector3{ inNewRotation.x, inNewRotation.y, inNewRotation.z } ));
	*/

	//cubeModel.transform = MatrixMultiply(MatrixScale(enttTransform.scale, enttTransform.scale, enttTransform.scale), cubeModel.transform);
	//cubeModel.transform = MatrixMultiply(MatrixRotateXYZ(Vector3{ DEG2RAD * enttTransform.rot.x, DEG2RAD * enttTransform.rot.y, DEG2RAD * enttTransform.rot.z }), cubeModel.transform);
	//cubeModel.transform = MatrixMultiply(MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z), cubeModel.transform);*/

}

void entt_maincube::update_spatial_props(entt_transform inNewEnttTransform)
{/*
	enttTransform.pos = inNewEnttTransform.pos;
	enttTransform.scale = inNewEnttTransform.scale;
	enttTransform.rot = inNewEnttTransform.rot;

	cubeModel.transform = MatrixMultiply(cubeModel.transform, MatrixScale(enttTransform.scale));
	cubeModel.transform = MatrixMultiply(cubeModel.transform, MatrixRotateXYZ(Vector3{ DEG2RAD * enttTransform.rot.x, DEG2RAD * enttTransform.rot.y, DEG2RAD * enttTransform.rot.z }));
	cubeModel.transform = MatrixMultiply(cubeModel.transform, MatrixTranslate(enttTransform.pos.x, enttTransform.pos.y, enttTransform.pos.z));
	*/
}

#ifdef DEBUG

entt* entt_maincube::try_select(Ray inRay, RayCollision inRayCollision)
{
	/*
	RayCollision boxHitInfo = GetRayCollisionBox(inRay, selectionBoundingBox);

	if (boxHitInfo.hit && (boxHitInfo.distance < inRayCollision.distance))
	{
		return this;
	}
	else
	{
		return nullptr;
	}
	*/

	// Check ray collision against model meshes
	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < cubeModel.meshCount; m++)
	{
		// NOTE: We consider the model.transform for the collision check but 
		// it can be checked against any transform Matrix, used when checking against same
		// model drawn multiple times with multiple transforms
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

entt* entt_maincube::try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#endif
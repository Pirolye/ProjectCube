#include "entt_maincube.h"
#include "rlgl.h"
#include "raymath.h"

;
void entt_maincube::on_make()
{
	cubeModel = LoadModel("../../game/content/model/smallCube/smallCube.glb");                 // Load model
	cubeTexture = LoadTexture("../../game/content/model/smallCube/smallCube_albedo.png"); // Load model texture
	cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = cubeTexture;

	cubeShader = containingWorld->make_shader("../../game/content/model/smallCube/base_lighting.vs", "../../game/content/model/smallCube/smallCube_lighting.fs");
	cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(cubeShader, "matModel");
	int ambientLoc = GetShaderLocation(cubeShader, "ambient");
	SetShaderValue(cubeShader, ambientLoc, containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
	cubeModel.materials[0].shader = cubeShader;

	update_spatial_props(Vector3Zero(), 1.0f, Vector3Zero());

	selectionBoundingBox = GetModelBoundingBox(cubeModel);
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
		DrawModel(cubeModel, enttTransform.pos, enttTransform.scale, WHITE);
		DrawModelWires(cubeModel, enttTransform.pos, enttTransform.scale, RED);
	}
	else
	{
		DrawModel(cubeModel, enttTransform.pos, enttTransform.scale, WHITE);
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

void entt_maincube::update_spatial_props(Vector3 inNewPos, float inNewScale, Vector3 inNewRotation)
{
	enttTransform.pos = inNewPos;
	enttTransform.scale = inNewScale;
	enttTransform.rot = inNewRotation;

	cubeModel.transform = MatrixIdentity();

	Vector3 posScaled = Vector3Scale(enttTransform.pos, enttTransform.scale);

	cubeModel.transform = MatrixMultiply(MatrixScale(enttTransform.scale, enttTransform.scale, enttTransform.scale), cubeModel.transform);
	cubeModel.transform = MatrixMultiply(MatrixRotateXYZ(Vector3{ DEG2RAD * enttTransform.rot.x, DEG2RAD * enttTransform.rot.y, DEG2RAD * enttTransform.rot.z }), cubeModel.transform);
	cubeModel.transform = MatrixMultiply(cubeModel.transform, MatrixTranslate(posScaled.x, posScaled.y, posScaled.z));

	//BoundingBox entityBox = BoundingBox
	{ Vector3
		{
			enttTransform.pos.x - enttTransform.scale / 2,
			enttTransform.pos.y - enttTransform.scale / 2,
			enttTransform.pos.z - enttTransform.scale / 2
	},
		Vector3
		{
			enttTransform.pos.x + enttTransform.scale / 2,
			enttTransform.pos.y + enttTransform.scale / 2,
			enttTransform.pos.z + enttTransform.scale / 2
	};
	};
	
	//selectionBoundingBox = entityBox;
	
	/*
	
	selectionBoundingBox.min = Vector3Scale(selectionBoundingBox.min, enttTransform.scale * enttTransform.scale);
	selectionBoundingBox.max = Vector3Scale(selectionBoundingBox.max, enttTransform.scale * enttTransform.scale);



	//Vector3 bruh = { enttTransform.scale / 2.0f, enttTransform.scale / 2.0f, enttTransform.scale / 2.0f };
	Vector3 bruh = { enttTransform.scale, enttTransform.scale, enttTransform.scale };
	
	Vector3 posScaled = Vector3Scale(enttTransform.pos, (enttTransform.scale * enttTransform.scale));

	Vector3 aaa = Vector3{ posScaled.x + posScaled.x + posScaled.x, posScaled.y + posScaled.y + posScaled.y, posScaled.z + posScaled.z + posScaled.z };
	selectionBoundingBox.min = Vector3Add(selectionBoundingBox.min, aaa);
	selectionBoundingBox.max = Vector3Add(selectionBoundingBox.max, aaa);
	//Vector3 bbb = Vector3Subtract(aaa, bruh);

	*/
}

void entt_maincube::update_spatial_props(entt_transform inNewEnttTransform)
{
	enttTransform.pos = inNewEnttTransform.pos;
	enttTransform.scale = inNewEnttTransform.scale;
	enttTransform.rot = inNewEnttTransform.rot;

	cubeModel.transform = MatrixMultiply(cubeModel.transform, MatrixScale(enttTransform.scale, enttTransform.scale, enttTransform.scale));
	cubeModel.transform = MatrixMultiply(cubeModel.transform, MatrixRotateXYZ(Vector3{ DEG2RAD * enttTransform.rot.x, DEG2RAD * enttTransform.rot.y, DEG2RAD * enttTransform.rot.z }));
	cubeModel.transform = MatrixMultiply(cubeModel.transform, MatrixTranslate(enttTransform.pos.x, enttTransform.pos.y, enttTransform.pos.z));

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
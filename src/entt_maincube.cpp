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

	/*
	//(Levente): All of this will be made into a separate header!
	bodyDef.bodyType = eDynamicBody;
	body = containingWorld->physicsSpace->CreateBody(bodyDef);
		
	q3BoxDef boxDef; // See q3Box.h for settings details
	q3Transform localSpace; // Contains position and orientation, see q3Transform.h for details
	q3Identity(localSpace); // Specify the origin, and identity orientation

	// Create a box at the origin with width, height, depth = (1.0, 1.0, 1.0)
	// and add it to a rigid body. The transform is defined relative to the owning body
	boxDef.Set(localSpace, q3Vec3(2.0, 2.0, 2.0));
	body->AddBox(boxDef);

	body->SetToSleep();
	*/

	collisionBox = new dynamic_body(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, containingWorld->gScene, containingWorld);

	update_spatial_props(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{0.0f, 0.0f, 0.0f});

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
	if (containingWorld->editorCurrentlySelectedEntt == this && containingWorld->isInEditorMode)
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

	DrawText(a.c_str(), 1000, 10, 24, WHITE);
	DrawText(a1.c_str(), 1000, 30, 24, WHITE);
	DrawText(a2.c_str(), 1000, 50, 24, WHITE);


}

void entt_maincube::on_update()
{
	//q3Transform pos = body->GetTransform();
	//update_spatial_props(Vector3{ pos.position[0], pos.position[1], pos.position[2]}, enttTransform.scale, enttTransform.rot);
	
	//collisionBox->update();

	entt_transform newT = collisionBox->get_updated_spatial_props();

	Vector3 difference = Vector3{ 0.02f, 0.02f, 0.02f };

	update_spatial_props(newT.pos, newT.scale, newT.rot);
	//entt_transform newI = collisionBox->get_updated_spatial_props();
	//update_spatial_props(newI.pos, newI.scale, Vector3Add(newI.rot, difference));

	if (containingWorld->editorCurrentlySelectedEntt == this && containingWorld->isInEditorMode)
	{
		if (IsKeyPressed(KEY_X))
		{
			collisionBox->enable();
		}

	}
}

void entt_maincube::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation)
{
	enttTransform.pos = inNewPos;
	enttTransform.scale = inNewScale;
	enttTransform.rot = inNewRotation;

	Matrix matScale = MatrixScale(enttTransform.scale.x, enttTransform.scale.y, enttTransform.scale.z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * enttTransform.rot.x , DEG2RAD * enttTransform.rot.y , DEG2RAD * enttTransform.rot.z  });
	Matrix matTranslation = MatrixTranslate(enttTransform.pos.x, enttTransform.pos.y, enttTransform.pos.z);

	cubeModel.transform = MatrixIdentity();
	cubeModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	collisionBox->update_spatial_props(inNewPos, inNewScale, inNewRotation);

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
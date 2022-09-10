#include "world.h"

#include "raylib.h"

;
#include "raymath.h"

#define CAMERA_IMPLEMENTATION
#include "rcamera.h"

;
#include "rlgl.h"

#include "stdlib.h"

void entt_camera::on_make()
{
	rayCam = new Camera3D;
	
	rayCam->position = Vector3{ -5.0f, 0.0f, 0.0f }; // Camera position
	rayCam->target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	rayCam->up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	rayCam->fovy = 70.0f;                                // Camera field-of-view 
	rayCam->projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode((*rayCam), CAMERA_FREE); // Set a free camera mode
	
	//rayCam->position = Vector3{ 0.0f, 0.0f, 0.0f }; // Camera position
	rotation = Vector3Zero();

	//update_spatial_props(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero());
	//update_spatial_props(Vector3{ -5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero());

	debugModel = LoadModel("editor/camera_model.obj1"); //Loading the wrong model here intentionally...

}

void entt_camera::on_destroy() 
{
	delete(rayCam);
};

void entt_camera::on_update() 
{
	assert(dynamic_cast<entt*>(this) != false);
	if (currentlyDrawing != true) return;

	Vector2 delta = GetMouseDelta();

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		mousePosOriginal = GetMousePosition();
	}

	/*if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		HideCursor();

		if (delta.x != 0.0f)
		{
			//CAMERA.angle = Vector2{ CAMERA.angle.x + delta.x * 0.05f, CAMERA.angle.y};
			rotation.z = rotation.z + delta.x * 0.1f;
		}
		if (delta.y != 0.0f)
		{
			//CAMERA.angle = Vector2{ CAMERA.angle.x, CAMERA.angle.y + delta.y * 0.05f};
			rotation.y = rotation.y - delta.y * 0.1f;
		}

		if (IsKeyDown(KEY_W))
		{
			//(Levente): This is not scaled to framerate!!! TODO!!
			dynamic_cast<entt*>(this)->update_spatial_props(Vector3Add(enttTransform.pos, Vector3{ 0.1f, 0.0f, 0.0f }), enttTransform.scale, enttTransform.rot);

		}
		if (IsKeyDown(KEY_S))
		{
			//(Levente): This is not scaled to framerate!!! TODO!!
			dynamic_cast<entt*>(this)->update_spatial_props(Vector3Add(enttTransform.pos, Vector3{ -0.1f, 0.0f, 0.0f }), enttTransform.scale, enttTransform.rot);

		}
		if (IsKeyDown(KEY_A))
		{
			//(Levente): This is not scaled to framerate!!! TODO!!
			dynamic_cast<entt*>(this)->update_spatial_props(Vector3Add(enttTransform.pos, Vector3{ 0.0f, 0.0f, -0.1f }), enttTransform.scale, enttTransform.rot);

		}
		if (IsKeyDown(KEY_D))
		{
			//(Levente): This is not scaled to framerate!!! TODO!!
			dynamic_cast<entt*>(this)->update_spatial_props(Vector3Add(enttTransform.pos, Vector3{ 0.0f, 0.0f, 0.1f }), enttTransform.scale, enttTransform.rot);

		}

	}
	else
	{
		ShowCursor();
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			SetMousePosition(mousePosOriginal.x, mousePosOriginal.y);

		}
	}*/

};

void entt_camera::on_draw_2d()
{
	if (currentlyDrawing != true) return;
	DrawFPS(10, 10);

	if (containingWorld->isInEditorMode)
	{
		DrawText(id.c_str(), 0, 100, 30, WHITE);
		DrawText(std::to_string(rayCam->target.y).c_str(), 0, 200, 30, WHITE);
		DrawText(std::to_string(rayCam->target.z).c_str(), 0, 230, 30, WHITE);
	}
};

void entt_camera::on_draw_3d()
{
	//if (containingWorld->isInEditorMode && currentlyDrawing == false) DrawModel(debugModel, Vector3Zero(), 1.0f, RED);
	
	//(Levente): For now we won't be drawing cameras as the model's transform is not updated. Will do when we switch to custom camera movement.

	if (currentlyDrawing != true) return;
	ClearBackground(BLACK);
	
	if (containingWorld->isInEditorMode)
	{
		DrawGrid(10, 1.0f);
	}

}

void entt_camera::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation)
{
	
	//We will manually update the camera location, rotation target etc...
	
	//rayCam->target = Vector3Zero();
	//rayCam->position = inNewPos;
	//rayCam->target = rotation;

	enttTransform.pos = inNewPos;
	enttTransform.scale = inNewScale;
	enttTransform.rot = inNewRotation;

	Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ inNewRotation.x * DEG2RAD, inNewRotation.y * DEG2RAD, inNewRotation.z * DEG2RAD });
	Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

	debugModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

}

/*
*
*
* EDITOR FUNCTION DEFINITIONS
*
*
*/

#ifdef DEBUG

entt* entt_camera::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	// Check ray collision against model meshes
	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < debugModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(inRay, debugModel.meshes[m], debugModel.transform);
		if (meshHitInfo.hit)
		{
			// Save the closest hit mesh
			inRayCollision = meshHitInfo;

			return NULL;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return NULL;
		}
	}

};

#else

entt* entt_camera::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#endif
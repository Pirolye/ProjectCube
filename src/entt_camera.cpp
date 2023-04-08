#include "world.h"

#include "raylib.h"

;
#include "raymath.h"

;
#define CAMERA_IMPLEMENTATION
#include "rcamera.h"

;
#include "rlgl.h"

#include "stdlib.h"
#include "graphene.h"
#include "assert.h"

/*

BIRO: Originally I wanted to have a multi cam system, where you could have as many cameras as you wanted. However, due to poor software design regarding rcamera.h, I was not 
even close to accomplishing this. Now we have 1 camera which switches properties when entering/exiting editor mode.

*/


void entt_camera::on_make()
{
	rayCam = new Camera3D;
	
	rayCam->position = Vector3{ 0.0f, 0.0f, -5.0f }; // Camera position
	rayCam->target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	rayCam->up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	rayCam->fovy = 70.0f;                                // Camera field-of-view 
	rayCam->projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(transform.rot);

	mode = 0;
	moveSpeed = 0.1f;

	transform.scale = Vector3{ 1.0f, 1.0f, 1.0f };
	euler = Vector3Zero();

	cameraEditorModel = LoadModel("editor/camera_model.obj");
	
}

void entt_camera::on_destroy() 
{
	delete(rayCam);

	UnloadModel(cameraEditorModel);
};

void entt_camera::on_update() 
{
	update_camera();
};

void entt_camera::on_draw_2d()
{
	if (this != containingWorld->currentlyRenderingCamera) return;
	DrawFPS(10, 10);

};

void entt_camera::on_draw_3d()
{
	if (this != containingWorld->currentlyRenderingCamera) return;
	
	if (containingWorld->worldEditor.isInEditorMode)
	{
		DrawGrid(10, 1.0f);

		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (containingWorld->entityArray[i] != NULL)
			{
				if (dynamic_cast<entt_camera*>(containingWorld->entityArray[i]) != nullptr)
				{
					entt_camera* currentCam = dynamic_cast<entt_camera*>(containingWorld->entityArray[i]);
					if(currentCam != this) DrawModel(currentCam->cameraEditorModel, Vector3Zero(), 1.0f, BLUE);
				
				}
			}
		}
	}

}

void entt_camera::set_mode(int inMode, bool inIsForEditorOnly)
{
	if (inMode == 0)
	{
		mode = inMode;

		CameraMoveForward(rayCam, -10.0f, false);
		transform.pos = rayCam->position;

	}
	else if (inMode == 1)
	{
		mode = inMode;
	}
	else
	{
		printf("Tried to set camera.mode to a value not 1 or 0 (invalid) on camera %s!\n", id.c_str());
	}

	isForEditorOnly = inIsForEditorOnly;
}

void entt_camera::update_camera()
{
	if (this != containingWorld->currentlyRenderingCamera) return;
	
	if (mode == 1)
	{
		//(Levente): WARNING! I had to modify rcore.c in order to not put the mouse cursor back in the middle every time you enabled/disabled it! Also, I have to separately get the mouse delta every time I need it, using a variable such as diff in this case doesn't work for some reason.
		
		if (!containingWorld->worldEditor.canManipulateWorld) return;
		
		Vector2 c = GetMousePosition();

		bool canMove = false;

		Vector2 diff = GetMouseDelta(); 

		diff = Vector2{ diff.x * 0.5f, diff.y * 0.5f };
	
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			DisableCursor();
			UpdateCameraPro(rayCam, Vector3Zero(), Vector3{diff.x, diff.y, 0.0f}, 0.0f);

			euler.x = euler.x + diff.x;
			euler.y = euler.y + diff.y;

			if (euler.x > 180.0f) euler.x = 0.0f + diff.x;
			if (euler.x < 0.0f) euler.x = 360.0f + diff.x;

			if (euler.y > 180.0f) euler.y = 0.0f + diff.y;
			if (euler.y < 0.0f) euler.y = 360.0f + diff.y;


			editor_camera_update_model_rotation();
			SetMousePosition(c.x - GetMouseDelta().x, c.y - GetMouseDelta().y);
			canMove = true;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			EnableCursor();
			SetMousePosition(c.x - GetMouseDelta().x, c.y - GetMouseDelta().y);
			canMove = false;
		}

		if (!canMove) return;
		if (IsKeyDown(KEY_W))
		{
			CameraMoveForward(rayCam, moveSpeed, false);
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_S))
		{
			CameraMoveForward(rayCam, -moveSpeed, false);
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_D))
		{
			CameraMoveRight(rayCam, moveSpeed, false);
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_A))
		{
			CameraMoveRight(rayCam, -moveSpeed, false);
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_E))
		{
			rayCam->target = Vector3{ rayCam->target.x, rayCam->target.y + 0.1f, rayCam->target.z };
			rayCam->position = Vector3{ rayCam->position.x, rayCam->position.y + 0.1f, rayCam->position.z };
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_Q))
		{
			rayCam->target = Vector3{ rayCam->target.x, rayCam->target.y - 0.1f, rayCam->target.z };
			rayCam->position = Vector3{ rayCam->position.x, rayCam->position.y - 0.1f, rayCam->position.z };
			transform.pos = rayCam->position;
		}

	}
	else if (mode == 0)
	{
		return;
	}
	else
	{
		return;
	}
	
	

}

void entt_camera::editor_camera_update_model_rotation()
{



	Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
	Matrix matTranslation = MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z);	
	Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD*euler.x, DEG2RAD * euler.y, DEG2RAD * euler.z });

	cameraEditorModel.transform = MatrixIdentity();
	cameraEditorModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
}

void entt_camera::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{
	

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
	return nullptr;
};

#else

entt* entt_camera::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#endif
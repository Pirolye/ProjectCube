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
	
}

void entt_camera::on_destroy() 
{
	delete(rayCam);
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
	
		DrawGrid(10, 1.0f);
	
	if (containingWorld->worldEditor.isInEditorMode)
	{
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
		Vector2 diff = GetMouseDelta(); 

		diff = Vector2{ diff.x * 0.5f, diff.y * 0.5f };
	
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			DisableCursor();
			UpdateCameraPro(rayCam, Vector3Zero(), Vector3{diff.x, diff.y, 0.0f}, 0.0f);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			EnableCursor();
		}


		if (IsKeyDown(KEY_W))
		{
			CameraMoveForward(rayCam, 0.1f, false);
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_S))
		{
			CameraMoveForward(rayCam, -0.1f, false);
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_D))
		{
			CameraMoveRight(rayCam, 0.1f, false);
			transform.pos = rayCam->position;
		}
		if (IsKeyDown(KEY_A))
		{
			CameraMoveRight(rayCam, -0.1f, false);
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
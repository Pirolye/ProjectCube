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

	UpdateCamera(rayCam, CAMERA_CUSTOM); // Set a free camera mode

	transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(transform.rot);

	mode = 1;
	
}

void entt_camera::on_destroy() 
{
	delete(rayCam);
};

void entt_camera::on_update() 
{

};

void entt_camera::on_draw_2d()
{
	DrawFPS(10, 10);

};

void entt_camera::on_draw_3d()
{
	ClearBackground(BLACK);
	
	if (containingWorld->worldEditor.isInEditorMode)
	{
		DrawGrid(10, 1.0f);
	}

}

void entt_camera::update_camera()
{
	Vector2 diff = GetMouseDelta(); // Vector2{ .x * CAMERA_MOUSE_MOVE_SENSITIVITY, GetMouseDelta().y * CAMERA_MOUSE_MOVE_SENSITIVITY };

	diff = Vector2{ diff.x * 0.6f, diff.y * 0.6f };
	
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		DisableCursor();

		Vector3 prev{};
		graphene_quaternion_to_angles(transform.rot, &prev.x, &prev.y, &prev.z);

		graphene_quaternion_t* modifQ = graphene_quaternion_alloc();

		//printf("diff.y: %f \n", diff.y);
		printf("prev.y: %f diff.y: %f \n", prev.y, diff.y);

		if (prev.y < -60.0f)
		{
			 graphene_quaternion_init_from_angles(modifQ, diff.x, fabs(prev.y - -60.0f), 0.0f);

		}
		else if (prev.y > 60.0f)
		{
			graphene_quaternion_init_from_angles(modifQ, diff.x, -1.0f * (prev.y - 60.0f), 0.0f);

		}
		else
		{
			graphene_quaternion_init_from_angles(modifQ, diff.x, diff.y, 0.0f);
		}

		graphene_quaternion_t* finalQ = graphene_quaternion_alloc(); graphene_quaternion_init_identity(finalQ);
		graphene_quaternion_multiply(transform.rot, modifQ, finalQ);
	
		graphene_quaternion_init_from_quaternion(transform.rot, finalQ);

		float x, y, z;
		graphene_quaternion_to_angles(transform.rot, &x, &y, &z);

		Vector3 finalRot{ x - prev.x, y - prev.y, 0.0f };
		Vector3 finalMove{  };

		UpdateCameraPro(rayCam, finalMove, finalRot, 0.0f);

		graphene_quaternion_free(finalQ);
		graphene_quaternion_free(modifQ);



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
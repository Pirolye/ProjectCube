#include "world.h"

#include "raylib.h"

;
#include "raymath.h"

#define CAMERA_IMPLEMENTATION
#include "rcamera.h"

;
#include "rlgl.h"

#include "stdlib.h"

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

	SetCameraMode((*rayCam), CAMERA_FREE); // Set a free camera mode
	
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

void entt_camera::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, Vector3 inNewRotation)
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
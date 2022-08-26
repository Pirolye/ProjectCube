#include "entt_camera.h"

#include "raylib.h"
#include "rlgl.h"

#include "stdlib.h"

void entt_camera::on_make()
{
	rayCam = new Camera3D;
	
	rayCam->position = Vector3{ 4.0f, 0.0f, 0.0f }; // Camera position
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

void entt_camera::on_update() {};

void entt_camera::on_draw_2d()
{
	if (currentlyDrawing != true) return;
	ClearBackground(BLACK);
	DrawFPS(10, 10);

	DrawText(get_id().c_str(), 0, 100, 30, WHITE);
};

void entt_camera::on_draw_3d()
{
	if (currentlyDrawing != true) return;
	DrawGrid(10, 1.0f);
}

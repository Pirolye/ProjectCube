#include "entt_camera.h"

#include "raylib.h"
#include "rlgl.h"

void entt_camera::on_make()
{
	rayCam.position = Vector3{ 4.0f, 0.0f, 0.0f }; // Camera position
	rayCam.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	rayCam.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	rayCam.fovy = 70.0f;                                // Camera field-of-view 
	rayCam.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode(rayCam, CAMERA_CUSTOM); // Set a free camera mode

}

void entt_camera::on_destroy() {};
void entt_camera::on_update()
{
	UpdateCamera(&rayCam);
}

void entt_camera::on_draw()
{
	BeginDrawing();

	rlEnableBackfaceCulling();

	ClearBackground(BLACK);

	BeginMode3D(rayCam);

	rlEnableFrontfaceCulling();
	//DrawModel(mainCube, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
	rlDisableFrontfaceCulling();
	rlEnableBackfaceCulling();

	DrawGrid(10, 1.0f);

	EndMode3D();

	DrawFPS(10, 10);

	EndDrawing();

}

#include "world.h"
#include "rlgl.h"

void make_entt(enttType desiredType, Vector3 startingPos, world inWorld)
{
	entt* result;

	switch (desiredType)
	{
	case CAMERA:
		//result->camera = { 0 };
		result->camera.position = startingPos;
		result->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
		result->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };       // Default config
		result->camera.fovy = 70.0f;
		result->camera.projection = CAMERA_PERSPECTIVE;
		SetCameraMode(result->camera, CAMERA_CUSTOM);

		/*case MAINCUBE:
			result->mainCubeModel   = LoadModel("game/content/model/smallCube/smallCube.obj");
			result->mainCube_albedo = LoadTexture("game/content/model/smallCube/smallCube_albedo.png");
			result->mainCube_shader = LoadShader("game/content/model/smallCube/base_lighting.vs", "game/content/model/smallCube/smallCube_lighting.fs");*/
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld.entityArray[i] == NULL) { inWorld.entityArray[i] = result; }
	}

}

void step_world(world inWorld)
{
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld.entityArray[i] != NULL)
		{
			entt* current = inWorld.entityArray[i];

			switch (current->entityType)
			{
			case CAMERA:
				UpdateCamera(&current->camera);
			}
		}
	}

	BeginDrawing();

		rlEnableBackfaceCulling();

		ClearBackground(BLACK);

		BeginMode3D(camera);

			rlEnableFrontfaceCulling();
			DrawModel(mainCube, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
			rlDisableFrontfaceCulling();
			rlEnableBackfaceCulling();

			DrawGrid(10, 1.0f);

		EndMode3D();

		DrawFPS(10, 10);

	EndDrawing();

}

void empty_world(world inWorld)
{
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld.entityArray[i] != NULL)
		{
			entt* current = inWorld.entityArray[i];
			free current;
		}
	}
}
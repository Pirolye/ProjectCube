#include "entt_maincube.h"
#include "rlgl.h"

;
void entt_maincube::on_make()
{
	cubeModel = LoadModel("game/content/model/smallCube/smallCube.obj");                 // Load model
	cubeTexture = LoadTexture("game/content/model/smallCube/smallCube_albedo.png"); // Load model texture
	cubeModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = cubeTexture;

	pos = Vector3{ 0.0f, 0.0f, 0.0f };
}

void entt_maincube::on_destroy()
{
	UnloadTexture(cubeTexture);
	UnloadModel(cubeModel);
}

void entt_maincube::on_draw_3d()
{
	rlEnableFrontfaceCulling();
	DrawModel(cubeModel, pos, 1.0f, WHITE);
	rlDisableFrontfaceCulling();
}

void entt_maincube::on_draw_2d()
{
	
}

void entt_maincube::on_update()
{
	
}

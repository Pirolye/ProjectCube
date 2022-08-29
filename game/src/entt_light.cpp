#include "world.h"
;
#include "raymath.h"

;
void entt_light::on_make()
{
	for (int i = 0; i != 360; i++)
	{
		rayLight[i] = { 0 };
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		if (containingWorld->currentlyLoadedShaders[i].id != 0 && containingWorld->currentlyLoadedShaders[i].locs != 0)
		{
			for (int i2 = 0; i2 != 360; i2++)
			{
				if (rayLight[i2].enabled != true)
				{
					rayLight[i2] = CreateLight(LIGHT_POINT, Vector3{ -2, 1, -2 }, Vector3Zero(), WHITE, containingWorld->currentlyLoadedShaders[i]);
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}

	if(containingWorld->name == "debug") id = "debug_light_use_y_to_move_" + std::to_string(containingWorld->totalMadeEntts);

	debugModel = LoadModelFromMesh(GenMeshSphere(1.0f, 30, 30));
}

void entt_light::update_light_props(int inType, Vector3 inPos, Vector3 inTarget, Color inColor)
{
	for (int i2 = 0; i2 != 360; i2++)
	{
		if (rayLight[i2].enabled == true)
		{
			rayLight[i2].type = inType;
			rayLight[i2].position = inPos;
			rayLight[i2].target = inTarget;
			rayLight[i2].color = inColor;
		}
		else
		{
			continue;
		}
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		for (int i2 = 0; i2 != 360; i2++)
		{
			if (rayLight[i2].enabled == true)
			{
				if (containingWorld->currentlyLoadedShaders[i].id != 0 && containingWorld->currentlyLoadedShaders[i].locs != 0)
				{
					UpdateLightValues(containingWorld->currentlyLoadedShaders[i], rayLight[i2]);
				}

			}
		}
	}
};

void entt_light::on_destroy() 
{
	UnloadModel(debugModel);
};

void entt_light::on_update() 
{

};

void entt_light::on_draw_2d() {};
void entt_light::on_draw_3d() 
{
	DrawModel(debugModel, rayLight[0].position, 1.0f, WHITE);
};

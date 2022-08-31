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
					rayLight[i2] = CreateLight(LIGHT_POINT, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3Zero(), WHITE, containingWorld->currentlyLoadedShaders[i]);
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}

	enttTransform.pos = Vector3Zero();
	enttTransform.rot = Vector3Zero();
	enttTransform.scale = Vector3{1.0f, 1.0f, 1.0f};

	debugModel = LoadModel("../../game/editor/point_light_model.obj");
	selectionBoundingBox = GetModelBoundingBox(debugModel);
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
#ifdef DEBUG
	
	if (containingWorld->currentlySelectedEntt == dynamic_cast<entt*>(this))
	{

	}

#else

#endif
};

void entt_light::on_draw_2d() {};
void entt_light::on_draw_3d() 
{
	DrawModel(debugModel, rayLight[0].position, 0.7f, YELLOW);
#ifdef DEBUG
	if (containingWorld->currentlySelectedEntt == dynamic_cast<entt*>(this))
	{
	}
		DrawModelWires(debugModel, rayLight[0].position, 0.7f, RED);
		//DrawBoundingBox(selectionBoundingBox, RED);

#else
#endif
};

entt* entt_light::try_select(Ray inRay, RayCollision inRayCollision)
{
	/*
	
	
	RayCollision boxHitInfo = GetRayCollisionBox(inRay, selectionBoundingBox);

	if ((boxHitInfo.hit) && (boxHitInfo.distance < inRayCollision.distance))
	{
		inRayCollision = boxHitInfo;
		return this;
	}
	else
	{
		*/return nullptr;/*
	}
	*/
}

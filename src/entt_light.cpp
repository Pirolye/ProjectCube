#include "world.h"
;
#include "raymath.h"

;
//(Levente): Each shader individually keeps track of light source information. When making a light, we go through each shader and manually update the 
//information.
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

	transform.pos = Vector3Zero();
	transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(transform.rot);
	transform.scale = Vector3{1.0f, 1.0f, 1.0f};

#ifdef DEBUG
	debugModel = LoadModel("editor/point_light_model.obj");
#endif
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

void entt_light::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{
	transform.pos = inNewPos;
	transform.scale = inNewScale;
	transform.rot = inNewRot;

	Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
	float x, y, z;
	graphene_quaternion_to_radians(transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });
	Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

	debugModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	update_light_props(this->rayLight->type, transform.pos, Vector3Zero(), this->rayLight->color);

}

void entt_light::on_destroy() 
{
#ifdef DEBUG
	UnloadModel(debugModel);
#endif
};

void entt_light::on_update() 
{
	if (containingWorld->worldEditor.currentlySelectedEntt == this && containingWorld->worldEditor.isInEditorMode)
	{
	
	}
};

void entt_light::on_draw_2d() 
{

};

void entt_light::on_draw_3d() 
{
	if (containingWorld->worldEditor.isInEditorMode)
	{
		DrawModel(debugModel, Vector3Zero(), 1.0f, YELLOW);

		if (containingWorld->worldEditor.currentlySelectedEntt == this && containingWorld->worldEditor.isInEditorMode)
		{
			DrawModelWires(debugModel, Vector3Zero(), 1.0f, RED);
		}

	}
	
};

/*
*
*
* EDITOR FUNCTION DEFINITIONS
*
*
*/

#ifdef DEBUG

entt* entt_light::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	// Check ray collision against model meshes
	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < debugModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(inRay, debugModel.meshes[m], debugModel.transform);
		if (meshHitInfo.hit)
		{
			// Save the closest hit mesh
			inRayCollision = meshHitInfo;

			return this;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return nullptr;
		}
	}

}

#else

entt* entt_light::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
}

#endif

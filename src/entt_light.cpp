#include "world.h"
;
#include "raymath.h"

;
//(Levente): Each shader individually keeps track of light source information. When making a light, we go through each shader and manually update the 
//information.
void on_make(entt_light* inEntt)
{
	for (int i = 0; i != 360; i++)
	{
		inEntt->rayLight[i] = { 0 };
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		if (inEntt->containingWorld->currentlyLoadedShaders[i].id != 0 && inEntt->containingWorld->currentlyLoadedShaders[i].locs != 0)
		{
			for (int i2 = 0; i2 != 360; i2++)
			{
				if (inEntt->rayLight[i2].enabled != true)
				{
					inEntt->rayLight[i2] = CreateLight(LIGHT_POINT, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3Zero(), WHITE, inEntt->containingWorld->currentlyLoadedShaders[i]);
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}

	inEntt->transform.pos = Vector3Zero();
	inEntt->transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(inEntt->transform.rot);
	inEntt->transform.scale = Vector3{1.0f, 1.0f, 1.0f};

#ifdef DEBUG
	inEntt->debugModel = LoadModel("editor/point_light_model.obj");
#endif
}

void update_light_props(entt_light* inEntt, int inType, Vector3 inPos, Vector3 inTarget, Color inColor)
{
	for (int i2 = 0; i2 != 360; i2++)
	{
		if (inEntt->rayLight[i2].enabled == true)
		{
			inEntt->rayLight[i2].type = inType;
			inEntt->rayLight[i2].position = inPos;
			inEntt->rayLight[i2].target = inTarget;
			inEntt->rayLight[i2].color = inColor;
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
			if (inEntt->rayLight[i2].enabled == true)
			{
				if (inEntt->containingWorld->currentlyLoadedShaders[i].id != 0 && inEntt->containingWorld->currentlyLoadedShaders[i].locs != 0)
				{
					UpdateLightValues(inEntt->containingWorld->currentlyLoadedShaders[i], inEntt->rayLight[i2]);
				}

			}
		}
	}
};

void update_spatial_props(entt_light* inEntt, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{
	inEntt->transform.pos = inNewPos;
	inEntt->transform.scale = inNewScale;
	inEntt->transform.rot = inNewRot;

	Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
	float x, y, z;
	graphene_quaternion_to_radians(inEntt->transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });
	Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

	inEntt->debugModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	update_light_props(inEntt, inEntt->rayLight->type, inEntt->transform.pos, Vector3Zero(), inEntt->rayLight->color);

}

void on_destroy(entt_light* inEntt)
{
#ifdef DEBUG
	UnloadModel(inEntt->debugModel);
#endif
};

void on_update(entt_light* inEntt)
{
	if (inEntt->containingWorld->worldEditor->currentlySelectedEntt == inEntt && inEntt->containingWorld->worldEditor->isInEditorMode)
	{
	
	}
};

void on_draw_2d(entt_light* inEntt)
{

};

void on_draw_3d(entt_light* inEntt)
{
	if (inEntt->containingWorld->worldEditor->isInEditorMode)
	{
		DrawModel(inEntt->debugModel, Vector3Zero(), 1.0f, YELLOW);

		if (inEntt->containingWorld->worldEditor->currentlySelectedEntt == inEntt && inEntt->containingWorld->worldEditor->isInEditorMode)
		{
			DrawModelWires(inEntt->debugModel, Vector3Zero(), 1.0f, RED);
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

entt_light* editor_try_select(entt_light* inEntt)
{
	/*
	// Check ray collision against model meshes
	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < debugModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(inRay, debugModel.meshes[m], debugModel.transform);
		if (meshHitInfo.hit)
		{
			// Save the closest hit mesh
			inRayCollision = meshHitInfo;

			return inEntt;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return nullptr;
		}
	}*/

	return nullptr;
}

#else

entt* editor_try_select(entt_maincube* inEntt)
{
	return nullptr;
}

#endif

#include "world.h"
;
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "raylib.h"

;



//(Levente): Each shader individually keeps track of light source information. When making a light, we go through each shader and manually update the 
//information.
void on_make(entity_light_data* inData, entity* inEntity)
{
	for (int i = 0; i != 360; i++)
	{
		inData->rayLight[i] = { 0 };
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		if (inEntity->containingWorld->currentlyLoadedShaders[i].id != 0 && inEntity->containingWorld->currentlyLoadedShaders[i].locs != 0)
		{
			for (int i2 = 0; i2 != 360; i2++)
			{
				if (inData->rayLight[i2].enabled != true)
				{
					inData->rayLight[i2] = CreateLight(LIGHT_POINT, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3Zero(), WHITE, inEntity->containingWorld->currentlyLoadedShaders[i]);
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}

	inEntity->transform.pos = Vector3Zero();
	inEntity->transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(inEntity->transform.rot);
	inEntity->transform.scale = Vector3{1.0f, 1.0f, 1.0f};

#ifdef DEBUG
	inData->debugModel = LoadModel("editor/point_light_model.obj");
#endif
}

void update_light_props(entity_light_data* inData, entity* inEntity, int inType, Vector3 inPos, Vector3 inTarget, Color inColor)
{
	for (int i2 = 0; i2 != 360; i2++)
	{
		if (inData->rayLight[i2].enabled == true)
		{
			inData->rayLight[i2].type = inType;
			inData->rayLight[i2].position = inPos;
			inData->rayLight[i2].target = inTarget;
			inData->rayLight[i2].color = inColor;
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
			if (inData->rayLight[i2].enabled == true)
			{
				if (inEntity->containingWorld->currentlyLoadedShaders[i].id != 0 && inEntity->containingWorld->currentlyLoadedShaders[i].locs != 0)
				{
					UpdateLightValues(inEntity->containingWorld->currentlyLoadedShaders[i], inData->rayLight[i2]);
				}

			}
		}
	}
};

void update_spatial_props(entity_light_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{
	inEntity->transform.pos = inNewPos;
	inEntity->transform.scale = inNewScale;
	inEntity->transform.rot = inNewRot;

	Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
	float x, y, z;
	graphene_quaternion_to_radians(inEntity->transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });
	Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

	inData->debugModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	update_light_props(inData, inEntity, inData->rayLight->type, inEntity->transform.pos, Vector3Zero(), inData->rayLight->color);

}

void on_destroy(entity_light_data* inData, entity* inEntity)
{
#ifdef DEBUG
	UnloadModel(inData->debugModel);
#endif
};

void on_update(entity_light_data* inData, entity* inEntity)
{
};

void on_draw_2d(entity_light_data* inData, entity* inEntity)
{

};

void on_draw_3d(entity_light_data* inData, entity* inEntity)
{
	if (inEntity->containingWorld->worldEditor->isInEditorMode)
	{
		DrawModel(inData->debugModel, Vector3Zero(), 1.0f, YELLOW);

		if (inEntity->containingWorld->worldEditor->currentlySelectedEntity->id == inEntity->id && inEntity->containingWorld->worldEditor->isInEditorMode)
		{
			DrawModelWires(inData->debugModel, Vector3Zero(), 1.0f, RED);
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

entity* editor_try_select(entity_light_data* inData, entity* inEntity)
{
	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEntity->containingWorld->currentlyRenderingCamera->data);
	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < inData->debugModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inData->debugModel.meshes[m], inData->debugModel.transform);
		if (meshHitInfo.hit)
		{
			return inEntity;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return nullptr;
		}
	}
}

#else

entt* editor_try_select(entt_maincube* inEntt)
{
	return nullptr;
}

#endif

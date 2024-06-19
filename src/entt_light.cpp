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
}

void update_light_props(entity_light_data* inData, entity* inEntity, int inType, Vector3 inPos, Vector3 inTarget, Color inColor)
{
};

void update_spatial_props(entity_light_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRot)
{

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

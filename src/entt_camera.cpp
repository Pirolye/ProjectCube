#include "world.h"

#include "raylib.h"

;
#include "raymath.h"

;
#define CAMERA_IMPLEMENTATION
#include "rcamera.h"

;
#include "rlgl.h"

#include "stdlib.h"
#include <typeinfo>
#include "graphene.h"
#include "assert.h"

/*

BIRO: Originally I wanted to have a multi cam system, where you could have as many cameras as you wanted. However, due to poor software design regarding rcamera.h, I was not
even close to accomplishing inEntt. Now we have 1 camera which switches properties when entering/exiting editor mode.

*/


void on_make(entity_camera_data* inData, entity* inEntity)
{

}

void on_destroy(entity_camera_data* inData, entity* inEntity)
{
};

void on_update(entity_camera_data* inData, entity* inEntity)
{

};

void on_draw_2d(entity_camera_data* inData, entity* inEntity)
{

};

void on_draw_3d(entity_camera_data* inData, entity* inEntity)
{

}

void set_camera_mode(entity* inEntity, int inMode, bool inIsForEditorOnly)
{
}

void editor_camera_update_model_rotation(entity_camera_data* inData, entity* inEntity)
{
}

void update_spatial_props(entity_camera_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{



}

void transform_camera_by_delta(entity_camera_data* inData, entity* inEntity)
{

}

/*
*
*
* EDITOR FUNCTION DEFINITIONS
*
*
*/
/*
#ifdef DEBUG

entity_pointer* editor_try_select(entt_camera* inEntt)
{
	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEntity->containingWorld->currentlyRenderingCamera->rayCam);

	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < inEntt->cameraEditorModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEntt->cameraEditorModel.meshes[m], inEntt->cameraEditorModel.transform);
		if (meshHitInfo.hit)
		{
			return inEntity->thisInArray;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return nullptr;
		}
	}

};

#else

entt* editor_try_select(entt_camera* inEntt)
{
	return nullptr;
};

#endif*/
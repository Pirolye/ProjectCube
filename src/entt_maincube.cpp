#include "entity.h"
#include "rlgl.h"
#include "raymath.h"
#include "assert.h"
#include "world.h"

#include <iostream>
#include <string>
#include <sstream>

;
void on_make(entity_maincube_data* inData, entity* inEntity)
{

}


void on_destroy(entity_maincube_data* inData, entity* inEntity)
{
}



void on_draw_3d(entity_maincube_data* inData, entity* inEntity)
{

}

void on_draw_2d(entity_maincube_data* inData, entity* inEntity)
{
	/*
	std::ostringstream getTheAddress;
	getTheAddress << &(inEntt->collisionBox->t.rot.x);
	std::string b = getTheAddress.str();

	std::ostringstream getTheAddress1;
	getTheAddress1 << &(inEntt->collisionBox->t.rot.y);
	std::string b1 = getTheAddress1.str();

	std::ostringstream getTheAddress2;
	getTheAddress2 << &(inEntt->collisionBox->t.rot.z);
	std::string b2 = getTheAddress2.str();


	std::string a = "Reading rot.x at " + b + " = " + std::to_string(inEntt->collisionBox->t.rot.x);
	std::string a1 = "Reading rot.y at " + b1 + " = " + std::to_string(inEntt->collisionBox->t.rot.y);
	std::string a2 = "Reading rot.z at " + b2 + " = " + std::to_string(inEntt->collisionBox->t.rot.z);
	*/
	/*
	DrawText(a.c_str(), 1000, 10  std::stoi(std::to_string(id[14])), 24, WHITE);
	DrawText(a1.c_str(), 1000, 30 * std::stoi(std::to_string(id[14])), 24, WHITE);
	DrawText(a2.c_str(), 1000, 50 * std::stoi(std::to_string(id[14])), 24, WHITE);
	*/

	/*
	DrawText(a.c_str(), 1000, 10, 24, WHITE);
	DrawText(a1.c_str(), 1000, 30, 24, WHITE);
	DrawText(a2.c_str(), 1000, 50, 24, WHITE);
	*/


}

void on_update(entity_maincube_data* inData, entity* inEntity)
{

}

void update_spatial_props(entity_maincube_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{

}

void update_spatial_props(entity_maincube_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale)
{

}


/*
*
*
* EDITOR FUNCTION DEFINITIONS
* 
* 
*/

#ifdef DEBUG

entity* editor_try_select(entity_maincube_data* inData, entity* inEntity)
{
	/*// Check ray collision against model meshes
	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < inEntt->cubeModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(inRay, inEntt->cubeModel.meshes[m], inEntt->cubeModel.inEntt->entityInfo.transform);
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
};

#else

entt* editor_try_select(entt_maincube* inEntt)
{
	return nullptr;
};

#endif
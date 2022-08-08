#ifndef _ENTT_H_
#define _ENTT_H_

#include "raylib.h"

typedef enum { CAMERA = 0, PLAYERCUBE, MAINCUBE, SMALLCUBE, LIGHT, COLLISIONBOX } enttType;

typedef struct 
{
	enttType entityType;
	const char* uniqueID;

	//camera
	Camera3D camera;

	//playerCube

	//mainCube
	Model mainCubeModel;
	Texture2D mainCube_albedo;
	Shader mainCube_shader;

	//smallCube
	//light
	//collisionBox
} entt;

#endif








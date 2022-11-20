#ifndef _H_GAME_INSTANCE_
#define _H_GAME_INSTANCE_

#include "raylib.h"
#include "raymath.h"
#include "PxPhysicsAPI.h"
#include <string>

;
#include "world.h"

static PxDefaultAllocator		gAllocator;
static PxDefaultErrorCallback	gErrorCallback;
static PxFoundation*            gFoundation;
static PxPhysics*               gPhysics;

;
struct game_instance
{
	std::string displayName;
	int windowWidth, windowHeight;
	//

	PxDefaultCpuDispatcher* gDispatcher = NULL;
	//PxScene*              gScene = NULL;
	PxPvd*                  gPvd = NULL;

	world* persistentWorld;

	float targetFPS;

	game_instance(std::string inDisplayName, int inWindowWidth, int inWindowHeight, float inTargetFPS);
	~game_instance() {};

	void update();
	void draw();
	void on_init();
	void on_exit();

	void load_world(std::string inWorldName);
};



#else

#endif

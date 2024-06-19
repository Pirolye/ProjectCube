#ifndef _WORLD_H_
#define _WORLD_H_

#include "entities.h"
#include "world_editor.h"
#include "world_editor_ui.h"


;
#include "PxPhysicsAPI.h"
#include "graphene.h"

#define MAX_ENTITIES_IN_WORLD 1024

using namespace physx;

struct game_instance;

struct world
{

	std::string name;

	entity* entityArray[MAX_ENTITIES_IN_WORLD] = { NULL };
	int entityArrayCurrentSize = 0;
	int totalMadeEntts = 0;

	// Physics
	PxScene* gScene;
	PxPhysics* globalPhysics;

	game_instance* gameInstance;

	//(Levente): Lighting is done inside the shaders of course, but lights are also entites in their own term. This means we have to manually update all shaders
	//to notify them of any changes that might happened in lighting data.
	Shader currentlyLoadedShaders[MAX_ENTITIES_IN_WORLD * 64] = { NULL }; //Note: We have to do NULL initialization here because the compiler won't do it when defining! (Even though it accepts it as valid syntax...)
	float defaultAmbientLightValue[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	//(Levente): Since the rcamera remake in raylib 4.5, we can have multiple cameras without issue!
	entity* currentlyRenderingCamera;

	world_editor* worldEditor;


};

void world_init(world* inWorld, game_instance* inGameInstance, PxPhysics* inPhysicsMemAddress); // Alias init
void world_deinit(world* inWorld);

//NOTE: Use reinterpret_cast to make it into the desired entt type!
entity* world_make_desired_entity(std::string inType, world* inWorld);

Shader world_make_shader(world* inWorld, const char* vertexShader, const char* fragmentShader);
void world_set_cam(world* inWorld, entity* inCam);

void world_update(world* inWorld);
void world_draw_all(world* inWorld);

//(Levente): Will be factored out in the future.
void world_run_script_on_init(world* inWorld);
void world_run_script_on_update(world* inWorld);
void world_run_script_on_destroy(world* inWorld);

#else

#endif

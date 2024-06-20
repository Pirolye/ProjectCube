#include "entt_camera.h"
#include "entt_maincube.h"

#include "world_editor.h"

#include "game_instance.h"

;
#include "assert.h"

#define FLT_MAX 340282346638528859811704183484516925440.0f 


void world_init(world* inWorld, game_instance* inGameInstance, PxPhysics* inPhysicsMemAddress)
{

	inWorld->gameInstance = inGameInstance;
	assert(inWorld->gameInstance != nullptr);

	inWorld->name = "debug";

	inWorld->globalPhysics = inPhysicsMemAddress; // (Levente): We do this because for some reason the can't read gPhysics static var properly. (Even though it can read gFoundation just fine...)

	PxSceneDesc sceneDesc(inWorld->globalPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	inWorld->gameInstance->gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = inWorld->gameInstance->gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	inWorld->gScene = inWorld->globalPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = inWorld->gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

#ifdef DEBUG
	inWorld->worldEditor = new world_editor;
	//inWorld->worldEditor = reinterpret_cast<world_editor*>(malloc(sizeof(world_editor))); //This never had a default constructor, yet the compiler can't handle the empty new directive now for some reason...
	init_world_editor(inWorld->worldEditor, inWorld);
#endif

	world_run_script_on_init(inWorld);

}

void world_deinit(world* inWorld) //(Levente): Technically this is really bad. We will need a proper shutdown procedure!
{
	#if DEBUG
		shutdown_world_editor(inWorld->worldEditor);
	#endif

	world_run_script_on_destroy(inWorld);

	//@TODO: Figure this out! 

	/*
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] != NULL)
		{
			inWorld->entityArray[i]->on_destroy();
		}
	}
	*/

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] != NULL)
		{
			delete inWorld->entityArray[i];
		}
	}

	/*
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != nullptr)
		{
			entityArray[i]->on_destroy();
			delete entityArray[i];
		}
		else
		{
			continue;
		}
	}
	*/

}

Shader world_make_shader(world* inWorld, const char* vertexShader, const char* fragmentShader)
{
	Shader temp = LoadShader(vertexShader, fragmentShader);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		if (inWorld->currentlyLoadedShaders[i].id == 0 && inWorld->currentlyLoadedShaders[i].locs == 0)
		{
			inWorld->currentlyLoadedShaders[i] = temp;
			break;
		}
	}

	return temp;
}


void world_update(world* inWorld)
{

	if (GetFrameTime() > 0)
	{

		if (!inWorld->worldEditor->isInEditorMode)
		{
			inWorld->gScene->simulate(1.0f / inWorld->gameInstance->targetFPS);
			inWorld->gScene->fetchResults(true);

		}


		
	}


	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		entity* entity = inWorld->entityArray[i];

		if (entity != NULL)
		{
			
			on_update(entity);
		}
	}

	if (IsKeyPressed(KEY_F1))
	{
		#ifdef DEBUG 
			if (inWorld->worldEditor->isInEditorMode) exit_editor_mode(inWorld->worldEditor);
			else enter_editor_mode(inWorld->worldEditor);
		#endif
	}

	if(inWorld->worldEditor->isInEditorMode) update_world_editor(inWorld->worldEditor);

	world_run_script_on_update(inWorld);


}

void world_draw_all(world* inWorld)
{
	BeginDrawing();

	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inWorld->currentlyRenderingCamera->data);
	
	BeginMode3D(camera->rayCam);
	
	ClearBackground(BLACK);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		entity* entity = inWorld->entityArray[i];

		if (entity != NULL)
		{
			on_draw_3d(entity);
		}
	}

	if (inWorld->worldEditor->isInEditorMode) draw_world_editor_3d(inWorld->worldEditor);

	EndMode3D();

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		entity* entity = inWorld->entityArray[i];

		if (entity != NULL)
		{

			on_draw_2d(entity);
		}
	}

	if (inWorld->worldEditor->isInEditorMode) draw_world_editor_2d(inWorld->worldEditor);

	EndDrawing();

}

void engine_add_model_to_visibility_array(world* inWorld, model* inModel)
{
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->worldEditor->visibilityArray[i] == NULL)
		{
			inWorld->worldEditor->visibilityArray[i] = inModel;
			break;
		}
	}
}
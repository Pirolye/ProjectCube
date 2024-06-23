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

	entity_camera_data* camera = (entity_camera_data*)(inWorld->currentlyRenderingCamera->data);
	
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

entity* world_make_desired_entity(std::string inType, world* inWorld)
{
	entity* newEntity = new entity;

	newEntity->type = inType;

	//inType.erase(0, 12);

	std::string id = "entity_" + inType + std::to_string(inWorld->totalMadeEntts);
	newEntity->containingWorld = inWorld;
	newEntity->id = id;

	inWorld->totalMadeEntts = inWorld->totalMadeEntts + 1;

	inWorld->entityArrayCurrentSize = inWorld->entityArrayCurrentSize + 1;


	bool isThereAnotherCameraInTheWorld = false;
	if (inType == "camera")
	{
		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (inWorld->entityArray[i] != NULL)
			{
				if (inWorld->entityArray[i]->type == "camera")
				{
					isThereAnotherCameraInTheWorld = true;
					break;
				}
				else
				{
					isThereAnotherCameraInTheWorld = false;
					continue;
				}
			}
		}

		if (isThereAnotherCameraInTheWorld == false) inWorld->currentlyRenderingCamera = newEntity;

	}

	on_make(newEntity);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] == NULL)
		{
			inWorld->entityArray[i] = newEntity;
			break;
		}
	}

	for (int i = 0; i < MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] && inWorld->entityArray[i]->type == "light")
		{
			update_light_locations(inWorld->entityArray[i]);
		}
	}

	return newEntity;
}

entity* world_make_desired_entity_runtime(std::string inType, world* inWorld)
{
	entity_camera_data* camera = (entity_camera_data*)(inWorld->currentlyRenderingCamera->data);
	
	entity* newEntity = new entity;

	newEntity->type = inType;

	std::string id = "entity_" + inType + std::to_string(inWorld->totalMadeEntts);
	newEntity->containingWorld = inWorld;
	newEntity->id = id;

	inWorld->totalMadeEntts = inWorld->totalMadeEntts + 1;

	inWorld->entityArrayCurrentSize = inWorld->entityArrayCurrentSize + 1;

	on_make(newEntity);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] == NULL)
		{
			inWorld->entityArray[i] = newEntity;
			break;
		}
	}

	RayCollision meshHitInfo = { 0 };
	meshHitInfo.distance = FLT_MAX;
	meshHitInfo.hit = false;
	Ray mouseRay = GetMouseRay(GetMousePosition(), camera->rayCam);
	
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->worldEditor->visibilityArray[i] != NULL)
		{
			for (int m = 0; m < inWorld->worldEditor->visibilityArray[i]->model.meshCount; m++)
			{
				meshHitInfo = GetRayCollisionMesh(mouseRay, inWorld->worldEditor->visibilityArray[i]->model.meshes[m], inWorld->worldEditor->visibilityArray[i]->model.transform);

				if (meshHitInfo.hit)
				{
					if (inWorld->worldEditor->visibilityArray[i]->container == newEntity) break;
					{
						update_spatial_properties(newEntity, meshHitInfo.point, Vector3{ 1.0f, 1.0f, 1.0f });
						break;
					}
				}
			}


		}
	}

	for (int i = 0; i < MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] && inWorld->entityArray[i]->type == "light")
		{
			update_light_locations(inWorld->entityArray[i]);
		}
	}

	return newEntity;
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
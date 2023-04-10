#include "entt_camera.h"
#include "entt_maincube.h"

#include "game_instance.h"

;
#include "assert.h"

#define FLT_MAX 340282346638528859811704183484516925440.0f 


world::world(game_instance* inGameInstance, PxPhysics* inPhysicsMemAddress) 
{

	
	
	/*
	//(Levente): Okay... this is clever but not very logical. Apparently 0xcdcdcd... is not a nullptr so we individually assign NULL to every uninitialized entt in the array!
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		entityArray[i] = NULL;
	}

	//(Levente): Same for the shaders.
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		currentlyLoadedShaders[i] = { 0 };
	}
	*/

	gameInstance = inGameInstance;
	assert(gameInstance != nullptr);

	name = "debug";

	globalPhysics = inPhysicsMemAddress; // (Levente): We do this because for some reason the program
										 // can't read gPhysics static var properly. (Even though it can read
										 // gFoundation just fine...)

	PxSceneDesc sceneDesc(globalPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gameInstance->gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gameInstance->gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = globalPhysics->createScene(sceneDesc);

	//physicsSpace = new q3Scene(1.0 / 60);
	//physicsSpace->SetGravity(q3Vec3(0.0, -1.0, 0.0));

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

#ifdef DEBUG
	init_world_editor();
#endif

	run_script_on_init();

}

world::~world() //(Levente): Technically this is really bad. We will need a proper shutdown procedure!
{
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

Shader world::make_shader(const char* vertexShader, const char* fragmentShader)
{
	Shader temp = LoadShader(vertexShader, fragmentShader);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		if (currentlyLoadedShaders[i].id == 0 && currentlyLoadedShaders[i].locs == 0)
		{
			currentlyLoadedShaders[i] = temp;
			break;
		}
	}

	return temp;
}


void world::update()
{

	if (GetFrameTime() > 0)
	{
		
		if (!worldEditor.isInEditorMode) 
		{
			gScene->simulate(1.0f / gameInstance->targetFPS);
			gScene->fetchResults(true);
		}
		
		//(Levente): Traditionally you don't want physics in the editor...
		//gScene->simulate(1.0f / gameInstance->targetFPS);
		//gScene->fetchResults(true);
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_update();
		}
	}

	if (IsKeyPressed(KEY_F1))
	{
		#ifdef DEBUG 
			if (worldEditor.isInEditorMode) exit_editor_mode();
			else enter_editor_mode();
		#endif
	}

	if(worldEditor.isInEditorMode) update_world_editor();

	run_script_on_update();


}

void world::draw_all()
{
	BeginDrawing();

	BeginMode3D( currentlyRenderingCamera->rayCam);
	
	ClearBackground(BLACK);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_3d();
		}
	}

	if (worldEditor.isInEditorMode) draw_world_editor_3d();

	EndMode3D();

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_2d();
		}
	}

	if (worldEditor.isInEditorMode) draw_world_editor_2d();

	EndDrawing();

}

void world::on_destroy()
{
#if DEBUG
	shutdown_world_editor();
#endif
	
	run_script_on_destroy();
	
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_destroy();
		}
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			delete entityArray[i];
		}
	}
	
}

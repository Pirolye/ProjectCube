#include "entt_camera.h"
#include "entt_maincube.h"

#include "game_instance.h"

;
#include "assert.h"

#define FLT_MAX 340282346638528859811704183484516925440.0f 


world::world(game_instance* inGameInstance, PxPhysics* inPhysicsMemAddress) 
{
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
	
	gameInstance = inGameInstance;
	assert(gameInstance != nullptr);

	name = "debug";
	gameCameraPosition = Vector3{ 10.0f, 0.0f, 0.0f };

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
#else
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

entt* world::make_desired_entt(entts inDesiredEntt)
{
	switch (inDesiredEntt)
	{
		case entts::cam:
		{
			bool isThereAnotherCameraInTheWorld = false;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] != NULL)
				{
					isThereAnotherCameraInTheWorld = dynamic_cast<entt_camera*>(entityArray[i]);
					break;
				}
			}

			assert(isThereAnotherCameraInTheWorld != true);
			
			entt_camera* cam = new entt_camera;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			cam->id = "entt_camera_" + std::to_string(totalMadeEntts);
			cam->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = cam;
					break;
				}
			}

			cam->on_make();

			return cam;

			break;
		}

		case entts::mainCube:
		{
			entt_maincube* c = new entt_maincube;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			c->id = "entt_maincube_" + std::to_string(totalMadeEntts);
			c->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = c;
					break;
				}
			}

			c->on_make(); 
			
			return c;

			break;
		}
		
		case entts::mainCube_Static:
		{
			entt_maincube_static* c = new entt_maincube_static;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			c->id = "entt_maincube_static_" + std::to_string(totalMadeEntts);
			c->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = c;
					break;
				}
			}

			c->on_make();

			return c;

			break;
		}
		case entts::light:
		{
			entt_light* l1 = new entt_light;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			l1->id = "entt_point_light_" + std::to_string(totalMadeEntts);
			l1->containingWorld = this;

			l1->on_make();

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = l1;
					break;
				}
			}

			return l1;

			break;
		}


		default:
		{
			return nullptr;
		}

	}
}

void world::update()
{
	assert(dynamic_cast<entt_camera*>(entityArray[0]) != nullptr && "The/A camera should always exist at entity array index 0!");
	if(worldEditor.isInEditorMode && canMoveCamera) UpdateCamera(dynamic_cast<entt_camera*>(entityArray[0])->rayCam, CAMERA_FIRST_PERSON);

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
		#else
		
		#endif
	}

	if(worldEditor.isInEditorMode) update_world_editor();

	run_script_on_update();


}

void world::draw_all()
{
	BeginDrawing();

	BeginMode3D(*(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
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

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
	gameCameraPosition = Vector3{ -5.0f, 0.0f, 0.0f };

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
	assert(dynamic_cast<entt_camera*>(entityArray[0]) != nullptr && "The/a camera should always exist at entity array index 0!");
	UpdateCamera(dynamic_cast<entt_camera*>(entityArray[0])->rayCam);

	cameraSwitchedLastFrame = false;
	if (GetFrameTime() > 0)
	{
		/*
		if (!isInEditorMode) 
		{
			gScene->simulate(1.0f / 60.0f);
			gScene->fetchResults(true);
		}
		*/

		gScene->simulate(1.0f / 60.0f);
		gScene->fetchResults(true);
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
		if (isInEditorMode) exit_editor_mode();
		else enter_editor_mode();
		#else
		
		#endif
	}

	if(isInEditorMode) update_world_editor();

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
	EndMode3D();

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_2d();
		}
	}

	if (isInEditorMode)
	{
		switch (currentlyEditingAxis)
		{
		case 0:
			DrawText("x", 0, 100, 30, WHITE);
			break;
		case 1:
			DrawText("y", 0, 100, 30, WHITE);
			break;
		case 2:
			DrawText("z", 0, 100, 30, WHITE);
			break;
		}
	}

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

/*
* 
* 
* EDITOR FUNCTION DEFINITIONS
* 
* 
*/

#ifdef DEBUG

void world::enter_editor_mode()
{
	isInEditorMode = true;

	entt_camera* cam = dynamic_cast<entt_camera*>(entityArray[0]); //(Levente): Camera is always at index 0!

	cam->rayCam->position = Vector3{0.0f, 0.0f, 0.0f}; // Camera position
	cam->rayCam->target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	cam->rayCam->up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	cam->rayCam->fovy = 70.0f;                                // Camera field-of-view 
	cam->rayCam->projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode(*(cam->rayCam), CAMERA_FREE); // Set a free camera mode

}

void world::exit_editor_mode()
{
	isInEditorMode = false;

	entt_camera* cam = dynamic_cast<entt_camera*>(entityArray[0]); //(Levente): Camera is always at index 0!

	cam->rayCam->position = gameCameraPosition; // Camera position
	cam->rayCam->target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	cam->rayCam->up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	cam->rayCam->fovy = 70.0f;                                // Camera field-of-view 
	cam->rayCam->projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode(*(cam->rayCam), CAMERA_CUSTOM); // Set a free camera mode

	currentlySelectedEntt = nullptr;

	// START WORLD AGAIN FROM THE BEGINNING
}

void world::update_world_editor()
{

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) editor_try_select_entt();

	if (IsKeyPressed(KEY_TAB))
	{
		currentlyEditingAxis = currentlyEditingAxis + 1;
		if (currentlyEditingAxis == 3) currentlyEditingAxis = 0;
	}

	//Will be removed once we got editor gui
	if (currentlySelectedEntt != NULL)
	{
		DrawText(currentlySelectedEntt->id.c_str(), 10, 550, 20, RED);

		if (IsKeyDown(KEY_W))
		{
			editor_move_entt(currentlyEditingAxis, 0.05f);
		}
		if (IsKeyDown(KEY_S))
		{
			editor_move_entt(currentlyEditingAxis, -0.05f);
		}
		if (IsKeyDown(KEY_A))
		{
			editor_rotate_entt(currentlyEditingAxis, 1.0f);
		}
		if (IsKeyDown(KEY_D))
		{
			editor_rotate_entt(currentlyEditingAxis, -1.0f);
		}


	}

}

void world::editor_move_entt(int axis, float val)
{
	if (currentlySelectedEntt == nullptr) return;

	if (axis == 0)
	{
		entt_transform t = currentlySelectedEntt->enttTransform;
		currentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x + val, t.pos.y, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 1)
	{
		entt_transform t = currentlySelectedEntt->enttTransform;
		currentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y + val, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 2)
	{
		entt_transform t = currentlySelectedEntt->enttTransform;
		currentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y, t.pos.z + val }, t.scale, t.rot);

	}

}

void world::editor_rotate_entt(int axis, float val)
{
	if (currentlySelectedEntt == nullptr) return;

	if (axis == 0)
	{
		entt_transform t = currentlySelectedEntt->enttTransform;

		currentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x + val, t.rot.y, t.rot.z });

	}
	if (axis == 1)
	{
		entt_transform t = currentlySelectedEntt->enttTransform;

		currentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x, t.rot.y + val, t.rot.z });

	}
	if (axis == 2)
	{
		entt_transform t = currentlySelectedEntt->enttTransform;

		currentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x, t.rot.y, t.rot.z + val });

	}

}



void world::editor_try_select_entt()
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_light*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_light*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_maincube*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			//if (dynamic_cast<entt_maincube_static*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_maincube_static*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_camera*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);

			if (currentlySelectedEntt != nullptr) break;
			else continue;
		}
	}
};



#else

void world::editor_move_entt(int axis, float val) {};
void world::enter_editor_mode() {};
void world::exit_editor_mode() {};
void world::update_world_editor() {};
void world::editor_try_select_entt() {};


#endif
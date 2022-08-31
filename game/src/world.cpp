#include "world.h"
#include "entt_camera.h"
#include "entt_maincube.h"

;
#include "assert.h"

#define FLT_MAX 340282346638528859811704183484516925440.0f 


world::world() 
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
	
	name = "debug";

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

		case entts::camSetCurrentlyRendering:
		{
			entt_camera* camS = new entt_camera;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			camS->id = "entt_camera_" + std::to_string(totalMadeEntts);
			camS->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = camS;
					break;
				}
			}

			currentlyRenderingCam = camS;
			camS->currentlyDrawing = true;

			camS->on_make();

			return camS;

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
	assert(currentlyRenderingCam != nullptr);

	UpdateCamera((currentlyRenderingCam->rayCam));

	cameraSwitchedLastFrame = false;

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
		isInEditorMode = true;
		#else
		isInEditorMode = false;
		#endif
	}

	if(isInEditorMode) update_world_editor();

	run_script_on_update();

}

void world::draw_all()
{
	assert(currentlyRenderingCam != nullptr);

	BeginDrawing();

	BeginMode3D(*(currentlyRenderingCam->rayCam));
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

void world::update_world_editor()
{

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) editor_try_select_entt();

	//Will be removed once we got editor gui
	if (currentlySelectedEntt != nullptr) DrawText(currentlySelectedEntt->id.c_str(), 10, 550, 20, RED);

	if (IsKeyPressed(KEY_TAB))
	{

		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (entityArray[i] != NULL)
			{
				if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr)
				{
					entt_camera* foundCam = dynamic_cast<entt_camera*>(entityArray[i]);

					if (foundCam != currentlyRenderingCam)
					{
						currentlyRenderingCam->currentlyDrawing = false;
						foundCam->currentlyDrawing = false;

						entt_camera* prev = currentlyRenderingCam;

						currentlyRenderingCam = foundCam;
						currentlyRenderingCam->currentlyDrawing = true;

						prev->currentlyDrawing = false;

						cameraSwitchedLastFrame = true;

						break;
					}


				}
			}
		}

	}


}

void world::editor_try_select_entt()
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	cursorSelectionRay = GetMouseRay(GetMousePosition(), *(currentlyRenderingCam->rayCam));

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_light*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_light*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_maincube*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr) currentlySelectedEntt = dynamic_cast<entt_camera*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			
			if (currentlySelectedEntt != nullptr) break;
			else continue;
		}
	}
};



#else

void world::update_world_editor() {};
void world::editor_try_select_entt() {};


#endif
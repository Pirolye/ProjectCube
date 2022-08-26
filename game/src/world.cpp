#include "world.h"
#include "entt_camera.h"

;
#include "assert.h"

world::world() //(Levente): Okay... this is clever but not very logical. Apparently 0xcdcdcd... is not a nullptr so we individually assign NULL to every
			   // uninitialized entt in the array!
{
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		entityArray[i] = NULL;
	}


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

void world::make_desired_entt(entts inDesiredEntt, Vector3 inStartingPosition)
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

		cam->on_make();

		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (entityArray[i] == NULL)
			{
				entityArray[i] = cam;
				break;
			}
		}

		break;
	}

	case entts::camSetCurrentlyRendering:
	{
		entt_camera* camS = new entt_camera;

		totalMadeEntts = totalMadeEntts + 1;
		entityArrayCurrentSize = entityArrayCurrentSize + 1;

		camS->id = "entt_camera_" + std::to_string(totalMadeEntts);
		camS->containingWorld = this;

		camS->on_make();

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

		break;
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

void world::draw_all()
{
	assert(currentlyRenderingCam != nullptr);

	BeginDrawing();

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_2d();
		}
	}

	BeginMode3D(*(currentlyRenderingCam->rayCam));

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_3d();
		}
	}

	EndMode3D();

	EndDrawing();
}

void world::on_destroy()
{
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

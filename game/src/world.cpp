#include "world.h"
#include "entt_camera.h"

void world::make_desired_entity(entts inDesiedEntt, Vector3 inStartingPosition)
{
	switch(inDesiedEntt)
	{
	case cameraDefConfig:
		entt_camera* currentCam;

		entityArray[entityArray.back() + 1] = currentCam;

		currentCam->on_make();
	}
}

void world::update()
{
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != nullptr)
		{
			entityArray[i]->update();
		}
	}
}

void world::draw()
{
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != nullptr)
		{
			entityArray[i]->draw();
		}
	}
}
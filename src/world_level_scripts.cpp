#include "world.h"

;
#include "raymath.h"

#include <typeinfo>
;

void world_run_script_on_init(world* inWorld)
{
	if (inWorld->name == "debug")
	{
		
		entity* mainCubeStatic1 = world_make_desired_entity("maincube", inWorld);
		//entt* mainCube2 = world_make_desired_entt<entt_maincube_static>(inWorld);
		

		//update_spatial_props(mainCubeStatic1, Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		//static_cast<entt_maincube_static*>(mainCube2)->update_spatial_props(Vector3{ 10.0f, -1.0f, 10.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		
		/*
		entt* mainCube3 = world_make_desired_entt<entt_maincube_static>(inWorld);
		entt* mainCube4 = world_make_desired_entt<entt_maincube_static>(inWorld);
		entt* mainCube5 = world_make_desired_entt<entt_maincube_static>(inWorld);
		entt* mainCube6 = world_make_desired_entt<entt_maincube_static>(inWorld);
		entt* mainCube7 = world_make_desired_entt<entt_maincube_static>(inWorld);
		entt* mainCube8 = world_make_desired_entt<entt_maincube_static>(inWorld);
		static_cast<entt_maincube_static*>(mainCube3)->update_spatial_props(Vector3{ -15.0f, -5.0f, -5.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		static_cast<entt_maincube_static*>(mainCube4)->update_spatial_props(Vector3{ 0.0f, 10.0f, 5.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		static_cast<entt_maincube_static*>(mainCube5)->update_spatial_props(Vector3{ -35.0f, -30.0f, -10.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		static_cast<entt_maincube_static*>(mainCube6)->update_spatial_props(Vector3{ 25.0f, 25.0f, -15.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		static_cast<entt_maincube_static*>(mainCube7)->update_spatial_props(Vector3{ -20.0f, -5.0f, -40.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		static_cast<entt_maincube_static*>(mainCube8)->update_spatial_props(Vector3{ 15.0f, -30.0f, -20.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

		*/
		
		entity* gameplayCam = world_make_desired_entity("camera", inWorld);
		set_camera_mode(gameplayCam, 1, true);

		entity* secondaryLight = world_make_desired_entity("light", inWorld);  // ALWAYS MAKE LIGHTS LAST BECAUSE OTHERWISE THEY WON'T WORK!!!

		update_light_props(reinterpret_cast<entity_light_data*>(secondaryLight->data), secondaryLight, 1, Vector3{7.0f, 7.0f, 7.0f}, Vector3Zero(), WHITE);

		/*
		entt* gameplayCam1 = world_make_desired_entt<entt_camera>(inWorld);
		entt* gameplayCam2 = world_make_desired_entt<entt_camera>(inWorld);
		static_cast<entt_camera*>(gameplayCam)->set_mode(1, true);
		static_cast<entt_camera*>(gameplayCam1)->set_mode(1, false);
		static_cast<entt_camera*>(gameplayCam2)->set_mode(1, true);

		static_cast<entt_light*>(secondaryLight)->update_light_props(1, Vector3{ 7.0f, 7.0f, 7.0f }, Vector3Zero(), WHITE);

		entt* mainCube = world_make_desired_entt<entt_maincube>(inWorld);
		entt* editorCam = world_make_desired_entt<entt_camera>(inWorld);
		entt* mainLight = world_make_desired_entt<entt_light>(inWorld);
		static_cast<entt_camera*>(editorCam)->set_mode(1, true);

		static_cast<entt_light*>(mainLight)->update_light_props(1, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero(), WHITE);
		static_cast<entt_maincube*>(mainCube)->update_spatial_props(Vector3{ 0.0f, 10.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		*/


		return;
	}
	
	else if (inWorld->name == "campaign_1_1")
	{
		return;
	}

	else
	{
		return;
	}

};

void world_run_script_on_update(world* inWorld)
{
	if (inWorld->name == "debug")
	{

	}

};

void world_run_script_on_destroy(world *inWorld)
{
	if (inWorld->name == "debug")
	{



		return;
	}


};

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

	if (inType == "maincube")
	{
		entity_maincube_data* mainCubeData = new entity_maincube_data;
		on_make(mainCubeData, newEntity);
		newEntity->data = mainCubeData;
	}
	if (inType == "camera")
	{
		entity_camera_data* mainCameraData = new entity_camera_data;
		on_make(mainCameraData, newEntity);
		newEntity->data = mainCameraData;
	}
	if (inType == "light")
	{
		entity_light_data* mainLightData = new entity_light_data;
		on_make(mainLightData, newEntity);
		newEntity->data = mainLightData;
	}


	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] == NULL)
		{
			inWorld->entityArray[i] = newEntity;
			break;
		}
	}

	return newEntity;
}

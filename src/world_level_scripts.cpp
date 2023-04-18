#include "world.h"

;
#include "raymath.h"

#include <typeinfo>
;

void world_run_script_on_init(world* inWorld)
{
	if (inWorld->name == "debug")
	{
		
		entt_maincube* mainCube1 = reinterpret_cast<entt_maincube*>(world_make_desired_entt<entt_maincube>(inWorld));
		//entt* mainCube2 = world_make_desired_entt<entt_maincube_static>(inWorld);
		
		//entt* secondaryLight = world_make_desired_entt<entt_light>(inWorld);  // ALWAYS MAKE LIGHTS LAST BECAUSE OTHERWISE THEY WON'T WORK!!!

		update_spatial_props(mainCube1, Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
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
		
		entt_camera* gameplayCam = reinterpret_cast<entt_camera*>(world_make_desired_entt<entt_camera>(inWorld));
		set_mode(gameplayCam, 1, true);


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

template <typename t> void* world_make_desired_entt(world* inWorld)
{
	t* newEntt = new t;

	inWorld->totalMadeEntts = inWorld->totalMadeEntts + 1;
	inWorld->entityArrayCurrentSize = inWorld->entityArrayCurrentSize + 1;

	std::string typeName = typeid(t).name();
	
	typeName.erase(0, 12);

	newEntt->id = "entt_" + typeName + std::to_string(inWorld->totalMadeEntts);
	newEntt->containingWorld = inWorld;

	on_make(newEntt);

	bool isThereAnotherCameraInTheWorld = false;
	if (typeid(t) == typeid(entt_camera))
	{
		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (inWorld->entityArray[i] != NULL)
			{
				isThereAnotherCameraInTheWorld = (reinterpret_cast<entt_camera*>(inWorld->entityArray[i]) != NULL);
				if (isThereAnotherCameraInTheWorld) break;
				else continue;
			}
		}

		if (isThereAnotherCameraInTheWorld == false) inWorld->currentlyRenderingCamera = reinterpret_cast<entt_camera*>(newEntt);

	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inWorld->entityArray[i] == NULL)
		{
			inWorld->entityArray[i] = newEntt;
			break;
		}
	}

	return newEntt;
}

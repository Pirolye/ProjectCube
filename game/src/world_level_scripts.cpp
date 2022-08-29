#include "world.h"

;
#include "raymath.h"

;
void world::run_script_on_init()
{
	if (name == "debug")
	{
		
		entt* mainCam = this->make_desired_entt(entts::camSetCurrentlyRendering);
		entt* Cam1 = this->make_desired_entt(entts::cam);
		entt* mainCube = this->make_desired_entt(entts::mainCube);
		entt* mainLight = this->make_desired_entt(entts::light);
		entt* secondaryLight = this->make_desired_entt(entts::light);

		entt* mainCube2 = this->make_desired_entt(entts::mainCube);

		dynamic_cast<entt_light*>(secondaryLight)->update_light_props(1, Vector3{ 7.0f, 7.0f, 7.0f }, Vector3Zero(), WHITE);
		dynamic_cast<entt_light*>(mainLight)->update_light_props(1, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero(), WHITE);
		dynamic_cast<entt_maincube*>(mainCube2)->pos = Vector3{ 3.0f, 3.0f, 3.0f };

		
		return;
	}
	
	else if (name == "campaign_1_1")
	{
		return;
	}

	else
	{
		return;
	}

};

void world::run_script_on_update()
{
	if (name == "debug")
	{
		return;
	}

};

void world::run_script_on_destroy()
{
	if (name == "debug")
	{



		return;
	}


};
#include "world.h"

;
#include "raymath.h"

;
void world::run_script_on_init()
{
	if (name == "debug")
	{
		
		entt* gameplayCam = this->make_desired_entt(entts::cam);
		entt* gameplayCam1 = this->make_desired_entt(entts::cam);
		//		entt* gameplayCam2 = this->make_desired_entt(entts::cam);
		entt* mainCube = this->make_desired_entt(entts::mainCube);
		entt* mainCube1 = this->make_desired_entt(entts::mainCube);
		entt* mainCube2 = this->make_desired_entt(entts::mainCube_Static);
		
		entt* mainLight = this->make_desired_entt(entts::light);
		entt* secondaryLight = this->make_desired_entt(entts::light);  // ALWAYS MAKE LIGHTS LAST BECAUSE OTHERWISE THEY WON'T WORK!!!

		dynamic_cast<entt_maincube*>(mainCube)->update_spatial_props(Vector3{ 0.0f, 10.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		dynamic_cast<entt_maincube*>(mainCube1)->update_spatial_props(Vector3{ 5.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		dynamic_cast<entt_maincube_static*>(mainCube2)->update_spatial_props(Vector3{ 10.0f, -1.0f, 10.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		
		entt* mainCube3 = this->make_desired_entt(entts::mainCube_Static);
		entt* mainCube4 = this->make_desired_entt(entts::mainCube_Static);
		entt* mainCube5 = this->make_desired_entt(entts::mainCube_Static);
		entt* mainCube6 = this->make_desired_entt(entts::mainCube_Static);
		entt* mainCube7 = this->make_desired_entt(entts::mainCube_Static);
		entt* mainCube8 = this->make_desired_entt(entts::mainCube_Static);
		dynamic_cast<entt_maincube_static*>(mainCube3)->update_spatial_props(Vector3{ -15.0f, -5.0f, -5.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		dynamic_cast<entt_maincube_static*>(mainCube4)->update_spatial_props(Vector3{ 0.0f, 10.0f, 5.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		dynamic_cast<entt_maincube_static*>(mainCube5)->update_spatial_props(Vector3{ -35.0f, -30.0f, -10.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		dynamic_cast<entt_maincube_static*>(mainCube6)->update_spatial_props(Vector3{ 25.0f, 25.0f, -15.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		dynamic_cast<entt_maincube_static*>(mainCube7)->update_spatial_props(Vector3{ -20.0f, -5.0f, -40.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		dynamic_cast<entt_maincube_static*>(mainCube8)->update_spatial_props(Vector3{ 15.0f, -30.0f, -20.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

		entt* editorCam = this->make_desired_entt(entts::cam);

		dynamic_cast<entt_camera*>(editorCam)->set_mode(0);
		dynamic_cast<entt_camera*>(gameplayCam)->set_mode(1);
		dynamic_cast<entt_camera*>(gameplayCam1)->set_mode(0);
		//dynamic_cast<entt_camera*>(gameplayCam2)->set_mode(1);

		dynamic_cast<entt_light*>(secondaryLight)->update_light_props(1, Vector3{ 7.0f, 7.0f, 7.0f }, Vector3Zero(), WHITE);
		dynamic_cast<entt_light*>(mainLight)->update_light_props(1, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero(), WHITE);
		

		
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

	}

};

void world::run_script_on_destroy()
{
	if (name == "debug")
	{



		return;
	}


};
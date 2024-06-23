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
		
		entity* gameplayCam = world_make_desired_entity("camera", inWorld);
		set_camera_mode(gameplayCam, 1, true);

		entity* secondaryLight = world_make_desired_entity("light", inWorld);  // ALWAYS MAKE LIGHTS LAST BECAUSE OTHERWISE THEY WON'T WORK!!!

		update_light_properties(secondaryLight, 1, Vector3{7.0f, 7.0f, 7.0f}, Vector3Zero(), WHITE);

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


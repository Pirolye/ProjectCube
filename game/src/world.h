#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"
#include "entt_camera.h"
#include "entt_maincube.h"

;
#include "entt_light.h"

;
#include "ode/ode.h"

struct collision_data
{
	dWorldID world;
	dJointGroupID contactGroup;
};

;
//(Levente): The world houses the main entity array plus standard configuration you would want. Loading worlds is taken care of by the game instance.
struct world
{
	world(); // Alias init
	virtual ~world(); // Separate from on_destroy()

	std::string name;

	entt* entityArray[MAX_ENTITIES_IN_WORLD];
	int entityArrayCurrentSize = 0;
	int totalMadeEntts = 0;

	// Physics
	float gravity_y = 1.0;
	dWorldID physicsWorld;
	dSpaceID physicsSpace;
	dJointGroupID collisionContactGroup;
	collision_data collisionData;
	dGeomID groundGeom;


	float defaultAmbientLightValue[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	//(Levente): Lighting is done inside the shaders of course, but lights are also entites in their own term. This means we have to manually update all shaders
	//to notify them of any changes that might happened in lighting data.
	Shader currentlyLoadedShaders[MAX_ENTITIES_IN_WORLD * 2];

	//(Levente): Each world can have as many cams as we like. The cam that we would like rendering will be set by setting this pointer.
	entt_camera* currentlyRenderingCam;

	//(Levente): The make functions make the data structures and also do whatever is neccessary at that point in gameplay. Usually registering to arrays.
	entt* make_desired_entt(entts inDesiredEntity);
	Shader make_shader(const char* vertexShader, const char* fragmentShader);

	bool cameraSwitchedLastFrame = false;

	// Editor functions
	entt* currentlySelectedEntt = nullptr;
	Ray cursorSelectionRay = { 0 };
	void editor_try_select_entt();
	void update_world_editor();
	bool isInEditorMode = false;

	void update();
	void draw_all();
	void on_destroy();

	//(Levente): Will be factored out in the future.
	void run_script_on_init();
	void run_script_on_update();
	void run_script_on_destroy();
};

void nearCallback(void* data, dGeomID o1, dGeomID o2);


#else

#endif

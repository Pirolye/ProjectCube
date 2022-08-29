#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"
#include "entt_camera.h"
#include "entt_maincube.h"
;
#include "entt_light.h"

;
struct world
{
	world(); // Alias init
	virtual ~world(); // Separate from on_destroy()

	std::string name;

	entt* entityArray[MAX_ENTITIES_IN_WORLD];
	int entityArrayCurrentSize = 0;
	int totalMadeEntts = 0;

	float defaultAmbientLightValue[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	Shader currentlyLoadedShaders[MAX_ENTITIES_IN_WORLD * 2];

	//(Levente): Each world can have as many cams as we like. The cam that we would like rendering will be set by setting this pointer.
	entt_camera* currentlyRenderingCam;

	entt* make_desired_entt(entts inDesiredEntity);
	Shader make_shader(const char* vertexShader, const char* fragmentShader);

	bool cameraSwitchedLastFrame = false;

	// Editor functions
	entt* currentlySelectedEntt = nullptr;
	Ray cursorSelectionRay = { 0 };
	void try_select_entt();

	void update();
	void draw_all();
	void on_destroy();
	
	void run_script_on_init();
	void run_script_on_update();
	void run_script_on_destroy();
}

#else

#endif

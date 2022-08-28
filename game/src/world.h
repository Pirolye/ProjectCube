#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"
#include "entt_camera.h"
;
#include "entt_light.h"

;
struct world
{
	world();
	virtual ~world();

	entt* entityArray[MAX_ENTITIES_IN_WORLD];
	int entityArrayCurrentSize = 0;
	int totalMadeEntts = 0;

	float defaultAmbientLightValue[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	Shader currentlyLoadedShaders[MAX_ENTITIES_IN_WORLD * 2];

	entt_camera *currentlyRenderingCam; //(Levente): Each world can have as many cams as we like. The cam that we would like rendering from 
									    // will be set by setting this pointer.

	entt* make_desired_entt(entts inDesiredEntity);
	Shader make_shader(const char* vertexShader, const char* fragmentShader);

	bool cameraSwitchedLastFrame = false;

	void update();
	void draw_all();
	void on_destroy();
}

#else

#endif

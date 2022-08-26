#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"
#include "entt_camera.h"

;
struct world
{
	world();
	virtual ~world();

	entt* entityArray[MAX_ENTITIES_IN_WORLD];
	int entityArrayCurrentSize = 0;
	int totalMadeEntts = 0;

	entt_camera *currentlyRenderingCam; //(Levente): Each world can have as many cams as we like. The cam that we would like rendering from 
									    // will be set by setting this pointer.

	entt* make_desired_entt(entts inDesiredEntity);

	bool cameraSwitchedLastFrame = false;

	void update();
	void draw_all();
	void on_destroy();
}

#else

#endif

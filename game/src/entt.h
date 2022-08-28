#ifndef _ENTT_H_
#define _ENTT_H_

#include "raylib.h"
#include <string>

struct world;

#define MAX_ENTITIES_IN_WORLD 360

enum entts {mainCube, cam, camSetCurrentlyRendering, light};

struct entt
{
	std::string id;
	world* containingWorld;

	entt() {};
	~entt() {};

	virtual void on_make() {};
	virtual void on_destroy() {};
	virtual void on_update() {};
	virtual void on_draw_2d() {};
	virtual void on_draw_3d() {}; //(Levente): Used when drawing 3d only components!

	std::string get_id();
};

#else

#endif
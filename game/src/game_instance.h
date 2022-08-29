#ifndef _H_GAME_INSTANCE_
#define _H_GAME_INSTANCE_

#include "raylib.h"
#include "raymath.h"
#include <string>

;
#include "world.h"

;
struct game_instance
{
	std::string displayName;
	int windowWidth, windowHeight;
	//

	world* persistentWorld;

	game_instance(std::string inDisplayName, int inWindowWidth, int inWindowHeight);
	~game_instance() {};

	void update();
	void draw();
	void on_init();
	void on_exit();

	void load_world(std::string inWorldName);
};



#else

#endif

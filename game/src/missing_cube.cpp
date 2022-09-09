/*******************************************************************************************
*
*  Missing Cube
* 
*  A game by Levente Biro (C) 2022. Uses raylib. (rlgl.h, rcamera.h is modified!)
*
********************************************************************************************/

#include "game_instance.h"

int main()
{

	game_instance* currentInstance = new game_instance("Missing Cube", 1600, 900);

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		currentInstance->update();
		currentInstance->draw();

	}

	currentInstance->on_exit();
	delete currentInstance;

	return 0;

}

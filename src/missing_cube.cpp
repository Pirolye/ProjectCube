/*******************************************************************************************
*
*  Missing Cube
* 
*  A game by Levente Biro (C) 2022. Uses raylib 4.5 (MODIFIED)
*									Uses Nvidia Physx 5.2
*									Uses graphene 1.10.8
*
********************************************************************************************/

#include "game_instance.h"

int main(int argc, char** argv)
{
	game_instance* currentInstance = new game_instance("Missing Cube", 1600, 900, 75);

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

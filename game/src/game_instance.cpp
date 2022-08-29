#include "game_instance.h"
#include "assert.h"

game_instance::game_instance(std::string inDisplayName, int inWindowWidth, int inWindowHeight)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

	// Initialization
	InitWindow(inWindowWidth, inWindowHeight, inDisplayName.c_str());
	displayName = inDisplayName;
	windowWidth = inWindowWidth;
	windowHeight = inWindowHeight;

	InitAudioDevice();      // Initialize audio device
	SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

	persistentWorld = new world;

	//
	//
	//
}

void game_instance::update()
{
	assert(persistentWorld != NULL); assert(persistentWorld != nullptr);
	
	persistentWorld->update();
};

void game_instance::draw()
{
	persistentWorld->draw_all();
};

void game_instance::on_exit()
{
	persistentWorld->on_destroy();

	CloseAudioDevice();     // Close audio context

	CloseWindow();          // Close window and OpenGL context

}

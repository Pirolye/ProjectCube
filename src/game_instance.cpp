#include "game_instance.h"
#include "assert.h"

using namespace physx;

game_instance::game_instance(std::string inDisplayName, int inWindowWidth, int inWindowHeight)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("120.10.0.0", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	// Initialization
	InitWindow(inWindowWidth, inWindowHeight, inDisplayName.c_str());
	displayName = inDisplayName;
	windowWidth = inWindowWidth;
	windowHeight = inWindowHeight;

	InitAudioDevice();      // Initialize audio device
	SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

	persistentWorld = new world(this, gPhysics);

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

	gPhysics->release();
	gFoundation->release();

	CloseWindow();          // Close window and OpenGL context

}

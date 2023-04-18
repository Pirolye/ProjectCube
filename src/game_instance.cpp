#include "game_instance.h"
#include "assert.h"

using namespace physx;

game_instance::game_instance(std::string inDisplayName, int inWindowWidth, int inWindowHeight, float inTargetFPS)
{

	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	// Initialization
	InitWindow(inWindowWidth, inWindowHeight, inDisplayName.c_str());
	displayName = inDisplayName;
	windowWidth = inWindowWidth;
	windowHeight = inWindowHeight;
	SetExitKey(0);

	InitAudioDevice();      // Initialize audio device
	SetTargetFPS(inTargetFPS); 

	targetFPS = inTargetFPS;

	persistentWorld = new world;
	world_init(persistentWorld, this, gPhysics);

	//
	//
	//
}

void game_instance::update()
{
	assert(persistentWorld != NULL); assert(persistentWorld != nullptr);
	
	world_update(persistentWorld);
};

void game_instance::draw()
{
	assert(persistentWorld != NULL); assert(persistentWorld != nullptr);

	world_draw_all(persistentWorld);
};

void game_instance::on_exit()
{
	world_deinit(persistentWorld);

	CloseAudioDevice();     // Close audio context

	gPhysics->release();
	gFoundation->release();

	CloseWindow();          // Close window and OpenGL context

}

#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"
#include "entt_camera.h"
#include "entt_maincube.h"

;
#include "entt_light.h"

;
#include "PxPhysicsAPI.h"
using namespace physx;

struct game_instance;


;
//(Levente): The world houses the main entity array plus standard configuration you would want. Loading worlds is taken care of by the game instance.
struct world
{
	world(game_instance* inGameInstance, PxPhysics* inPhysicsMemAddress); // Alias init
	virtual ~world(); // Separate from on_destroy()

	std::string name;

	entt* entityArray[MAX_ENTITIES_IN_WORLD];
	int entityArrayCurrentSize = 0;
	int totalMadeEntts = 0;

	// Physics
	PxScene* gScene;
	PxPhysics* globalPhysics;

	game_instance* gameInstance;

	//(Levente): Lighting is done inside the shaders of course, but lights are also entites in their own term. This means we have to manually update all shaders
	//to notify them of any changes that might happened in lighting data.
	Shader currentlyLoadedShaders[MAX_ENTITIES_IN_WORLD * 2];
	float defaultAmbientLightValue[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	//(Levente): Each world has 1 camera located at index 0. We switch the properties of this when exiting/entering the editor.
	Vector3 gameCameraPosition;

	//(Levente): The make functions make the data structures and also do whatever is neccessary at that point in gameplay. Usually registering to arrays.
	entt* make_desired_entt(entts inDesiredEntity);
	Shader make_shader(const char* vertexShader, const char* fragmentShader);

	bool cameraSwitchedLastFrame = false;

	// Editor functions
	entt* currentlySelectedEntt = nullptr;
	Ray cursorSelectionRay = { 0 };
	void editor_try_select_entt();
	void editor_move_entt(int axis, float val);
	void editor_rotate_entt(int axis, float val);
	void update_world_editor();
	void enter_editor_mode();
	void exit_editor_mode();
	bool isInEditorMode = false;
	int currentlyEditingAxis = 0; // 0 = x, 1 = y, 2 = z

	void update();
	void draw_all();
	void on_destroy();

	//(Levente): Will be factored out in the future.
	void run_script_on_init();
	void run_script_on_update();
	void run_script_on_destroy();
};

#else

#endif

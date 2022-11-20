#ifndef _WORLD_H_
#define _WORLD_H_

#include "entt.h"
#include "entt_camera.h"
#include "entt_maincube.h"

;
#include "entt_light.h"

;
#include "PxPhysicsAPI.h"
#include "graphene.h"

using namespace physx;

struct game_instance;

struct world_editor
{
	bool isInEditorMode = false;
	int editorCurrentlyEditingAxis = 0; // 0 = x, 1 = y, 2 = z

	entt* editorCurrentlySelectedEntt = nullptr;

	bool canSelectEntt = true;
	bool selectingGizmoMoveAxisX = false;
	bool selectingGizmoMoveAxisY = false;
	bool selectingGizmoMoveAxisZ = false;

	Ray cursorSelectionRay = { 0 };	
	
	Model editorGizmoMoveAxisX{};
	Model editorGizmoMoveAxisY{};
	Model editorGizmoMoveAxisZ{};

	Model editorGizmoMoveAxisXY{};
	Model editorGizmoMoveAxisYZ{};
	Model editorGizmoMoveAxisZX{};
	
	Texture editorGizmoMoveAxisMat{};
	
	Mesh editorGizmoHelperMesh{};
	Model editorGizmoHelperModel{};

};

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
	// This is because of a limitation arising from raycamera.h
	Vector3 gameCameraPosition;
	bool canMoveCamera = false;

	//(Levente): The make functions make the data structures and also do whatever is neccessary at that point in gameplay. Usually registering to arrays.
	entt* make_desired_entt(entts inDesiredEntity);
	Shader make_shader(const char* vertexShader, const char* fragmentShader);

	bool cameraSwitchedLastFrame = false;

	// Editor functions
	world_editor worldEditor{};
	void editor_try_select_entt();
	void editor_move_entt(int axis, float val);
	void editor_rotate_entt(int axis, float val);
	void update_world_editor();
	void draw_world_editor_3d();
	void draw_world_editor_2d();
	void enter_editor_mode();
	void exit_editor_mode();
	void editor_draw_gizmo(Vector3 inCenterPos);
	void editor_check_against_gizmo(Vector3 inGizmoCenterPos);
	void editor_move_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToMove);

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

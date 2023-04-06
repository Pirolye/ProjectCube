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
	int currentGizmoMode = 0; //(Levente): This determines move/rotate/scale gizmo. 0 = move, 1 = rotate, 2 = scale
	int currentlyEditingAxis = 0; // 0 = x, 1 = y, 2 = z; 3 = xy, 4 = yz. 5 = zx

	entt* editorCurrentlySelectedEntt = nullptr;

	bool canSelectEntt = true;
	bool selectingGizmoMoveAxisX = false;
	bool selectingGizmoMoveAxisY = false;
	bool selectingGizmoMoveAxisZ = false;
	bool selectingGizmoMoveAxisXY = false;
	bool selectingGizmoMoveAxisYZ = false;
	bool selectingGizmoMoveAxisZX = false;

	Model editorGizmoMoveAxisX{};
	Model editorGizmoMoveAxisY{};
	Model editorGizmoMoveAxisZ{};

	Model editorGizmoMoveAxisXY{};
	Model editorGizmoMoveAxisYZ{};
	Model editorGizmoMoveAxisZX{};

	Model editorGizmoRotateAxisX{};
	Model editorGizmoRotateAxisY{};
	Model editorGizmoRotateAxisZ{};
	bool selectingGizmoRotateAxisX = false;
	bool selectingGizmoRotateAxisY = false;
	bool selectingGizmoRotateAxisZ = false;

	Texture editorGizmoAxisMat{};
	
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

	entt* entityArray[MAX_ENTITIES_IN_WORLD] = { NULL };
	int entityArrayCurrentSize = 0;
	int totalMadeEntts = 0;

	// Physics
	PxScene* gScene;
	PxPhysics* globalPhysics;

	game_instance* gameInstance;

	//(Levente): Lighting is done inside the shaders of course, but lights are also entites in their own term. This means we have to manually update all shaders
	//to notify them of any changes that might happened in lighting data.
	Shader currentlyLoadedShaders[MAX_ENTITIES_IN_WORLD * 64] = { NULL }; //Note: We have to do NULL initialization here because the compiler won't do it when defining! (Even though it accepts it as valid syntax...)
	float defaultAmbientLightValue[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	//(Levente): Since the rcamera remake in raylib 4.5, we can have multiple cameras without issue!
	entt_camera* currentlyRenderingCamera;

	//(Levente): The make functions make the data structures and also do whatever is neccessary at that point in gameplay. Usually registering to arrays.
	template <typename t> entt* make_desired_entt();
	Shader make_shader(const char* vertexShader, const char* fragmentShader);
	void set_cam(entt_camera* i);

	bool cameraSwitchedLastFrame = false;

	// Editor functions
	world_editor worldEditor{};
	void editor_try_select_entt();
	void editor_move_entt(int axis, float val);
	void editor_rotate_entt(int axis, float val);
	void init_world_editor();
	void update_world_editor();
	void draw_world_editor_3d();
	void draw_world_editor_2d();
	void enter_editor_mode();
	void exit_editor_mode();
	void editor_draw_gizmo(Vector3 inCenterPos);
	void editor_check_against_move_gizmo(Vector3 inGizmoCenterPos);
	void editor_check_against_rotate_gizmo(Vector3 inGizmoCenterPos);
	void editor_move_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToMove);
	void editor_rotate_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToRotate);
	bool editor_is_selecting_any_gizmo();
	void editor_do_not_select_any_gizmo();
	void editor_next_camera();
	//void editor_scale_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToRotate); // We need entt transform overhaul for this!

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

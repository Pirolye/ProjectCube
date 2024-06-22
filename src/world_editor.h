#ifndef _H_WORLD_EDITOR_
#define _H_WORLD_EDITOR_

#include "world.h"

struct world_editor_ui;

struct gizmo_model
{
	Model model;
	int axis;
	bool isSelected;
	Texture texture;
	Matrix helperModelRotation;
	float prevPoint;
	float currentPoint;
};

struct gizmo
{
	gizmo_model* model[6];
	// 0 = X, 1 = Y, 2 = Z, 3 = XY, 4 = YZ, 5 = ZX
	gizmo_model* currentlySelectedModel;
};

struct world_editor
{
	world* currentWorld;
	
	bool isInEditorMode = false;
	int currentGizmoMode = 0; //(Levente): This determines move/rotate/scale gizmo. 0 = move, 1 = rotate, 2 = scale
	int currentlyEditingAxis = 0; // 0 = x, 1 = y, 2 = z; 3 = xy, 4 = yz. 5 = zx

	entity* currentlySelectedEntity = nullptr;
	entity* currentlyCopiedEntity = nullptr;
	model* visibilityArray[1024] = { NULL };
	bool selectingEntt = false;

	gizmo* moveGizmo;
	gizmo* scaleGizmo;
	gizmo* rotateGizmo;
	
	bool canManipulateWorld = true;

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

	world_editor_ui* editorUI;


};

entity* editor_try_select_entity(world_editor* inWorldEditor);
void editor_copy_entity(entity* entityToCopy, world_editor* inEditor);
void editor_paste_entity(world_editor* inEditor);

void init_world_editor(world_editor* inWorldEditor, world* inCurrentWorld);
void shutdown_world_editor(world_editor* inWorldEditor);
void update_world_editor(world_editor* inWorldEditor);
void draw_world_editor_3d(world_editor* inWorldEditor);
void draw_world_editor_2d(world_editor* inWorldEditor);

void enter_editor_mode(world_editor* inWorldEditor);
void exit_editor_mode(world_editor* inWorldEditor);

void editor_draw_gizmo(world_editor* inWorldEditor, Vector3 inCenterPos);

void editor_check_against_move_gizmo(world_editor* inWorldEditor, Vector3 inGizmoCenterPos);
void editor_check_against_rotate_gizmo(world_editor* inWorldEditor, Vector3 inGizmoCenterPos);
void editor_check_against_scale_gizmo(world_editor* inWorldEditor, Vector3 inGizmoCenterPos);

void editor_move_entity_gizmo(world_editor* inWorldEditor, int inAxis, Vector3 inGizmoCenterPos, entity* entityToMove);
void editor_scale_entity_gizmo(world_editor* inWorldEditor, int inAxis, Vector3 inGizmoCenterPos, entity* entityToMove);
void editor_rotate_entity_gizmo(world_editor* inWorldEditor, int inAxis, Vector3 inGizmoCenterPos, entity* entityToRotate);

bool editor_is_selecting_any_gizmo(world_editor* inWorldEditor);

void editor_do_not_select_any_gizmo(world_editor* inWorldEditor);

void editor_next_camera(world_editor* inWorldEditor);
//void editor_scale_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToRotate); // We need entt transform overhaul for this!


#else

#endif

#include "world.h"

;
#include "raylib.h"

#include "raymath.h"

/*

BIRO: I decided to separate the standard world functions from the editor ones because the main file was getting bloated and hard to navigate.


*/

#ifdef DEBUG

bool world::editor_is_selecting_any_gizmo()
{
	if (worldEditor.selectingGizmoMoveAxisX
		|| worldEditor.selectingGizmoMoveAxisY
		|| worldEditor.selectingGizmoMoveAxisZ
		|| worldEditor.selectingGizmoMoveAxisXY
		|| worldEditor.selectingGizmoMoveAxisYZ
		|| worldEditor.selectingGizmoMoveAxisZX 
		|| worldEditor.selectingGizmoRotateAxisX 
		|| worldEditor.selectingGizmoRotateAxisY 
		|| worldEditor.selectingGizmoRotateAxisZ) 
	{
		return true;
	}
	else
	{
		return false;
	}

}

void world::editor_do_not_select_any_gizmo()
{
	worldEditor.selectingGizmoMoveAxisX = false;
	worldEditor.selectingGizmoMoveAxisY = false;
	worldEditor.selectingGizmoMoveAxisZ = false;
	worldEditor.selectingGizmoMoveAxisXY = false;
	worldEditor.selectingGizmoMoveAxisYZ = false;
	worldEditor.selectingGizmoMoveAxisZX = false;
	worldEditor.selectingGizmoRotateAxisX = false;
	worldEditor.selectingGizmoRotateAxisY = false;
	worldEditor.selectingGizmoRotateAxisZ = false;
}

void world::enter_editor_mode()
{
	worldEditor.isInEditorMode = true;
	canMoveCamera = true;

	entt_camera* cam = dynamic_cast<entt_camera*>(entityArray[0]); //(Levente): Camera is always at index 0!

	cam->rayCam->position = Vector3{ 0.0f, 0.0f, -5.0f }; // Camera position
	cam->rayCam->target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	cam->rayCam->up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	cam->rayCam->fovy = 70.0f;                                // Camera field-of-view 
	cam->rayCam->projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode((*cam->rayCam), CAMERA_FREE); // Set a free camera mode

}

void world::exit_editor_mode()
{
	worldEditor.isInEditorMode = false;
	canMoveCamera = false;

	entt_camera* cam = dynamic_cast<entt_camera*>(entityArray[0]); //(Levente): Camera is always at index 0!

	cam->rayCam->position = Vector3{ 0.0f, 0.0f, -5.0f }; // Camera position
	cam->rayCam->target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	cam->rayCam->up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	cam->rayCam->fovy = 70.0f;                                // Camera field-of-view 
	cam->rayCam->projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode((*cam->rayCam), CAMERA_FREE); // Set a free camera mode

	worldEditor.editorCurrentlySelectedEntt = nullptr;

	// START WORLD AGAIN FROM THE BEGINNING
}

void world::init_world_editor()
{
	worldEditor.editorGizmoAxisMat = LoadTexture("editor/gizmo_move_axis_albedo.png");

	worldEditor.editorGizmoMoveAxisX = LoadModel("editor/gizmo_move_axis.obj");
	worldEditor.editorGizmoMoveAxisY = LoadModel("editor/gizmo_move_axis.obj");
	worldEditor.editorGizmoMoveAxisZ = LoadModel("editor/gizmo_move_axis.obj");
	worldEditor.editorGizmoMoveAxisXY = LoadModel("editor/gizmo_move_axis_combined.obj");
	worldEditor.editorGizmoMoveAxisYZ = LoadModel("editor/gizmo_move_axis_combined.obj");
	worldEditor.editorGizmoMoveAxisZX = LoadModel("editor/gizmo_move_axis_combined.obj");

	worldEditor.editorGizmoRotateAxisX = LoadModel("editor/gizmo_rotate_axis.obj");
	worldEditor.editorGizmoRotateAxisY = LoadModel("editor/gizmo_rotate_axis.obj");
	worldEditor.editorGizmoRotateAxisZ = LoadModel("editor/gizmo_rotate_axis.obj");
	worldEditor.editorGizmoRotateAxisX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;
	worldEditor.editorGizmoRotateAxisY.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;
	worldEditor.editorGizmoRotateAxisZ.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;

	worldEditor.editorGizmoMoveAxisX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;
	worldEditor.editorGizmoMoveAxisY.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;
	worldEditor.editorGizmoMoveAxisZ.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;

	worldEditor.editorGizmoMoveAxisXY.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;
	worldEditor.editorGizmoMoveAxisYZ.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;
	worldEditor.editorGizmoMoveAxisZX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = worldEditor.editorGizmoAxisMat;

	worldEditor.editorGizmoHelperMesh = GenMeshPlane(99999.0f, 99999.0f, 10, 10);
	worldEditor.editorGizmoHelperModel = LoadModelFromMesh(worldEditor.editorGizmoHelperMesh);

}

void world::update_world_editor()
{
	if (worldEditor.editorCurrentlySelectedEntt != nullptr)
	{
		if(worldEditor.currentGizmoMode == 0) editor_check_against_move_gizmo(worldEditor.editorCurrentlySelectedEntt->enttTransform.pos);
		if (worldEditor.currentGizmoMode == 1) editor_check_against_rotate_gizmo(worldEditor.editorCurrentlySelectedEntt->enttTransform.pos);

	}
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && worldEditor.canSelectEntt) editor_try_select_entt();

	if (IsKeyPressed(KEY_TAB))
	{
		worldEditor.currentlyEditingAxis = worldEditor.currentlyEditingAxis + 1;
		if (worldEditor.currentlyEditingAxis == 3) worldEditor.currentlyEditingAxis = 0;
	}
	
	// Set gizmo modes
	if(!editor_is_selecting_any_gizmo()) 
	{
		if (IsKeyPressed(KEY_W))
		{
			worldEditor.currentGizmoMode = 0;
		}
		if (IsKeyPressed(KEY_E))
		{
			worldEditor.currentGizmoMode = 1;
		}
		if (IsKeyPressed(KEY_R))
		{
			worldEditor.currentGizmoMode = 2;
		}
	}

	//Will be removed once we got editor gui

	// @@REDUNDANT, WONT FIX WARNING
	if (worldEditor.editorCurrentlySelectedEntt != NULL)
	{
		DrawText(worldEditor.editorCurrentlySelectedEntt->id.c_str(), 10, 550, 20, RED);

		/*
		if (IsKeyDown(KEY_W))
		{
			editor_move_entt(worldEditor.currentlyEditingAxis, 0.05f);
		}
		if (IsKeyDown(KEY_S))
		{
			editor_move_entt(worldEditor.currentlyEditingAxis, -0.05f);
		}
		if (IsKeyDown(KEY_A))
		{
			editor_rotate_entt(worldEditor.currentlyEditingAxis, 1.0f);
		}
		if (IsKeyDown(KEY_D))
		{
			editor_rotate_entt(worldEditor.currentlyEditingAxis, -1.0f);
		}*/


	}

}

void world::draw_world_editor_3d()
{
	//(Levente): This will draw the model used to calculate the new position of the actor based on the raycasts. 
	DrawModel(worldEditor.editorGizmoHelperModel, Vector3Zero(), 1.0f, RED);
	
	
	if(worldEditor.editorCurrentlySelectedEntt != nullptr) editor_draw_gizmo(worldEditor.editorCurrentlySelectedEntt->enttTransform.pos);

}

void world::draw_world_editor_2d()
{

}


void world::editor_move_entt(int axis, float val)
{
	if (worldEditor.editorCurrentlySelectedEntt == nullptr) return;

	if (axis == 0)
	{
		entt_transform t = worldEditor.editorCurrentlySelectedEntt->enttTransform;
		worldEditor.editorCurrentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x + val, t.pos.y, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 1)
	{
		entt_transform t = worldEditor.editorCurrentlySelectedEntt->enttTransform;
		worldEditor.editorCurrentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y + val, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 2)
	{
		entt_transform t = worldEditor.editorCurrentlySelectedEntt->enttTransform;
		worldEditor.editorCurrentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y, t.pos.z + val }, t.scale, t.rot);

	}

}

void world::editor_rotate_entt(int axis, float val)
{
	if (worldEditor.editorCurrentlySelectedEntt == nullptr) return;

	if (axis == 0)
	{
		entt_transform t = worldEditor.editorCurrentlySelectedEntt->enttTransform;

		worldEditor.editorCurrentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x + val, t.rot.y, t.rot.z });

	}
	if (axis == 1)
	{
		entt_transform t = worldEditor.editorCurrentlySelectedEntt->enttTransform;

		worldEditor.editorCurrentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x, t.rot.y + val, t.rot.z });

	}
	if (axis == 2)
	{
		entt_transform t = worldEditor.editorCurrentlySelectedEntt->enttTransform;

		worldEditor.editorCurrentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x, t.rot.y, t.rot.z + val });

	}

}



void world::editor_try_select_entt()
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_light*>(entityArray[i]) != nullptr) worldEditor.editorCurrentlySelectedEntt = dynamic_cast<entt_light*>(entityArray[i])->editor_try_select(worldEditor.cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube*>(entityArray[i]) != nullptr) worldEditor.editorCurrentlySelectedEntt = dynamic_cast<entt_maincube*>(entityArray[i])->editor_try_select(worldEditor.cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube_static*>(entityArray[i]) != nullptr) worldEditor.editorCurrentlySelectedEntt = dynamic_cast<entt_maincube_static*>(entityArray[i])->editor_try_select(worldEditor.cursorSelectionRay, collision);
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr) worldEditor.editorCurrentlySelectedEntt = dynamic_cast<entt_camera*>(entityArray[i])->editor_try_select(worldEditor.cursorSelectionRay, collision);

			if (worldEditor.editorCurrentlySelectedEntt != nullptr) break;
			else continue;
		}
	}
};



#else

void world::editor_move_entt(int axis, float val) {};
void world::enter_editor_mode() {};
void world::exit_editor_mode() {};
void world::init_world_editor() {};
void world::draw_world_editor_2d() {};
void world::draw_world_editor_3d() {};
void world::update_world_editor() {};
void world::editor_try_select_entt() {};
void world::editor_draw_gizmo(Vector3 i) {};
void world::editor_check_against_gizmo() {};


#endif
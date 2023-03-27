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
	if(worldEditor.currentlyEditingAxis == 0) DrawText("x", 0, 100, 30, WHITE);
	if(worldEditor.currentlyEditingAxis == 1) DrawText("y", 0, 100, 30, WHITE);
	if(worldEditor.currentlyEditingAxis == 2) DrawText("z", 0, 100, 30, WHITE);
	if(worldEditor.currentlyEditingAxis == 3) DrawText("xy", 0, 100, 30, WHITE);
	if(worldEditor.currentlyEditingAxis == 4) DrawText("yz", 0, 100, 30, WHITE);
	if(worldEditor.currentlyEditingAxis == 5) DrawText("zx", 0, 100, 30, WHITE);

	if (worldEditor.selectingGizmoMoveAxisX) DrawText("Moving X gizmo", 0, 150, 30, WHITE);
	if (worldEditor.selectingGizmoMoveAxisY) DrawText("Moving Y gizmo", 0, 150, 30, WHITE);
	if (worldEditor.selectingGizmoMoveAxisZ) DrawText("Moving Z gizmo", 0, 150, 30, WHITE);
	if (worldEditor.selectingGizmoMoveAxisXY) DrawText("Moving XY gizmo", 0, 150, 30, WHITE);
	if (worldEditor.selectingGizmoMoveAxisYZ) DrawText("Moving YZ gizmo", 0, 150, 30, WHITE);
	if (worldEditor.selectingGizmoMoveAxisZX) DrawText("Moving ZX gizmo", 0, 150, 30, WHITE);

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

/*
* 
* BIRO: About gizmos. We draw each model separately and adjust rotation. When the left mouse is held down, we raycast against them. If we hit one of them,
*       a variable is set indicating we are holding onto one. When we do that, we raycast each frame to get the delta between last frame's mouse pos against the helper mesh 
*		(which is transformed behind the gizmo model we're holding) and the current frame's. We then apply this delta to the position of the object on the axis we're
*		were moving on. 
* 
*		This is probably really slow, but it doesn't use and trigonometry and such, so it's easy to understand and maintain.
* 
*/

void world::editor_draw_gizmo(Vector3 inCenterPos)
{
	Vector3 v{ 1.0f, 0.0f, 0.0f };
	//Color c1{ 0, 255, 0, 100 };

	if (worldEditor.currentGizmoMode == 0)
	{

		Matrix matScaleX = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationX = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD * 90.0f });
		Matrix matTranslationX = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		worldEditor.editorGizmoMoveAxisX.transform = MatrixMultiply(MatrixMultiply(matScaleX, matRotationX), matTranslationX);

		Matrix matScaleY = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationY = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f , 0.0f });
		Matrix matTranslationY = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		worldEditor.editorGizmoMoveAxisY.transform = MatrixMultiply(MatrixMultiply(matScaleY, matRotationY), matTranslationY);

		Matrix matScaleZ = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationZ = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f , 0.0f });
		Matrix matTranslationZ = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		worldEditor.editorGizmoMoveAxisZ.transform = MatrixMultiply(MatrixMultiply(matScaleZ, matRotationZ), matTranslationZ);

		Matrix matScaleXY = MatrixScale(0.8f, 0.8f, 0.8f);
		Matrix matRotationXY = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f, 0.0f });
		Matrix matTranslationXY = MatrixTranslate(inCenterPos.x + 1.5f, inCenterPos.y + 1.5f, inCenterPos.z);
		worldEditor.editorGizmoMoveAxisXY.transform = MatrixMultiply(MatrixMultiply(matScaleXY, matRotationXY), matTranslationXY);

		Matrix matScaleYZ = MatrixScale(0.8f, 0.8f, 0.8f);
		Matrix matRotationYZ = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f , DEG2RAD * 90.0f }); //WARNING: When rotating, we change the local coordinate system orientation, so we 
																									// need to rotate on a different axis to get global coordinate system equivalent results
		Matrix matTranslationYZ = MatrixTranslate(inCenterPos.x, inCenterPos.y + 1.5f, inCenterPos.z - 1.5f);
		worldEditor.editorGizmoMoveAxisYZ.transform = MatrixMultiply(MatrixMultiply(matScaleYZ, matRotationYZ), matTranslationYZ);

		Matrix matScaleZX = MatrixScale(0.8f, 0.8f, 0.8f);
		Matrix matRotationZX = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f , 0.0f });
		Matrix matTranslationZX = MatrixTranslate(inCenterPos.x + 1.5f, inCenterPos.y, inCenterPos.z - 1.5f);
		worldEditor.editorGizmoMoveAxisZX.transform = MatrixMultiply(MatrixMultiply(matScaleZX, matRotationZX), matTranslationZX);

		if (worldEditor.selectingGizmoMoveAxisX == false)
		{
			DrawModel(worldEditor.editorGizmoMoveAxisX, Vector3Zero(), 1.0f, RED);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoMoveAxisX, Vector3Zero(), 1.0f, WHITE);
		}

		if (worldEditor.selectingGizmoMoveAxisY == false)
		{
			DrawModel(worldEditor.editorGizmoMoveAxisY, Vector3Zero(), 1.0f, GREEN);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoMoveAxisY, Vector3Zero(), 1.0f, WHITE);
		}

		if (worldEditor.selectingGizmoMoveAxisZ == false)
		{
			DrawModel(worldEditor.editorGizmoMoveAxisZ, Vector3Zero(), 1.0f, BLUE);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoMoveAxisZ, Vector3Zero(), 1.0f, WHITE);
		}

		if (worldEditor.selectingGizmoMoveAxisXY == false)
		{
			DrawModel(worldEditor.editorGizmoMoveAxisXY, Vector3Zero(), 1.0f, ORANGE);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoMoveAxisXY, Vector3Zero(), 1.0f, WHITE);
		}

		if (worldEditor.selectingGizmoMoveAxisYZ == false)
		{
			DrawModel(worldEditor.editorGizmoMoveAxisYZ, Vector3Zero(), 1.0f, SKYBLUE);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoMoveAxisYZ, Vector3Zero(), 1.0f, WHITE);
		}

		if (worldEditor.selectingGizmoMoveAxisZX == false)
		{
			DrawModel(worldEditor.editorGizmoMoveAxisZX, Vector3Zero(), 1.0f, PURPLE);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoMoveAxisZX, Vector3Zero(), 1.0f, WHITE);
		}
	}
	
	
	if (worldEditor.currentGizmoMode == 1)
	{

		Matrix matScaleX = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationX = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * 180.0f, 0.0f });
		Matrix matTranslationX = MatrixTranslate(inCenterPos.x, inCenterPos.y + 1.8f, inCenterPos.z - 0.5f);
		worldEditor.editorGizmoRotateAxisX.transform = MatrixMultiply(MatrixMultiply(matScaleX, matRotationX), matTranslationX);

		Matrix matScaleY = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationY = MatrixRotateXYZ(Vector3{ DEG2RAD * -90.0f, DEG2RAD * 90.0f , 0.0f });
		Matrix matTranslationY = MatrixTranslate(inCenterPos.x + 0.2f, inCenterPos.y - 0.4f, inCenterPos.z - 2.4f);
		worldEditor.editorGizmoRotateAxisY.transform = MatrixMultiply(MatrixMultiply(matScaleY, matRotationY), matTranslationY);

		Matrix matScaleZ = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationZ = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * 90.0f , 0.0f });
		Matrix matTranslationZ = MatrixTranslate(inCenterPos.x + 0.2f, inCenterPos.y + 1.8f, inCenterPos.z);
		worldEditor.editorGizmoRotateAxisZ.transform = MatrixMultiply(MatrixMultiply(matScaleZ, matRotationZ), matTranslationZ);

		if (worldEditor.selectingGizmoRotateAxisX == false)
		{
			DrawModel(worldEditor.editorGizmoRotateAxisX, Vector3Zero(), 1.0f, RED);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoRotateAxisX, Vector3Zero(), 1.0f, WHITE);
		}

		if (worldEditor.selectingGizmoRotateAxisY == false)
		{
			DrawModel(worldEditor.editorGizmoRotateAxisY, Vector3Zero(), 1.0f, GREEN);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoRotateAxisY, Vector3Zero(), 1.0f, WHITE);
		}

		if (worldEditor.selectingGizmoRotateAxisZ == false)
		{
			DrawModel(worldEditor.editorGizmoRotateAxisZ, Vector3Zero(), 1.0f, BLUE);
		}
		else
		{
			DrawModel(worldEditor.editorGizmoRotateAxisZ, Vector3Zero(), 1.0f, WHITE);
		}
	}


}

void world::editor_check_against_move_gizmo(Vector3 inCenterPos)
{
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };
		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

		meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoMoveAxisX.meshes[0], worldEditor.editorGizmoMoveAxisX.transform);
		if (meshHitInfo.hit)
		{
			if (worldEditor.selectingGizmoMoveAxisY == true 
				|| worldEditor.selectingGizmoMoveAxisZ == true
				|| worldEditor.selectingGizmoMoveAxisXY == true
				|| worldEditor.selectingGizmoMoveAxisYZ == true
				|| worldEditor.selectingGizmoMoveAxisZX == true)
			{

			}

			worldEditor.selectingGizmoMoveAxisX = true;
			worldEditor.canSelectEntt = false;

		}
		else
		{
			//(Levente): Lovely thing here. Keep latching on even if we've left the mesh's bounding box.
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoMoveAxisX)
			{
				worldEditor.selectingGizmoMoveAxisX = true;
				worldEditor.selectingGizmoMoveAxisY = false; // This is so that you don't grab onto another gizmo when grabbing this
				worldEditor.selectingGizmoMoveAxisZ = false; // UPDATE: This may not work but I'm not testing it now. Has no practical implications
				worldEditor.canSelectEntt = false;

			}
			else
			{
				worldEditor.selectingGizmoMoveAxisX = false;
			}

		}

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < worldEditor.editorGizmoMoveAxisY.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoMoveAxisY.meshes[m], worldEditor.editorGizmoMoveAxisY.transform);
			if (meshHitInfo.hit)
			{
				if (worldEditor.selectingGizmoMoveAxisX == true
					|| worldEditor.selectingGizmoMoveAxisZ == true
					|| worldEditor.selectingGizmoMoveAxisXY == true
					|| worldEditor.selectingGizmoMoveAxisYZ == true
					|| worldEditor.selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				worldEditor.selectingGizmoMoveAxisY = true;
				worldEditor.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoMoveAxisY)
				{
					worldEditor.selectingGizmoMoveAxisY = true;
					worldEditor.selectingGizmoMoveAxisX = false;
					worldEditor.selectingGizmoMoveAxisZ = false;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					worldEditor.selectingGizmoMoveAxisY = false;
				}

			}
		}

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < worldEditor.editorGizmoMoveAxisZ.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoMoveAxisZ.meshes[m], worldEditor.editorGizmoMoveAxisZ.transform);
			if (meshHitInfo.hit)
			{
				if (worldEditor.selectingGizmoMoveAxisY == true
					|| worldEditor.selectingGizmoMoveAxisX == true
					|| worldEditor.selectingGizmoMoveAxisXY == true
					|| worldEditor.selectingGizmoMoveAxisYZ == true
					|| worldEditor.selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				worldEditor.selectingGizmoMoveAxisZ = true;
				worldEditor.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoMoveAxisZ)
				{
					worldEditor.selectingGizmoMoveAxisZ = true;
					worldEditor.selectingGizmoMoveAxisX = false;
					worldEditor.selectingGizmoMoveAxisY = false;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					worldEditor.selectingGizmoMoveAxisZ = false;
				}

			}
		}



		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < worldEditor.editorGizmoMoveAxisXY.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoMoveAxisXY.meshes[m], worldEditor.editorGizmoMoveAxisXY.transform);
			if (meshHitInfo.hit)
			{
				//(Levente): I needed to introduce extra measures to stop you from grabbing another axis when already grabbing one. The previous fix for this may not work now,
				// but it's so difficult to reproduce if the actual transform code is in place that I won't try.
				if (worldEditor.selectingGizmoMoveAxisX == true
					|| worldEditor.selectingGizmoMoveAxisZ == true
					|| worldEditor.selectingGizmoMoveAxisY == true
					//|| worldEditor.selectingGizmoMoveAxisXY == true
					|| worldEditor.selectingGizmoMoveAxisYZ == true
					|| worldEditor.selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				worldEditor.selectingGizmoMoveAxisXY = true;
				worldEditor.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoMoveAxisXY)
				{
					worldEditor.selectingGizmoMoveAxisXY = true;
					worldEditor.selectingGizmoMoveAxisYZ = false;
					worldEditor.selectingGizmoMoveAxisZX = false;

					worldEditor.selectingGizmoMoveAxisY = false;
					worldEditor.selectingGizmoMoveAxisX = false;
					worldEditor.selectingGizmoMoveAxisZ = false;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					worldEditor.selectingGizmoMoveAxisXY = false;
				}

			}
		}
		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < worldEditor.editorGizmoMoveAxisYZ.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoMoveAxisYZ.meshes[m], worldEditor.editorGizmoMoveAxisYZ.transform);
			if (meshHitInfo.hit)
			{
				if (worldEditor.selectingGizmoMoveAxisX == true
					|| worldEditor.selectingGizmoMoveAxisZ == true
					|| worldEditor.selectingGizmoMoveAxisY == true
					|| worldEditor.selectingGizmoMoveAxisXY == true
					//|| worldEditor.selectingGizmoMoveAxisYZ == true
					|| worldEditor.selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				worldEditor.selectingGizmoMoveAxisYZ = true;
				worldEditor.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoMoveAxisYZ)
				{
					worldEditor.selectingGizmoMoveAxisXY = false;
					worldEditor.selectingGizmoMoveAxisYZ = true;
					worldEditor.selectingGizmoMoveAxisZX = false;

					worldEditor.selectingGizmoMoveAxisY = false;
					worldEditor.selectingGizmoMoveAxisX = false;
					worldEditor.selectingGizmoMoveAxisZ = false;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					worldEditor.selectingGizmoMoveAxisYZ = false;
				}

			}
		}
		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < worldEditor.editorGizmoMoveAxisZX.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoMoveAxisZX.meshes[m], worldEditor.editorGizmoMoveAxisZX.transform);
			if (meshHitInfo.hit)
			{
				if (worldEditor.selectingGizmoMoveAxisX == true
					|| worldEditor.selectingGizmoMoveAxisZ == true
					|| worldEditor.selectingGizmoMoveAxisY == true
					|| worldEditor.selectingGizmoMoveAxisXY == true
					|| worldEditor.selectingGizmoMoveAxisYZ == true
					/* || worldEditor.selectingGizmoMoveAxisZX == true*/)
				{
					break; return;
				}

				worldEditor.selectingGizmoMoveAxisZX = true;
				worldEditor.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoMoveAxisZX)
				{
					worldEditor.selectingGizmoMoveAxisXY = false;
					worldEditor.selectingGizmoMoveAxisYZ = false;
					worldEditor.selectingGizmoMoveAxisZX = true;

					worldEditor.selectingGizmoMoveAxisY = false;
					worldEditor.selectingGizmoMoveAxisX = false;
					worldEditor.selectingGizmoMoveAxisZ = false;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					worldEditor.selectingGizmoMoveAxisZX = false;
				}

			}
		}
	}
	else
	{
		worldEditor.selectingGizmoMoveAxisX = false;
		worldEditor.selectingGizmoMoveAxisY = false;
		worldEditor.selectingGizmoMoveAxisZ = false;
		worldEditor.selectingGizmoMoveAxisXY = false;
		worldEditor.selectingGizmoMoveAxisYZ = false;
		worldEditor.selectingGizmoMoveAxisZX = false;

		worldEditor.canSelectEntt = true;
	}

	if (worldEditor.selectingGizmoMoveAxisX && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(0, inCenterPos, worldEditor.editorCurrentlySelectedEntt);
	if (worldEditor.selectingGizmoMoveAxisY && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(1, inCenterPos, worldEditor.editorCurrentlySelectedEntt);
	if (worldEditor.selectingGizmoMoveAxisZ && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(2, inCenterPos, worldEditor.editorCurrentlySelectedEntt);

	if (worldEditor.selectingGizmoMoveAxisXY && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(3, inCenterPos, worldEditor.editorCurrentlySelectedEntt);
	if (worldEditor.selectingGizmoMoveAxisYZ && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(4, inCenterPos, worldEditor.editorCurrentlySelectedEntt);
	if (worldEditor.selectingGizmoMoveAxisZX && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(5, inCenterPos, worldEditor.editorCurrentlySelectedEntt);



}

void world::editor_check_against_rotate_gizmo(Vector3 inCenterPos)
{
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		for (int m = 0; m < worldEditor.editorGizmoRotateAxisX.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoRotateAxisX.meshes[m], worldEditor.editorGizmoRotateAxisX.transform);

			if (meshHitInfo.hit)
			{
				if (worldEditor.selectingGizmoMoveAxisX == true
					|| worldEditor.selectingGizmoMoveAxisZ == true
					|| worldEditor.selectingGizmoMoveAxisY == true
					|| worldEditor.selectingGizmoMoveAxisXY == true
					|| worldEditor.selectingGizmoMoveAxisYZ == true
					|| worldEditor.selectingGizmoMoveAxisZX == true
					|| worldEditor.selectingGizmoRotateAxisY == true
					|| worldEditor.selectingGizmoRotateAxisZ == true)
				{
					break; return;
				}
				else
				{
					worldEditor.selectingGizmoRotateAxisX = true;
					worldEditor.canSelectEntt = false;

					break; return;

				}
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoRotateAxisX)
				{
					editor_do_not_select_any_gizmo();
					worldEditor.selectingGizmoRotateAxisX = true;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					editor_do_not_select_any_gizmo();
				}

			}
		}
	}
	else
	{
		editor_do_not_select_any_gizmo();

		worldEditor.canSelectEntt = true;
		worldEditor.selectingInPrevFrame = false;
	}


	if (worldEditor.selectingGizmoRotateAxisX && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_rotate_entt_gizmo(0, inCenterPos, worldEditor.editorCurrentlySelectedEntt);
	if (worldEditor.selectingGizmoRotateAxisY && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_rotate_entt_gizmo(1, inCenterPos, worldEditor.editorCurrentlySelectedEntt);
	if (worldEditor.selectingGizmoRotateAxisZ && worldEditor.editorCurrentlySelectedEntt != nullptr) editor_rotate_entt_gizmo(2, inCenterPos, worldEditor.editorCurrentlySelectedEntt);

}

void world::editor_move_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToMove)
{
	canMoveCamera = false;

	//(Levente): When moving, we transform the helper behind the object so that the ray hits it perpendicular to the axis. Eg. On the X axis,
	//			 it won't be position ONTO the axis, but behind it,(since an axis (here: X) is 1D; normally we imagine it as sprawling across
	//           the map, but it's actually a straight line in +x and -x) since logically that's where the mouse's ray end up.
	//
	//			 For each axis, we do 2 casts, to get starting pos and ending pos = small increments = smooth.

	if (inAxis == 0)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float currentFramePointX = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float prevFramePointX = 0.0f;

		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointX = meshHitInfo.point.x;
				break;
				return;
			}
		}


		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointX = meshHitInfo.point.x;

				float diff = currentFramePointX - prevFramePointX;

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->enttTransform.pos.x - diff, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);
				break;
				return;
			}
		}
	}

	if (inAxis == 1)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float currentFramePointY = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float prevFramePointY = 0.0f;

		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointY = meshHitInfo.point.y;

				break;
				return;
			}
		}


		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointY = meshHitInfo.point.y;

				float diff = currentFramePointY - prevFramePointY;

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };
				newPos = Vector3{ enttToMove->enttTransform.pos.x, enttToMove->enttTransform.pos.y - diff, enttToMove->enttTransform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);
				break;
				return;
			}
		}
	}
	if (inAxis == 2)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f, 0.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float currentFramePointZ = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float prevFramePointZ = 0.0f;

		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointZ = meshHitInfo.point.z;

				break;
				return;
			}
		}


		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointZ = meshHitInfo.point.z;

				float diff = currentFramePointZ - prevFramePointZ;

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->enttTransform.pos.x, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z - diff };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);
				break;
				return;
			}
		}
	}
	
	if (inAxis == 3)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector2 currentFramePointXY{ 0.0f, 0.0f };

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector2 prevFramePointXY{ 0.0f, 0.0f };

		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointXY = Vector2{ meshHitInfo.point.x, meshHitInfo.point.y };

				break;
				return;
			}
		}


		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointXY = Vector2{ meshHitInfo.point.x, meshHitInfo.point.y };

				Vector2 diff = Vector2Subtract(currentFramePointXY, prevFramePointXY);

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->enttTransform.pos.x - diff.x, enttToMove->enttTransform.pos.y - diff.y, enttToMove->enttTransform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);
				break;
				return;
			}
		}
	}

	if (inAxis == 4)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector2 currentFramePointYZ{ 0.0f, 0.0f };

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector2 prevFramePointYZ{ 0.0f, 0.0f };

		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointYZ = Vector2{ meshHitInfo.point.y, meshHitInfo.point.z };

				break;
				return;
			}
		}


		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointYZ = Vector2{ meshHitInfo.point.y, meshHitInfo.point.z };

				Vector2 diff = Vector2Subtract(currentFramePointYZ, prevFramePointYZ);

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->enttTransform.pos.x, enttToMove->enttTransform.pos.y - diff.x, enttToMove->enttTransform.pos.z - diff.y };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);
				break;
				return;
			}
		}
	}

	if (inAxis == 5)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f, 0.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector2 currentFramePointZX{ 0.0f, 0.0f };

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector2 prevFramePointZX{ 0.0f, 0.0f };

		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointZX = Vector2{ meshHitInfo.point.z, meshHitInfo.point.x };

				break;
				return;
			}
		}


		for (int m = 0; m < worldEditor.editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[m], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointZX = Vector2{ meshHitInfo.point.z, meshHitInfo.point.x };

				Vector2 diff = Vector2Subtract(currentFramePointZX, prevFramePointZX);

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->enttTransform.pos.x - diff.y, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z - diff.x };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);
				break;
				return;
			}
		}
	}


	canMoveCamera = true;
}

void world::editor_rotate_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToMove)
{
	canMoveCamera = false;

	if (inAxis == 0)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };


		if (worldEditor.selectingInPrevFrame == false)
		{
			
			Vector2 mousePos = Vector2{ GetMousePosition().x, GetMousePosition().y};
			Ray cursorSelectionRayForFirstFrame = GetMouseRay(mousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForFirstFrame, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				worldEditor.firstFramePoint = Vector3{ meshHitInfo.point.x, meshHitInfo.point.y, meshHitInfo.point.z };
				worldEditor.selectingInPrevFrame = true;

			}


		}
		
		
		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector3 currentFramePoint{};

		meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			currentFramePoint = Vector3{ meshHitInfo.point.x, meshHitInfo.point.y, meshHitInfo.point.z };

			BeginDrawing();
			BeginMode3D( *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

			DrawCubeV(worldEditor.firstFramePoint, Vector3{ 1.0f, 1.0f, 1.0f, }, BLACK);
			DrawCubeV(currentFramePoint, Vector3{ 1.0f, 1.0f, 1.0f, }, WHITE);

			float a; // This is between the first frame and the current frame
			float b; // Between the center and the first frame
			float c; // Between the center and the now frame

			float cosAlpha; // Cosine at the vertex closest to the gizmo center (final applied rotation)
			float alpha; // Angle in deg at the vertex closest to the gizmo center (final applied rotation)

			a = Vector2Distance(Vector2{ worldEditor.firstFramePoint.y, worldEditor.firstFramePoint.z }, Vector2{ currentFramePoint.y, currentFramePoint.z });
			/*constant*/ b = Vector2Distance(Vector2{inGizmoCenterPos.y, inGizmoCenterPos.z}, Vector2{ worldEditor.firstFramePoint.y, worldEditor.firstFramePoint.z });
			c = Vector2Distance(Vector2{ inGizmoCenterPos.y, inGizmoCenterPos.z }, Vector2{ currentFramePoint.y, currentFramePoint.z });

			std::string a1 = std::to_string(a);
			std::string b1 = std::to_string(b);
			std::string c1 = std::to_string(c);

			cosAlpha = ((c * c) - (a * a) - (b * b)) / ((-2) * a * b);
			alpha = RAD2DEG*acosf(cosAlpha);

			std::string alpha1 = std::to_string(alpha);
			//Vector3 newPos{ 0.0f, 0.0f, 0.0f };			
			//newPos = Vector3{ enttToMove->enttTransform.pos.x - diff, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z };
			//enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);
			

			EndMode3D();

			
			DrawText(a1.c_str(), 150.0f, 150.0f, 24, WHITE);
			DrawText(b1.c_str(), 150.0f, 174.0f, 24, WHITE);
			DrawText(c1.c_str(), 150.0f, 198.0f, 24, WHITE);
			DrawText(alpha1.c_str(), 150.0f, 222.0f, 24, WHITE);
			EndDrawing();
		}
		


	}

	canMoveCamera = true;

}


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
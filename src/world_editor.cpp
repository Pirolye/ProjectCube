#include "world.h"

;
#include "raylib.h"

#include "raymath.h"

#include "rlimgui.h"

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
	entt_camera* nextCam = NULL;
	
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr)
			{
				nextCam = dynamic_cast<entt_camera*>(entityArray[i]);
				if (nextCam->isForEditorOnly == true) 
				{ 
					currentlyRenderingCamera = nextCam; 
					break; 
				}
				else
				{
					nextCam = NULL;
					continue;
				}

				break;
			}
		}
	}

	if (nextCam == NULL) printf("ERROR: Couldn't find a camera marked <<isForEditorOnly>> in world <<%s>> when entering the editor, so we didn't set the editor camera!\n", name.c_str());

	worldEditor.isInEditorMode = true;


}

void world::exit_editor_mode()
{
	worldEditor.isInEditorMode = false;

	entt_camera* nextCam = NULL;

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_camera*>(entityArray[i]))
			{
				nextCam = dynamic_cast<entt_camera*>(entityArray[i]);
				if (nextCam->isForEditorOnly == false) { currentlyRenderingCamera = nextCam; break; }
				else
				{
					nextCam = NULL;
					continue;
				}

				break;
			}
		}
	}

	if (nextCam == NULL) printf("ERROR: Couldn't find a camera marked <<!isForEditorOnly>> in world <<%s>> when exiting the editor, so we didn't set the gameplay camera!\n", name.c_str());


	worldEditor.currentlySelectedEntt = nullptr;

	// START WORLD AGAIN FROM THE BEGINNING
}

void world::editor_next_camera()
{
	int numOfCam = 0;
	
	entt_camera* camArray[64] = { NULL };

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_camera*>(entityArray[i]) && dynamic_cast<entt_camera*>(entityArray[i])->isForEditorOnly)
			{
				camArray[numOfCam] = dynamic_cast<entt_camera*>(entityArray[i]);
				numOfCam++;
			}
		}
	}

	if (camArray[0] == NULL) return;

	for (int i = 0; i != 64; i++)
	{
		if (camArray[i] != NULL)
		{
			if (camArray[i] == currentlyRenderingCamera)
			{
				if (camArray[i + 1] == NULL) currentlyRenderingCamera = camArray[0];
				else currentlyRenderingCamera = camArray[i + 1];

				break; 
			}
		}
	}

}

void world::init_world_editor()
{
	rlImGuiSetup(true);
	
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

void world::shutdown_world_editor()
{
	rlImGuiShutdown();
}

void world::update_world_editor()
{
	// If we are hovering any ImGui windows then we can't edit the world, however, when we are editing, we don't want ImGui to do its IO
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().NavVisible)
	{
		worldEditor.canManipulateWorld = false;
	}
	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().NavVisible)
	{
		worldEditor.canManipulateWorld = true;
		
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			ImGui::GetStyle().Alpha = 0.5f;
		}
		else
		{
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			ImGui::GetStyle().Alpha = 1.0f;

		}

	}
	
	if (worldEditor.currentlySelectedEntt != nullptr) worldEditor.selectingEntt = true; else worldEditor.selectingEntt = false;
	
	if (worldEditor.selectingEntt && worldEditor.canManipulateWorld)
	{
		if(worldEditor.currentGizmoMode == 0) editor_check_against_move_gizmo(worldEditor.currentlySelectedEntt->transform.pos);
		if(worldEditor.currentGizmoMode == 1) editor_check_against_rotate_gizmo(worldEditor.currentlySelectedEntt->transform.pos);

	}
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && worldEditor.canManipulateWorld) editor_try_select_entt();

	if (IsKeyPressed(KEY_TAB) && worldEditor.canManipulateWorld) editor_next_camera();
	
	if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && worldEditor.selectingEntt && worldEditor.canManipulateWorld)
	{
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
	}

}

void world::draw_world_editor_3d()
{
	//(Levente): This will draw the model used to calculate the new position of the actor based on the raycasts. 
	//DrawModel(worldEditor.editorGizmoHelperModel, Vector3Zero(), 1.0f, RED);
	
	
	if(worldEditor.selectingEntt) editor_draw_gizmo(worldEditor.currentlySelectedEntt->transform.pos);

}

void world::draw_world_editor_2d()
{
	rlImGuiBegin();

	bool open;
	ImGui::ShowDemoWindow(&open);



	rlImGuiEnd();
}


void world::editor_move_entt(int axis, float val)
{
	if (worldEditor.currentlySelectedEntt == nullptr) return;

	if (axis == 0)
	{
		entt_transform t = worldEditor.currentlySelectedEntt->transform;
		worldEditor.currentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x + val, t.pos.y, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 1)
	{
		entt_transform t = worldEditor.currentlySelectedEntt->transform;
		worldEditor.currentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y + val, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 2)
	{
		entt_transform t = worldEditor.currentlySelectedEntt->transform;
		worldEditor.currentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y, t.pos.z + val }, t.scale, t.rot);

	}

}

void world::editor_rotate_entt(int axis, float val)
{
}



void world::editor_try_select_entt()
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), currentlyRenderingCamera->rayCam);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_light*>(entityArray[i]) != nullptr) worldEditor.currentlySelectedEntt = dynamic_cast<entt_light*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube*>(entityArray[i]) != nullptr) worldEditor.currentlySelectedEntt = dynamic_cast<entt_maincube*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube_static*>(entityArray[i]) != nullptr) worldEditor.currentlySelectedEntt = dynamic_cast<entt_maincube_static*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr) worldEditor.currentlySelectedEntt = dynamic_cast<entt_camera*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);

			if (worldEditor.currentlySelectedEntt != nullptr) { break; worldEditor.selectingEntt = true; }
			else { worldEditor.selectingEntt = false;  continue; }
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
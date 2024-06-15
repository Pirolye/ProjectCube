#include <typeinfo>

#include "world.h"

;
#include "raylib.h"

#include "raymath.h"

#include "rlimgui.h"

#include "game_instance.h"



#ifdef DEBUG

bool editor_is_selecting_any_gizmo(world_editor* inEditor)
{
	if (inEditor->selectingGizmoMoveAxisX
		|| inEditor->selectingGizmoMoveAxisY
		|| inEditor->selectingGizmoMoveAxisZ
		|| inEditor->selectingGizmoMoveAxisXY
		|| inEditor->selectingGizmoMoveAxisYZ
		|| inEditor->selectingGizmoMoveAxisZX 
		|| inEditor->selectingGizmoRotateAxisX 
		|| inEditor->selectingGizmoRotateAxisY 
		|| inEditor->selectingGizmoRotateAxisZ) 
	{
		return true;
	}
	else
	{
		return false;
	}

}

void editor_do_not_select_any_gizmo(world_editor* inEditor)
{
	inEditor->selectingGizmoMoveAxisX = false;
	inEditor->selectingGizmoMoveAxisY = false;
	inEditor->selectingGizmoMoveAxisZ = false;
	inEditor->selectingGizmoMoveAxisXY = false;
	inEditor->selectingGizmoMoveAxisYZ = false;
	inEditor->selectingGizmoMoveAxisZX = false;
	inEditor->selectingGizmoRotateAxisX = false;
	inEditor->selectingGizmoRotateAxisY = false;
	inEditor->selectingGizmoRotateAxisZ = false;
}

void enter_editor_mode(world_editor* inEditor)
{
	entt_camera* nextCam = NULL;
	
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->entityArray[i] != NULL)
		{
			if (typeid(entt_camera) == *inEditor->currentWorld->entityArray[i]->type)
			{
				nextCam = reinterpret_cast<entt_camera*>(inEditor->currentWorld->entityArray[i]->pointer);
				if (nextCam->isForEditorOnly == true) 
				{ 
					inEditor->currentWorld->currentlyRenderingCamera = nextCam; 
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

	if (nextCam == NULL) printf("ERROR: Couldn't find a camera marked <<isForEditorOnly>> in world <<%s>> when entering the editor, so we didn't set the editor camera!\n", inEditor->currentWorld->name.c_str());

	inEditor->isInEditorMode = true;
	


}

void exit_editor_mode(world_editor* inEditor)
{
	inEditor->isInEditorMode = false;

	entt_camera* nextCam = NULL;

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->entityArray[i] != NULL)
		{
			if (typeid(entt_camera) == *inEditor->currentWorld->entityArray[i]->type)
			{
				nextCam = reinterpret_cast<entt_camera*>(inEditor->currentWorld->entityArray[i]->pointer);
				if (nextCam->isForEditorOnly == false) { inEditor->currentWorld->currentlyRenderingCamera = nextCam; break; }
				else
				{
					nextCam = NULL;
					continue;
				}

				break;
			}
		}
	}

	if (nextCam == NULL) printf("ERROR: Couldn't find a camera not marked isForEditorOnly in world <<%s>> when exiting the editor, so we didn't set the gameplay camera!\n", inEditor->currentWorld->name.c_str());


	inEditor->currentlySelectedEntt = nullptr;

	// START WORLD AGAIN FROM THE BEGINNING
}

void editor_next_camera(world_editor* inEditor)
{
	int numOfCam = 0;
	
	entt_camera* camArray[64] = { NULL }; 

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->entityArray[i] != NULL)
		{
			if (typeid(entt_camera) == typeid(inEditor->currentWorld->entityArray[i]->type))
			{
				entt_camera* cam = reinterpret_cast<entt_camera*>(inEditor->currentWorld->entityArray[i]->pointer);

				if (cam->isForEditorOnly)
				{
					camArray[numOfCam] = cam;
					numOfCam++;

				}
			}
			
		}
	}

	if (camArray[0] == NULL) return;

	for (int i = 0; i != 64; i++)
	{
		if (camArray[i] != NULL)
		{
			if (camArray[i] == inEditor->currentWorld->currentlyRenderingCamera)
			{
				if (camArray[i + 1] == NULL) inEditor->currentWorld->currentlyRenderingCamera = camArray[0];
				else inEditor->currentWorld->currentlyRenderingCamera = camArray[i + 1];

				break; 
			}
		}
	}

	//Don't delete, just set to empty!
	for (int i = 0; i != 64; i++)
	{
		camArray[i] = NULL;
	}

}

void init_world_editor(world_editor* inEditor, world* inCurrentWorld)
{
	inEditor->currentWorld = inCurrentWorld;
	inEditor->editorUI = new world_editor_ui;
	editor_ui_init(inEditor->editorUI, inEditor);
	
	rlImGuiSetup(true);
	
	inEditor->editorGizmoAxisMat = LoadTexture("editor/gizmo_move_axis_albedo.png");

	inEditor->editorGizmoMoveAxisX = LoadModel("editor/gizmo_move_axis.obj");
	inEditor->editorGizmoMoveAxisY = LoadModel("editor/gizmo_move_axis.obj");
	inEditor->editorGizmoMoveAxisZ = LoadModel("editor/gizmo_move_axis.obj");
	inEditor->editorGizmoMoveAxisXY = LoadModel("editor/gizmo_move_axis_combined.obj");
	inEditor->editorGizmoMoveAxisYZ = LoadModel("editor/gizmo_move_axis_combined.obj");
	inEditor->editorGizmoMoveAxisZX = LoadModel("editor/gizmo_move_axis_combined.obj");
			
	inEditor->editorGizmoRotateAxisX = LoadModel("editor/gizmo_rotate_axis.obj");
	inEditor->editorGizmoRotateAxisY = LoadModel("editor/gizmo_rotate_axis.obj");
	inEditor->editorGizmoRotateAxisZ = LoadModel("editor/gizmo_rotate_axis.obj");
	inEditor->editorGizmoRotateAxisX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
	inEditor->editorGizmoRotateAxisY.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
	inEditor->editorGizmoRotateAxisZ.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
			
	inEditor->editorGizmoMoveAxisX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
	inEditor->editorGizmoMoveAxisY.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
	inEditor->editorGizmoMoveAxisZ.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
			
	inEditor->editorGizmoMoveAxisXY.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
	inEditor->editorGizmoMoveAxisYZ.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
	inEditor->editorGizmoMoveAxisZX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->editorGizmoAxisMat;
			
	inEditor->editorGizmoHelperMesh = GenMeshPlane(99999.0f, 99999.0f, 10, 10);
	inEditor->editorGizmoHelperModel = LoadModelFromMesh(inEditor->editorGizmoHelperMesh);

}

void shutdown_world_editor(world_editor* inEditor)
{
	rlImGuiShutdown();
}

void update_world_editor(world_editor* inEditor)
{
	// If we are hovering any ImGui windows then we can't edit the world, however, when we are editing, we don't want ImGui to do its IO
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().NavVisible)
	{
		inEditor->canManipulateWorld = false;
	}
	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().NavVisible)
	{
		inEditor->canManipulateWorld = true;
		
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
	
	if (inEditor->currentlySelectedEntt != nullptr) inEditor->selectingEntt = true; else inEditor->selectingEntt = false;
	
	if (inEditor->selectingEntt && inEditor->canManipulateWorld)
	{
		if(inEditor->currentGizmoMode == 0) editor_check_against_move_gizmo(inEditor, inEditor->currentlySelectedEntt->transform.pos);
		if(inEditor->currentGizmoMode == 1) editor_check_against_rotate_gizmo(inEditor, inEditor->currentlySelectedEntt->transform.pos);

	}
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && inEditor->canManipulateWorld) editor_try_select_entt(inEditor);

	if (IsKeyPressed(KEY_TAB) && inEditor->canManipulateWorld) editor_next_camera(inEditor);
	
	if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && inEditor->selectingEntt && inEditor->canManipulateWorld)
	{
		// Set gizmo modes
		if(!editor_is_selecting_any_gizmo(inEditor)) 
		{
			if (IsKeyPressed(KEY_W))
			{
				inEditor->currentGizmoMode = 0;
			}
			if (IsKeyPressed(KEY_E))
			{
				inEditor->currentGizmoMode = 1;
			}
			if (IsKeyPressed(KEY_R))
			{
				inEditor->currentGizmoMode = 2;
			}
		}
	}

}

void draw_world_editor_3d(world_editor* inEditor)
{
	//(Levente): This will draw the model used to calculate the new position of the actor based on the raycasts. 
	//DrawModel(inEditor.editorGizmoHelperModel, Vector3Zero(), 1.0f, RED);
	
	
	if(inEditor->selectingEntt) editor_draw_gizmo(inEditor, inEditor->currentlySelectedEntt->transform.pos);

}

void draw_world_editor_2d(world_editor* inEditor)
{
	rlImGuiBegin();

	editor_setup_colors(inEditor->editorUI);

	editor_draw_main_menu(inEditor->editorUI);

	editor_draw_entt_panel(inEditor->editorUI);

	if (inEditor->editorUI->ImGuiStylerOpen)
	{
		ImGui::ShowStyleEditor(&(ImGui::GetStyle()));
	}

	//static bool open = true;
	//ImGui::ShowDemoWindow(&open);

	rlImGuiEnd();
}

// LEGACY
/*
void editor_move_entt(int axis, float val)
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

void editor_rotate_entt(int axis, float val)
{
}

*/


void editor_try_select_entt(world_editor* inEditor)
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->entityArray[i] != NULL)
		{
			/*
			const type_info* enttType = &typeid(inEditor->currentWorld->entityArray[i]->type);
			const char* typeName = enttType->name();
			editor_try_select(reinterpret_cast<typeName>inEditor->currentWorld->entityArray[i]
			*/


			if (*inEditor->currentWorld->entityArray[i]->type == typeid(entt_light))
			{
				inEditor->currentlySelectedEntt = editor_try_select(reinterpret_cast<entt_light*>(inEditor->currentWorld->entityArray[i]->pointer));
			}
			if (*inEditor->currentWorld->entityArray[i]->type == typeid(entt_camera))
			{
				inEditor->currentlySelectedEntt = editor_try_select(reinterpret_cast<entt_camera*>(inEditor->currentWorld->entityArray[i]->pointer));
			}
			if (*inEditor->currentWorld->entityArray[i]->type == typeid(entt_maincube))
			{
				inEditor->currentlySelectedEntt = editor_try_select(reinterpret_cast<entt_maincube*>(inEditor->currentWorld->entityArray[i]->pointer));
			}
			if (*inEditor->currentWorld->entityArray[i]->type == typeid(entt_maincube_static))
			{
				inEditor->currentlySelectedEntt = editor_try_select(reinterpret_cast<entt_maincube_static*>(inEditor->currentWorld->entityArray[i]->pointer));
			}

			if (inEditor->currentlySelectedEntt != nullptr) { break; inEditor->selectingEntt = true; }
			else { inEditor->selectingEntt = false;  continue; }
			
		}
	}
};

// -------- UI --------


#else

void editor_move_entt(int axis, float val) {};
void enter_editor_mode() {};
void exit_editor_mode() {};
void init_world_editor() {};
void draw_world_editor_2d() {};
void draw_world_editor_3d() {};
void update_world_editor() {};
void editor_try_select_entt() {};
void editor_draw_gizmo(Vector3 i) {};
void editor_check_against_gizmo() {};


#endif
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
	entity_camera_data* nextCam = NULL;
	
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->entityArray[i] != NULL)
		{
			if ("camera" == inEditor->currentWorld->entityArray[i]->type)
			{
				nextCam = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->entityArray[i]->data);
				if (nextCam->isForEditorOnly == true) 
				{ 
					inEditor->currentWorld->currentlyRenderingCamera = inEditor->currentWorld->entityArray[i];
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

	entity_camera_data* nextCam = NULL;

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->entityArray[i] != NULL)
		{
			if ("camera" == inEditor->currentWorld->entityArray[i]->type)
			{
				nextCam = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->entityArray[i]->data);
				if (nextCam->isForEditorOnly == false) { inEditor->currentWorld->currentlyRenderingCamera = inEditor->currentWorld->entityArray[i]; break; }
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


	inEditor->currentlySelectedEntity = nullptr;

	// START WORLD AGAIN FROM THE BEGINNING
}

void editor_next_camera(world_editor* inEditor)
{
	int numOfCam = 0;
	
	entity* camArray[64] = { NULL }; 

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->entityArray[i] != NULL)
		{
			if ("camera" == inEditor->currentWorld->entityArray[i]->type)
			{
				entity_camera_data* camData = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->entityArray[i]->data);

				if (camData->isForEditorOnly)
				{
					camArray[numOfCam] = inEditor->currentWorld->entityArray[i];
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
	inEditor->isInEditorMode = false;
	inEditor->moveGizmo = new gizmo;
	inEditor->scaleGizmo = new gizmo;
	inEditor->rotateGizmo = new gizmo;

	rlImGuiSetup(true);
	
	for (int i = 0; i < 6; i++)
	{
		inEditor->moveGizmo->model[i] = new gizmo_model;
		inEditor->moveGizmo->model[i]->isSelected = false;
		
		inEditor->moveGizmo->model[i]->texture = LoadTexture("editor/gizmo_move_axis_albedo.png");
		
		if (i == 0 || i == 1 || i == 2)
		{
			inEditor->moveGizmo->model[i]->model = LoadModel("editor/gizmo_move_axis.obj");

		}
		else
		{
			inEditor->moveGizmo->model[i]->model = LoadModel("editor/gizmo_move_axis_combined.obj");
		}

		inEditor->moveGizmo->model[i]->model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->moveGizmo->model[i]->texture;

		inEditor->moveGizmo->model[i]->axis = i;

		if (i == 0)
		{
			inEditor->moveGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		}
		if (i == 1)
		{
			inEditor->moveGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		}
		if (i == 2)
		{
			inEditor->moveGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f, 0.0f });
		}

	}
	
	for (int i = 0; i < 6; i++)
	{
		inEditor->scaleGizmo->model[i] = new gizmo_model;
		inEditor->scaleGizmo->model[i]->isSelected = false;

		inEditor->scaleGizmo->model[i]->texture = LoadTexture("editor/gizmo_move_axis_albedo.png");

		if (i == 0 || i == 1 || i == 2)
		{
			inEditor->scaleGizmo->model[i]->model = LoadModel("editor/gizmo_scale_axis.obj");

		}
		else
		{
			inEditor->scaleGizmo->model[i]->model = LoadModel("editor/gizmo_scale_axis_combined.obj");
		}

		inEditor->scaleGizmo->model[i]->model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->scaleGizmo->model[i]->texture;

		inEditor->scaleGizmo->model[i]->axis = i;

		if (i == 0)
		{
			inEditor->scaleGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		}
		if (i == 1)
		{
			inEditor->scaleGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		}
		if (i == 2)
		{
			inEditor->scaleGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f, 0.0f });
		}

	}

	for (int i = 0; i < 6; i++)
	{
		inEditor->rotateGizmo->model[i] = new gizmo_model;
		inEditor->rotateGizmo->model[i]->isSelected = false;

		inEditor->rotateGizmo->model[i]->texture = LoadTexture("editor/gizmo_move_axis_albedo.png");

		if (i == 0 || i == 1 || i == 2)
		{
			inEditor->rotateGizmo->model[i]->model = LoadModel("editor/gizmo_rotate_axis.obj");

		}
		else
		{
			inEditor->rotateGizmo->model[i]->model = LoadModel("editor/gizmo_rotate_axis_combined.obj");
		}

		inEditor->rotateGizmo->model[i]->model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inEditor->rotateGizmo->model[i]->texture;

		inEditor->rotateGizmo->model[i]->axis = i;

		if (i == 0)
		{
			inEditor->rotateGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		}
		if (i == 1)
		{
			inEditor->rotateGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		}
		if (i == 2)
		{
			inEditor->rotateGizmo->model[i]->helperModelRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f, 0.0f });
		}

	}

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
	
	if (inEditor->currentlySelectedEntity != nullptr) inEditor->selectingEntt = true; else inEditor->selectingEntt = false;
	
	if (inEditor->selectingEntt && inEditor->canManipulateWorld)
	{
		if(inEditor->currentGizmoMode == 0) editor_check_against_move_gizmo(inEditor, inEditor->currentlySelectedEntity->transform.pos);
		if (inEditor->currentGizmoMode == 1) editor_check_against_scale_gizmo(inEditor, inEditor->currentlySelectedEntity->transform.pos);
		if (inEditor->currentGizmoMode == 2) editor_check_against_rotate_gizmo(inEditor, inEditor->currentlySelectedEntity->transform.pos);

	}
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && inEditor->canManipulateWorld) { inEditor->currentlySelectedEntity = editor_try_select_entity(inEditor); };

	if (inEditor->currentlySelectedEntity != nullptr) { inEditor->selectingEntt = true; }
	else { inEditor->selectingEntt = false; }


	if (IsKeyPressed(KEY_TAB) && inEditor->canManipulateWorld) editor_next_camera(inEditor);

	if (IsKeyPressed(KEY_G) && inEditor->canManipulateWorld) world_make_desired_entity_runtime("maincube", inEditor->currentWorld);
	if (IsKeyPressed(KEY_H) && inEditor->canManipulateWorld) world_make_desired_entity_runtime("maincube_static", inEditor->currentWorld);
	
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
	
	
	if(inEditor->selectingEntt) editor_draw_gizmo(inEditor, inEditor->currentlySelectedEntity->transform.pos);

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


entity* editor_try_select_entity(world_editor* inEditor)
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->currentlyRenderingCamera->data);
	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (inEditor->currentWorld->worldEditor->visibilityArray[i] != NULL)
		{

			RayCollision meshHitInfo = { 0 };
			for (int m = 0; m < inEditor->currentWorld->worldEditor->visibilityArray[i]->model.meshCount; m++)
			{
				meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->currentWorld->worldEditor->visibilityArray[i]->model.meshes[m], inEditor->currentWorld->worldEditor->visibilityArray[i]->model.transform);
				if (meshHitInfo.hit)
				{
					return inEditor->currentWorld->worldEditor->visibilityArray[i]->container;

				}
			}

			
		}
	}
	
	return nullptr;
};

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
#ifndef _H_WORLD_EDITOR_UI_
#define _H_WORLD_EDITOR_UI_

#include <string>

//#include "world_editor.h"

struct world_editor;


struct world_editor_ui
{
	world_editor* worldEditor;
	
	bool enttPanelOpen = false;
	bool ImGuiStylerOpen = false;
	transform localT = {};

	std::string currentEntityType;
	std::string entityTypes[3];
	int currentEntitySelectorIndex = 0;

};

void editor_ui_init(world_editor_ui* inEditorUI, world_editor* inWorldEditor);

void editor_setup_colors(world_editor_ui* inWorldEditorUI);
void entt_panel_draw_vec3(world_editor_ui* inWorldEditorUI, Vector3* inVec3, char* inName);
void entt_panel_draw_quat_and_calculate_vec3(world_editor_ui* inWorldEditorUI, graphene_quaternion_t* inQuat, Vector3 inVecToModify, char* inName);
void editor_draw_entt_panel(world_editor_ui* inWorldEditorUI);
void editor_draw_main_menu(world_editor_ui* inWorldEditorUI);
void editor_draw_entity_maker_panel(world_editor_ui* inEditorUI);

#else

#endif
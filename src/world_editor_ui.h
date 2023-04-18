#ifndef _H_WORLD_EDITOR_UI_
#define _H_WORLD_EDITOR_UI_

#include "world.h"
#include "world_editor.h"

;
struct world_editor_ui
{
	world_editor* worldEditor;
	
	bool enttPanelOpen = false;
	bool ImGuiStylerOpen = false;
	entt_transform localT = {};

};

void editor_ui_init(world_editor_ui* inEditorUI, world_editor* inWorldEditor);

void editor_setup_colors(world_editor_ui* inWorldEditorUI);
void editor_entt_panel_draw_vec3(world_editor_ui* inWorldEditorUI, Vector3* inVec3, char* inName);
void editor_entt_panel_draw_quat(world_editor_ui* inWorldEditorUI, graphene_quaternion_t* inQuat, char* inName);
void editor_draw_entt_panel(world_editor_ui* inWorldEditorUI);
void editor_draw_main_menu(world_editor_ui* inWorldEditorUI);

#else

#endif
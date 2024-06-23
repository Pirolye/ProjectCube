#include "world.h"
#include "imgui.h"

;

void editor_ui_init(world_editor_ui* inEditorUI, world_editor* inWorldEditor)
{
	inEditorUI->worldEditor = inWorldEditor;
	inEditorUI->worldEditor->currentlySelectedEntity = nullptr;
	inEditorUI->currentEntityType = "maincube";

	inEditorUI->entityTypes[0] = "maincube";
	inEditorUI->entityTypes[1] = "maincube_static";
	inEditorUI->entityTypes[2] = "light";

}


void entt_panel_draw_vec3(world_editor_ui* inEditorUI, Vector3* inVec3, char* inName)
{
	if (ImGui::CollapsingHeader(inName))
	{
		
		ImGui::SameLine(); ImGui::TextDisabled("Vector3");
		ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV); 

		ImGui::TableNextColumn();

		ImGui::Text("x");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("x", &(inVec3->x), 1.0f, FLT_MAX, -FLT_MAX);

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("y");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("y", &(inVec3->y), 1.0f, FLT_MAX, -FLT_MAX);

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("z");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("z", &(inVec3->z), 1.0f, FLT_MAX, -FLT_MAX);

		ImGui::EndTable();

	}
	else
	{
		ImGui::SameLine(); ImGui::TextDisabled("Vector3");

	}

};

void entt_panel_draw_quat_and_calculate_vec3(world_editor_ui* inEditorUI, graphene_quaternion_t* inQuat, Vector3 inVecToModify, char* inName)
{
	graphene_vec4_t* v4 = graphene_vec4_alloc(); graphene_quaternion_to_vec4(inQuat, v4);

	graphene_quaternion_to_angles(inQuat, &inVecToModify.x, &inVecToModify.y, &inVecToModify.z);

	float x = graphene_vec4_get_x(v4);
	float y = graphene_vec4_get_y(v4);
	float z = graphene_vec4_get_z(v4);
	float w = graphene_vec4_get_w(v4);
	
	std::string x1 = std::to_string(x);
	std::string y1 = std::to_string(y);
	std::string z1 = std::to_string(z);
	std::string w1 = std::to_string(w);

	
	if (ImGui::CollapsingHeader(inName))
	{

		ImGui::SameLine(); ImGui::TextDisabled("graphene_quaternion_t*");
		ImGui::NewLine(); ImGui::Text("Values of a quaternion cannot be modified manually!");
		ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV);

		ImGui::TableNextColumn();

		ImGui::Text("x");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		//ImGui::DragFloat("x", &(inVec3->x), 1.0f, FLT_MAX, -FLT_MAX);
		ImGui::Text(x1.c_str());

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("y");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		//ImGui::DragFloat("y", &(inVec3->y), 1.0f, FLT_MAX, -FLT_MAX);
		ImGui::Text(y1.c_str());

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("z");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		//ImGui::DragFloat("z", &(inVec3->z), 1.0f, FLT_MAX, -FLT_MAX);
		ImGui::Text(z1.c_str());

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("w");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		//ImGui::DragFloat("w", &(inVec3->z), 1.0f, FLT_MAX, -FLT_MAX);
		ImGui::Text(w1.c_str());

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("euler x");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("Vec3.x", &(inVecToModify.x), 1.0f, FLT_MAX, -FLT_MAX);

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("euler y");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("Vec3.y", &(inVecToModify.y), 1.0f, FLT_MAX, -FLT_MAX);

		ImGui::TableNextRow(); ImGui::TableNextColumn();
		ImGui::Text("euler z");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("Vec3.z", &(inVecToModify.z), 1.0f, FLT_MAX, -FLT_MAX);

		graphene_quaternion_init_from_angles(inQuat, inVecToModify.x, inVecToModify.y, inVecToModify.z);

		ImGui::EndTable();

	}
	else
	{
		ImGui::SameLine(); ImGui::TextDisabled("graphene_quaternion_t*");

	}


	graphene_vec4_free(v4);

}

void editor_draw_entt_panel(world_editor_ui* inEditorUI)
{
	if (!inEditorUI->enttPanelOpen) return;

	std::string title;

	if (inEditorUI->worldEditor->currentlySelectedEntity == nullptr)
	{
		title = "viewing nullptr";

		ImGui::Begin(title.c_str(), &inEditorUI->enttPanelOpen, 0);
		ImGui::Text("No entity selected :(");
		ImGui::End();
		return;
	}

	title = "viewing " + inEditorUI->worldEditor->currentlySelectedEntity->id;

	inEditorUI->localT = inEditorUI->worldEditor->currentlySelectedEntity->transform;

	ImGui::Begin(title.c_str(), &inEditorUI->enttPanelOpen, 0);


	if (ImGui::TreeNode("transform"))
	{
		ImGui::SameLine(); ImGui::TextDisabled("transform");
		entt_panel_draw_vec3(inEditorUI, &inEditorUI->localT.pos, "position");
		entt_panel_draw_vec3(inEditorUI, &inEditorUI->localT.scale, "scale");
		entt_panel_draw_quat_and_calculate_vec3(inEditorUI, inEditorUI->localT.rot, inEditorUI->localT.euler, "rotation");
		ImGui::TreePop();
	}
	else
	{

		ImGui::SameLine(); ImGui::TextDisabled("transform");
	}

	if (inEditorUI->worldEditor->currentlySelectedEntity->type == "maincube")
	{
		entity_maincube_data* data = (entity_maincube_data*)(inEditorUI->worldEditor->currentlySelectedEntity->data);
		
		if (ImGui::TreeNode("entity_maincube_data*"))
		{
			ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV);

			ImGui::TableNextColumn();

			ImGui::Text("noPhysics");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::Checkbox("noPhysics", &data->noPhysics);

			ImGui::EndTable();
			ImGui::TreePop();
		}

	}

	update_spatial_properties(inEditorUI->worldEditor->currentlySelectedEntity, inEditorUI->localT.pos, inEditorUI->localT.scale, inEditorUI->localT.rot);


	ImGui::End();


}

void editor_draw_main_menu(world_editor_ui* inEditorUI)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Entity properties", NULL, &inEditorUI->enttPanelOpen);
			ImGui::MenuItem("ImGui styler", NULL, &inEditorUI->ImGuiStylerOpen);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void editor_draw_entity_maker_panel(world_editor_ui* inEditorUI)
{
	bool t = true;

	ImGui::Begin("Make entity", &t, 2 | 64 | 32);
	
	if (ImGui::ArrowButton("b", 0))
	{
		if (inEditorUI->currentEntitySelectorIndex == 0)
		{
			inEditorUI->currentEntityType = inEditorUI->entityTypes[2];
			inEditorUI->currentEntitySelectorIndex = 2;
		}
		else
		{
			inEditorUI->currentEntitySelectorIndex = inEditorUI->currentEntitySelectorIndex - 1;
			inEditorUI->currentEntityType = inEditorUI->entityTypes[inEditorUI->currentEntitySelectorIndex];

		}
		
	}

	ImGui::SameLine();
	ImGui::Text(inEditorUI->currentEntityType.c_str());
	
	ImGui::SameLine();
	if (ImGui::ArrowButton("a", 1))
	{
		if (inEditorUI->currentEntitySelectorIndex == 2)
		{
			inEditorUI->currentEntityType = inEditorUI->entityTypes[0];
			inEditorUI->currentEntitySelectorIndex = 0;

		}

		else
		{
			inEditorUI->currentEntitySelectorIndex = inEditorUI->currentEntitySelectorIndex + 1;
			inEditorUI->currentEntityType = inEditorUI->entityTypes[inEditorUI->currentEntitySelectorIndex];

		}

	}
	
	ImGui::End();
}

void editor_setup_colors(world_editor_ui* inEditorUI)
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.37f, 0.08f, 0.08f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.05f, 0.15f, 0.20f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.37f, 0.08f, 0.08f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.33f, 0.07f, 0.07f, 0.59f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.33f, 0.07f, 0.07f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.33f, 0.07f, 0.07f, 0.59f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.11f, 0.16f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.44f, 0.74f, 0.24f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.02f, 0.67f, 0.30f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.44f, 0.74f, 0.24f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.02f, 0.67f, 0.30f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.74f, 0.24f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.74f, 0.24f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.47f, 0.09f, 0.08f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.09f, 0.08f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.09f, 0.08f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.89f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.91f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.02f, 0.67f, 0.30f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.44f, 0.74f, 0.24f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.44f, 0.74f, 0.24f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.26f, 0.15f, 0.13f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGuiStyle* style = &(ImGui::GetStyle());
	style->FrameRounding = 12.0f;
	style->WindowRounding = 10.0f;
	style->ChildRounding = 10.0f;
	style->PopupRounding = 10.0f;
	//style->DisplayWindowPadding = ImVec2(gameInstance->windowWidth - 300, gameInstance->windowHeight - 200);
	style->FrameBorderSize = 0.0f;
	style->WindowBorderSize = 0.0f;
	style->ChildBorderSize = 0.0f;
	style->PopupBorderSize = 0.0f;


}
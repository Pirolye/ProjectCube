#include "world.h"
#include "imgui.h"
;

#define GET_VARIABLE_NAME(Variable) (#Variable)

;
void world_editor_ui::entt_panel_draw_vec3(Vector3* inVec3, char* inName)
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

void world_editor_ui::entt_panel_draw_quat(graphene_quaternion_t* inQuat, char* inName)
{
	graphene_vec4_t* v4 = graphene_vec4_alloc(); graphene_quaternion_to_vec4(inQuat, v4);
	
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

		ImGui::EndTable();

	}
	else
	{
		ImGui::SameLine(); ImGui::TextDisabled("Vector3");

	}

	graphene_vec4_free(v4);

}

void world::editor_draw_entt_panel()
{
	if (!worldEditor.enttPanelOpen) return;

	std::string title;

	if (worldEditor.currentlySelectedEntt == nullptr)
	{
		title = "viewing nullptr";

		ImGui::Begin(title.c_str(), &worldEditor.enttPanelOpen, 0);
		ImGui::Text("No entt selected :(");
		ImGui::End();
		return;
	}

	title = "viewing " + worldEditor.currentlySelectedEntt->id;

	worldEditor.localT = worldEditor.currentlySelectedEntt->transform;

	ImGui::Begin(title.c_str(), &worldEditor.enttPanelOpen, 0);


	/*
	if (ImGui::CollapsingHeader("transform"))
	{
		ImGui::SameLine(); ImGui::TextDisabled("entt_transform");
		if (ImGui::CollapsingHeader("transform.pos"))
		{
			ImGui::SameLine(); ImGui::TextDisabled("Vector3");
			ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV); //|/* ImGuiTableFlags_SizingStretchProp * / );

			ImGui::TableNextColumn();

			ImGui::Text("x");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::DragFloat("x", &worldEditor.localT.pos.x, 1.0f, FLT_MAX, -FLT_MAX);

			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("y");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::DragFloat("y", &worldEditor.localT.pos.y, 1.0f, FLT_MAX, -FLT_MAX);

			ImGui::TableNextRow(); ImGui::TableNextColumn();
			ImGui::Text("z");
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::DragFloat("z", &worldEditor.localT.pos.z, 1.0f, FLT_MAX, -FLT_MAX);

			ImGui::EndTable();

		}
		else
		{
			ImGui::SameLine(); ImGui::TextDisabled("Vector3");

		}
	}
	else
	{
		ImGui::SameLine(); ImGui::TextDisabled("entt_transform");

	}

	*/

	if (ImGui::TreeNode("transform"))
	{
		ImGui::SameLine(); ImGui::TextDisabled("entt_transform");
		worldEditor.worldEditorUI.entt_panel_draw_vec3(&worldEditor.localT.pos, "position");
		worldEditor.worldEditorUI.entt_panel_draw_vec3(&worldEditor.localT.scale, "scale");
		worldEditor.worldEditorUI.entt_panel_draw_quat(worldEditor.localT.rot, "rotation");
		ImGui::TreePop();
	}
	else
	{

		ImGui::SameLine(); ImGui::TextDisabled("entt_transform");
	}


	worldEditor.currentlySelectedEntt->update_spatial_props(worldEditor.localT.pos, worldEditor.localT.scale, worldEditor.localT.rot);





	ImGui::End();


}

void world::editor_draw_main_menu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("View and edit entt properties", NULL, &worldEditor.enttPanelOpen);
			ImGui::MenuItem("ImGui styler", NULL, &worldEditor.ImGuiStylerOpen);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void world_editor_ui::setup_colors()
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
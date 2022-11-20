#include "world.h"

;
#include "raylib.h"

#include "raymath.h"

/*

BIRO: I decided to separate the standard world functions from the editor ones because the main file was getting bloated and hard to navigate.


*/

#ifdef DEBUG

void world::enter_editor_mode()
{
	isInEditorMode = true;
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
	isInEditorMode = false;
	canMoveCamera = false;

	entt_camera* cam = dynamic_cast<entt_camera*>(entityArray[0]); //(Levente): Camera is always at index 0!

	cam->rayCam->position = Vector3{ 0.0f, 0.0f, -5.0f }; // Camera position
	cam->rayCam->target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	cam->rayCam->up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	cam->rayCam->fovy = 70.0f;                                // Camera field-of-view 
	cam->rayCam->projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode((*cam->rayCam), CAMERA_FREE); // Set a free camera mode

	editorCurrentlySelectedEntt = nullptr;

	// START WORLD AGAIN FROM THE BEGINNING
}

void world::update_world_editor()
{
	if (editorCurrentlySelectedEntt != nullptr) editor_check_against_gizmo(editorCurrentlySelectedEntt->enttTransform.pos);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && editorInfo.canSelectEntt) editor_try_select_entt();

	if (IsKeyPressed(KEY_TAB))
	{
		editorCurrentlyEditingAxis = editorCurrentlyEditingAxis + 1;
		if (editorCurrentlyEditingAxis == 3) editorCurrentlyEditingAxis = 0;
	}

	//Will be removed once we got editor gui
	if (editorCurrentlySelectedEntt != NULL)
	{
		DrawText(editorCurrentlySelectedEntt->id.c_str(), 10, 550, 20, RED);

		if (IsKeyDown(KEY_W))
		{
			editor_move_entt(editorCurrentlyEditingAxis, 0.05f);
		}
		if (IsKeyDown(KEY_S))
		{
			editor_move_entt(editorCurrentlyEditingAxis, -0.05f);
		}
		if (IsKeyDown(KEY_A))
		{
			editor_rotate_entt(editorCurrentlyEditingAxis, 1.0f);
		}
		if (IsKeyDown(KEY_D))
		{
			editor_rotate_entt(editorCurrentlyEditingAxis, -1.0f);
		}


	}

}

void world::draw_world_editor_3d()
{
	//(Levente): This will draw the model used to calculate the new position of the actor based on the raycasts. 
	//DrawModel(editorGizmoHelperModel, Vector3Zero(), 1.0f, RED);
	
	
	if(editorCurrentlySelectedEntt != nullptr) editor_draw_gizmo(editorCurrentlySelectedEntt->enttTransform.pos);

}

void world::draw_world_editor_2d()
{
	switch (editorCurrentlyEditingAxis)
	{
	case 0:
		DrawText("x", 0, 100, 30, WHITE);
		break;
	case 1:
		DrawText("y", 0, 100, 30, WHITE);
		break;
	case 2:
		DrawText("z", 0, 100, 30, WHITE);
		break;
	}

	if (editorInfo.selectingGizmoMoveAxisX) DrawText("Moving X gizmo", 0, 150, 30, WHITE);
	if (editorInfo.selectingGizmoMoveAxisY) DrawText("Moving Y gizmo", 0, 150, 30, WHITE);
	if (editorInfo.selectingGizmoMoveAxisZ) DrawText("Moving Z gizmo", 0, 150, 30, WHITE);

}


void world::editor_move_entt(int axis, float val)
{
	if (editorCurrentlySelectedEntt == nullptr) return;

	if (axis == 0)
	{
		entt_transform t = editorCurrentlySelectedEntt->enttTransform;
		editorCurrentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x + val, t.pos.y, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 1)
	{
		entt_transform t = editorCurrentlySelectedEntt->enttTransform;
		editorCurrentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y + val, t.pos.z }, t.scale, t.rot);

	}
	if (axis == 2)
	{
		entt_transform t = editorCurrentlySelectedEntt->enttTransform;
		editorCurrentlySelectedEntt->update_spatial_props(Vector3{ t.pos.x, t.pos.y, t.pos.z + val }, t.scale, t.rot);

	}

}

void world::editor_rotate_entt(int axis, float val)
{
	if (editorCurrentlySelectedEntt == nullptr) return;

	if (axis == 0)
	{
		entt_transform t = editorCurrentlySelectedEntt->enttTransform;

		editorCurrentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x + val, t.rot.y, t.rot.z });

	}
	if (axis == 1)
	{
		entt_transform t = editorCurrentlySelectedEntt->enttTransform;

		editorCurrentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x, t.rot.y + val, t.rot.z });

	}
	if (axis == 2)
	{
		entt_transform t = editorCurrentlySelectedEntt->enttTransform;

		editorCurrentlySelectedEntt->update_spatial_props(t.pos, t.scale, Vector3{ t.rot.x, t.rot.y, t.rot.z + val });

	}

}



void world::editor_try_select_entt()
{
	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;

	cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			if (dynamic_cast<entt_light*>(entityArray[i]) != nullptr) editorCurrentlySelectedEntt = dynamic_cast<entt_light*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube*>(entityArray[i]) != nullptr) editorCurrentlySelectedEntt = dynamic_cast<entt_maincube*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_maincube_static*>(entityArray[i]) != nullptr) editorCurrentlySelectedEntt = dynamic_cast<entt_maincube_static*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr) editorCurrentlySelectedEntt = dynamic_cast<entt_camera*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);

			if (editorCurrentlySelectedEntt != nullptr) break;
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

	Matrix matScaleX = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotationX = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD * 90.0f });
	Matrix matTranslationX = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
	editorGizmoMoveAxisX.transform = MatrixMultiply(MatrixMultiply(matScaleX, matRotationX), matTranslationX);

	Matrix matScaleY = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotationY = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f , 0.0f });
	Matrix matTranslationY = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
	editorGizmoMoveAxisY.transform = MatrixMultiply(MatrixMultiply(matScaleY, matRotationY), matTranslationY);

	Matrix matScaleZ = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotationZ = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f , 0.0f });
	Matrix matTranslationZ = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
	editorGizmoMoveAxisZ.transform = MatrixMultiply(MatrixMultiply(matScaleZ, matRotationZ), matTranslationZ);

	if (editorInfo.selectingGizmoMoveAxisX == false)
	{
		DrawModel(editorGizmoMoveAxisX, Vector3Zero(), 1.0f, RED);
	}
	else
	{
		DrawModel(editorGizmoMoveAxisX, Vector3Zero(), 1.0f, WHITE);
	}

	if (editorInfo.selectingGizmoMoveAxisY == false)
	{
		DrawModel(editorGizmoMoveAxisY, Vector3Zero(), 1.0f, GREEN);
	}
	else
	{
		DrawModel(editorGizmoMoveAxisY, Vector3Zero(), 1.0f, WHITE);
	}

	if (editorInfo.selectingGizmoMoveAxisZ == false)
	{
		DrawModel(editorGizmoMoveAxisZ, Vector3Zero(), 1.0f, BLUE);
	}
	else
	{
		DrawModel(editorGizmoMoveAxisZ, Vector3Zero(), 1.0f, WHITE);
	}

}

void world::editor_check_against_gizmo(Vector3 inCenterPos)
{
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		//if (editorInfo.selectingGizmoMoveAxisY || editorInfo.selectingGizmoMoveAxisZ) return;
		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };
		cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		for (int m = 0; m < editorGizmoMoveAxisX.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, editorGizmoMoveAxisX.meshes[m], editorGizmoMoveAxisX.transform);
			if (meshHitInfo.hit)
			{
				editorInfo.selectingGizmoMoveAxisX = true;
				editorInfo.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && editorInfo.selectingGizmoMoveAxisX) 
				{
					editorInfo.selectingGizmoMoveAxisX = true;
					editorInfo.canSelectEntt = false;

				}
				else
				{
					editorInfo.selectingGizmoMoveAxisX = false;
				}

			}
		}

		//if (editorInfo.selectingGizmoMoveAxisX || editorInfo.selectingGizmoMoveAxisZ) return;
		cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < editorGizmoMoveAxisY.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, editorGizmoMoveAxisY.meshes[m], editorGizmoMoveAxisY.transform);
			if (meshHitInfo.hit)
			{
				editorInfo.selectingGizmoMoveAxisY = true;
				editorInfo.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && editorInfo.selectingGizmoMoveAxisY)
				{
					editorInfo.selectingGizmoMoveAxisY = true;
					editorInfo.canSelectEntt = false;

				}
				else
				{
					editorInfo.selectingGizmoMoveAxisY = false;
				}
					//editorInfo.selectingGizmoMoveAxisY = false;

			}
		}

		//if (editorInfo.selectingGizmoMoveAxisY || editorInfo.selectingGizmoMoveAxisX) return;
		cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < editorGizmoMoveAxisZ.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, editorGizmoMoveAxisZ.meshes[m], editorGizmoMoveAxisZ.transform);
			if (meshHitInfo.hit)
			{
				editorInfo.selectingGizmoMoveAxisZ = true;
				editorInfo.canSelectEntt = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && editorInfo.selectingGizmoMoveAxisZ)
				{
					editorInfo.selectingGizmoMoveAxisZ = true;
					editorInfo.canSelectEntt = false;

				}
				else
				{
					editorInfo.selectingGizmoMoveAxisZ = false;
				}
					//editorInfo.selectingGizmoMoveAxisZ = false;

			}
		}

	}
	else
	{
		editorInfo.selectingGizmoMoveAxisX = false;
		editorInfo.selectingGizmoMoveAxisY = false;
		editorInfo.selectingGizmoMoveAxisZ = false;
		editorInfo.canSelectEntt = true;
	}

	if (editorInfo.selectingGizmoMoveAxisX && editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(0, inCenterPos, editorCurrentlySelectedEntt);
	if (editorInfo.selectingGizmoMoveAxisY && editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(1, inCenterPos, editorCurrentlySelectedEntt);
	if (editorInfo.selectingGizmoMoveAxisZ && editorCurrentlySelectedEntt != nullptr) editor_move_entt_gizmo(2, inCenterPos, editorCurrentlySelectedEntt);


}

void world::editor_move_entt_gizmo(int inAxis, Vector3 inGizmoCenterPos, entt* enttToMove)
{
	canMoveCamera = false;

	//(Levente): When moving, we transform the helper behind the object so that the ray hits it perpendicular to the axis. Eg. On the X axis,
	//			 it won't be position ONTO the axis, but behind it,(since an axis (here: X) is 1D; normally we imagine it as sprawling across
	//           the map, but it's actually a straight line in +x and -x) since logically that's where the mouse's ray end up.
	//
	//			 For each axis, we do 2 casts, to get starting pos and ending pos = small increments = logical.

	if (inAxis == 0)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float currentFramePointX = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float prevFramePointX = 0.0f;

		for (int m = 0; m < editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, editorGizmoHelperModel.meshes[m], editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointX = meshHitInfo.point.x;
				break;
				return;
			}
		}


		for (int m = 0; m < editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, editorGizmoHelperModel.meshes[m], editorGizmoHelperModel.transform);
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

	else if (inAxis == 1)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float currentFramePointY = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float prevFramePointY = 0.0f;

		for (int m = 0; m < editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, editorGizmoHelperModel.meshes[m], editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointY = meshHitInfo.point.y;

				break;
				return;
			}
		}


		for (int m = 0; m < editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, editorGizmoHelperModel.meshes[m], editorGizmoHelperModel.transform);
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
	else if (inAxis == 2)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f, 0.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float currentFramePointZ = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		float prevFramePointZ = 0.0f;

		for (int m = 0; m < editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, editorGizmoHelperModel.meshes[m], editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointZ = meshHitInfo.point.z;

				break;
				return;
			}
		}


		for (int m = 0; m < editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, editorGizmoHelperModel.meshes[m], editorGizmoHelperModel.transform);
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

	canMoveCamera = true;
}

#else

void world::editor_move_entt(int axis, float val) {};
void world::enter_editor_mode() {};
void world::exit_editor_mode() {};
void world::update_world_editor() {};
void world::editor_try_select_entt() {};
void world::editor_draw_gizmo(Vector3 i) {};
void world::editor_check_against_gizmo() {};


#endif
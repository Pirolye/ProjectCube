#include "world.h"
#include "raymath.h"

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
					break;
					return;
				}
				else
				{
					worldEditor.selectingGizmoRotateAxisX = true;
					worldEditor.canSelectEntt = false;

					break;
					return;

				}
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoRotateAxisX)
				{
					worldEditor.selectingGizmoRotateAxisX = true;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					worldEditor.selectingGizmoRotateAxisX = false;
				}

			}
		}

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		meshHitInfo = { 0 };

		for (int m = 0; m < worldEditor.editorGizmoRotateAxisY.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoRotateAxisY.meshes[m], worldEditor.editorGizmoRotateAxisY.transform);

			if (meshHitInfo.hit)
			{
				if (worldEditor.selectingGizmoMoveAxisX == true
					|| worldEditor.selectingGizmoMoveAxisZ == true
					|| worldEditor.selectingGizmoMoveAxisY == true
					|| worldEditor.selectingGizmoMoveAxisXY == true
					|| worldEditor.selectingGizmoMoveAxisYZ == true
					|| worldEditor.selectingGizmoMoveAxisZX == true
					|| worldEditor.selectingGizmoRotateAxisX == true
					|| worldEditor.selectingGizmoRotateAxisZ == true)
				{
					break; return;
				}
				else
				{
					worldEditor.selectingGizmoRotateAxisY = true;
					worldEditor.canSelectEntt = false;

					break; return;

				}
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && worldEditor.selectingGizmoRotateAxisY)
				{
					worldEditor.selectingGizmoRotateAxisY = true;
					worldEditor.canSelectEntt = false;

				}
				else
				{
					worldEditor.selectingGizmoRotateAxisY = false;
				}

			}
		}

	}
	else
	{
		editor_do_not_select_any_gizmo();

		worldEditor.canSelectEntt = true;
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

				newPos = Vector3{ enttToMove->transform.pos.x - diff, enttToMove->transform.pos.y, enttToMove->transform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->transform.scale, enttToMove->transform.rot);
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
				newPos = Vector3{ enttToMove->transform.pos.x, enttToMove->transform.pos.y - diff, enttToMove->transform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->transform.scale, enttToMove->transform.rot);
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

				newPos = Vector3{ enttToMove->transform.pos.x, enttToMove->transform.pos.y, enttToMove->transform.pos.z - diff };

				enttToMove->update_spatial_props(newPos, enttToMove->transform.scale, enttToMove->transform.rot);
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

				newPos = Vector3{ enttToMove->transform.pos.x - diff.x, enttToMove->transform.pos.y - diff.y, enttToMove->transform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->transform.scale, enttToMove->transform.rot);
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

				newPos = Vector3{ enttToMove->transform.pos.x, enttToMove->transform.pos.y - diff.x, enttToMove->transform.pos.z - diff.y };

				enttToMove->update_spatial_props(newPos, enttToMove->transform.scale, enttToMove->transform.rot);
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

				newPos = Vector3{ enttToMove->transform.pos.x - diff.y, enttToMove->transform.pos.y, enttToMove->transform.pos.z - diff.x };

				enttToMove->update_spatial_props(newPos, enttToMove->transform.scale, enttToMove->transform.rot);
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

		//FIRST METHOD

		/*
		if (worldEditor.selectingInPrevFrame == false)
		{

			delete worldEditor.enttRot;

			Vector2 mousePos = Vector2{ GetMousePosition().x, GetMousePosition().y};
			Ray cursorSelectionRayForFirstFrame = GetMouseRay(mousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForFirstFrame, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				worldEditor.firstFramePoint = Vector3{ meshHitInfo.point.x, meshHitInfo.point.y, meshHitInfo.point.z };
				worldEditor.selectingInPrevFrame = true;

				worldEditor.enttRot = new Vector3{ enttToMove->transform.rot.x, enttToMove->transform.rot.y, enttToMove->transform.rot.z };

			}


		}

		worldEditor.cursorSelectionRay = GetMouseRay(GetMousePosition(), *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
		Vector3 currentFramePoint{};

		meshHitInfo = GetRayCollisionMesh(worldEditor.cursorSelectionRay, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			currentFramePoint = Vector3{ meshHitInfo.point.x, meshHitInfo.point.y, meshHitInfo.point.z };

			//BeginDrawing();
			//BeginMode3D( *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

			//DrawCubeV(worldEditor.firstFramePoint, Vector3{ 1.0f, 1.0f, 1.0f, }, BLACK);
			//DrawCubeV(currentFramePoint, Vector3{ 1.0f, 1.0f, 1.0f, }, WHITE);

			float a; // This is between the first frame and the current frame
			float b; // Between the center and the first frame
			float c; // Between the center and the now frame

			float cosAlpha; // Cosine at the vertex closest to the gizmo center
			float alpha; // Angle in deg at the vertex closest to the gizmo center (final applied rotation)

			a = Vector2Distance(Vector2{ worldEditor.firstFramePoint.y, worldEditor.firstFramePoint.z }, Vector2{ currentFramePoint.y, currentFramePoint.z });

			//This should be constant
			b = Vector2Distance(Vector2{inGizmoCenterPos.y, inGizmoCenterPos.z}, Vector2{ worldEditor.firstFramePoint.y, worldEditor.firstFramePoint.z });

			c = Vector2Distance(Vector2{ inGizmoCenterPos.y, inGizmoCenterPos.z }, Vector2{ currentFramePoint.y, currentFramePoint.z });

			std::string a1 = std::to_string(a);
			std::string b1 = std::to_string(b);
			std::string c1 = std::to_string(c);

			cosAlpha = ((c * c) - (a * a) - (b * b)) / ((-2) * a * b);
			alpha = RAD2DEG*acosf(cosAlpha);

			std::string alpha1 = std::to_string(alpha);
			Vector3 newRot{ 0.0f, 0.0f, 0.0f };
			newRot = Vector3{ worldEditor.enttRot->x + alpha, enttToMove->transform.rot.y, enttToMove->transform.rot.z };
			enttToMove->update_spatial_props(enttToMove->transform.pos, enttToMove->transform.scale, newRot);


			//EndMode3D();


			//DrawText(a1.c_str(), 150.0f, 150.0f, 24, WHITE);
			//DrawText(b1.c_str(), 150.0f, 174.0f, 24, WHITE);
			//DrawText(c1.c_str(), 150.0f, 198.0f, 24, WHITE);
			//DrawText(alpha1.c_str(), 150.0f, 222.0f, 24, WHITE);
			//EndDrawing();
		}

		*/

		//SECOND METHOD


		Vector2 mousePos = Vector2{ GetMousePosition().x, GetMousePosition().y };
		Ray cursorSelectionRayCurrentFrame = GetMouseRay(mousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

		Vector2 mousePosPrevFrame = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };
		Ray cursorSelectionRayPrevFrame = GetMouseRay(mousePosPrevFrame, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

		meshHitInfo = GetRayCollisionMesh(cursorSelectionRayCurrentFrame, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			Vector3 currentFramePoint = meshHitInfo.point;

			RayCollision meshHitInfoForPrevFrame = GetRayCollisionMesh(cursorSelectionRayPrevFrame, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);

			if (!meshHitInfoForPrevFrame.hit) return; //THIS MIGHT NOT BE GOOD

			float a; // This is between the first frame and the current frame
			float b; // Between the center and the first frame
			float c; // Between the center and the now frame

			float cosAlpha; // Cosine at the vertex closest to the gizmo center
			float alpha; // Angle in deg at the vertex closest to the gizmo center (final applied rotation)

			a = Vector2Distance(Vector2{ meshHitInfoForPrevFrame.point.y, meshHitInfoForPrevFrame.point.z }, Vector2{ meshHitInfo.point.y, meshHitInfo.point.z });

			//This should be constant
			b = Vector2Distance(Vector2{ inGizmoCenterPos.y, inGizmoCenterPos.z }, Vector2{ meshHitInfoForPrevFrame.point.y, meshHitInfoForPrevFrame.point.z });

			c = Vector2Distance(Vector2{ inGizmoCenterPos.y, inGizmoCenterPos.z }, Vector2{ meshHitInfo.point.y, meshHitInfo.point.z });

			alpha = 0.0f;

			cosAlpha = ((a * a) - (c * c) - (b * b)) / ((-2) * c * b);

			alpha = RAD2DEG * acosf(cosAlpha);

			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				alpha = alpha * -1.0f;
			}

			if (IsKeyDown(KEY_LEFT_CONTROL))
			{
				graphene_quaternion_t* rotInQuat = graphene_quaternion_alloc();
				graphene_quaternion_init_from_angles(rotInQuat, alpha, 0.0f, 0.0f);

				graphene_quaternion_t* finalQ = graphene_quaternion_alloc(); graphene_quaternion_init_identity(finalQ);

				graphene_quaternion_multiply(rotInQuat, enttToMove->transform.rot, finalQ);

				enttToMove->update_spatial_props(enttToMove->transform.pos, enttToMove->transform.scale, finalQ);

				graphene_quaternion_free(rotInQuat);
				graphene_quaternion_free(finalQ);

			}
			else
			{
				graphene_quaternion_t* rotInQuat = graphene_quaternion_alloc();
				graphene_quaternion_init_from_angles(rotInQuat, alpha, 0.0f, 0.0f);

				graphene_quaternion_t* finalQ = graphene_quaternion_alloc(); graphene_quaternion_init_identity(finalQ);

				graphene_quaternion_multiply(enttToMove->transform.rot, rotInQuat, finalQ);

				enttToMove->update_spatial_props(enttToMove->transform.pos, enttToMove->transform.scale, finalQ);

				graphene_quaternion_free(rotInQuat);
				graphene_quaternion_free(finalQ);
			}

		}


	}

	if (inAxis == 1)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, 0.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		worldEditor.editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 mousePos = Vector2{ GetMousePosition().x, GetMousePosition().y };
		Ray cursorSelectionRayCurrentFrame = GetMouseRay(mousePos, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

		Vector2 mousePosPrevFrame = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };
		Ray cursorSelectionRayPrevFrame = GetMouseRay(mousePosPrevFrame, *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));

		meshHitInfo = GetRayCollisionMesh(cursorSelectionRayCurrentFrame, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			Vector3 currentFramePoint = meshHitInfo.point;

			RayCollision meshHitInfoForPrevFrame = GetRayCollisionMesh(cursorSelectionRayPrevFrame, worldEditor.editorGizmoHelperModel.meshes[0], worldEditor.editorGizmoHelperModel.transform);

			if (!meshHitInfoForPrevFrame.hit) return; //THIS MIGHT NOT BE GOOD

			float a; // This is between the first frame and the current frame
			float b; // Between the center and the first frame
			float c; // Between the center and the now frame

			float cosAlpha; // Cosine at the vertex closest to the gizmo center
			float alpha; // Angle in deg at the vertex closest to the gizmo center (final applied rotation)

			a = Vector2Distance(Vector2{ meshHitInfoForPrevFrame.point.x, meshHitInfoForPrevFrame.point.z }, Vector2{ meshHitInfo.point.x, meshHitInfo.point.z });

			//This should be constant
			b = Vector2Distance(Vector2{ inGizmoCenterPos.x, inGizmoCenterPos.z }, Vector2{ meshHitInfoForPrevFrame.point.x, meshHitInfoForPrevFrame.point.z });

			c = Vector2Distance(Vector2{ inGizmoCenterPos.x, inGizmoCenterPos.z }, Vector2{ meshHitInfo.point.x, meshHitInfo.point.z });

			alpha = 0.0f;

			cosAlpha = ((a * a) - (c * c) - (b * b)) / ((-2) * c * b);

			alpha = RAD2DEG * acosf(cosAlpha);

			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				alpha = alpha * -1.0f;
			}

			std::string a1 = std::to_string(alpha);

			BeginDrawing();
			DrawText(a1.c_str(), 150, 150, 24, WHITE);
			EndDrawing();

			graphene_quaternion_t* rotInQuat = graphene_quaternion_alloc();
			graphene_quaternion_init_from_angles(rotInQuat, 0.0f, alpha, 0.0f);

			graphene_quaternion_t* finalQ = graphene_quaternion_alloc(); graphene_quaternion_init_identity(finalQ);

			graphene_quaternion_multiply(enttToMove->transform.rot, rotInQuat, finalQ);

			enttToMove->update_spatial_props(enttToMove->transform.pos, enttToMove->transform.scale, finalQ);

			graphene_quaternion_free(rotInQuat);
			graphene_quaternion_free(finalQ);


		}

		canMoveCamera = true;

	}
}

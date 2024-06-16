#include <typeinfo>
#include "world.h"
#include "raymath.h"

/*

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

void editor_draw_gizmo(world_editor* inEditor, Vector3 inCenterPos)
{
	//(Levente): NOTE: For now, editing a camera's properties using the gizmo handles is disabled. Fix: when I implement global/local transform into the gizmo system!
	if (typeid(entt_camera) == typeid(inEditor->currentlySelectedEntt)) return;

	Vector3 v{ 1.0f, 0.0f, 0.0f };

	if (inEditor->currentGizmoMode == 0)
	{


		Matrix matScaleX = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationX = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD * 90.0f });
		Matrix matTranslationX = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		inEditor->editorGizmoMoveAxisX.transform = MatrixMultiply(MatrixMultiply(matScaleX, matRotationX), matTranslationX);

		Matrix matScaleY = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationY = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f , 0.0f });
		Matrix matTranslationY = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		inEditor->editorGizmoMoveAxisY.transform = MatrixMultiply(MatrixMultiply(matScaleY, matRotationY), matTranslationY);

		Matrix matScaleZ = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationZ = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f , 0.0f });
		Matrix matTranslationZ = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		inEditor->editorGizmoMoveAxisZ.transform = MatrixMultiply(MatrixMultiply(matScaleZ, matRotationZ), matTranslationZ);

		Matrix matScaleXY = MatrixScale(0.8f, 0.8f, 0.8f);
		Matrix matRotationXY = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f, 0.0f });
		Matrix matTranslationXY = MatrixTranslate(inCenterPos.x + 1.5f, inCenterPos.y + 1.5f, inCenterPos.z);
		inEditor->editorGizmoMoveAxisXY.transform = MatrixMultiply(MatrixMultiply(matScaleXY, matRotationXY), matTranslationXY);

		Matrix matScaleYZ = MatrixScale(0.8f, 0.8f, 0.8f);
		Matrix matRotationYZ = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f , DEG2RAD * 90.0f }); //WARNING: When rotating, we change the local coordinate system orientation, so we need to rotate on a different axis to get global coordinate system equivalent results
		Matrix matTranslationYZ = MatrixTranslate(inCenterPos.x, inCenterPos.y + 1.5f, inCenterPos.z - 1.5f);
		inEditor->editorGizmoMoveAxisYZ.transform = MatrixMultiply(MatrixMultiply(matScaleYZ, matRotationYZ), matTranslationYZ);

		Matrix matScaleZX = MatrixScale(0.8f, 0.8f, 0.8f);
		Matrix matRotationZX = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f , 0.0f });
		Matrix matTranslationZX = MatrixTranslate(inCenterPos.x + 1.5f, inCenterPos.y, inCenterPos.z - 1.5f);
		inEditor->editorGizmoMoveAxisZX.transform = MatrixMultiply(MatrixMultiply(matScaleZX, matRotationZX), matTranslationZX);


		if (inEditor->selectingGizmoMoveAxisX == false)
		{
			DrawModel(inEditor->editorGizmoMoveAxisX, Vector3Zero(), 1.0f, RED);
		}
		else
		{
			DrawModel(inEditor->editorGizmoMoveAxisX, Vector3Zero(), 1.0f, WHITE);
		}

		if (inEditor->selectingGizmoMoveAxisY == false)
		{
			DrawModel(inEditor->editorGizmoMoveAxisY, Vector3Zero(), 1.0f, GREEN);
		}
		else
		{
			DrawModel(inEditor->editorGizmoMoveAxisY, Vector3Zero(), 1.0f, WHITE);
		}

		if (inEditor->selectingGizmoMoveAxisZ == false)
		{
			DrawModel(inEditor->editorGizmoMoveAxisZ, Vector3Zero(), 1.0f, BLUE);
		}
		else
		{
			DrawModel(inEditor->editorGizmoMoveAxisZ, Vector3Zero(), 1.0f, WHITE);
		}

		if (inEditor->selectingGizmoMoveAxisXY == false)
		{
			DrawModel(inEditor->editorGizmoMoveAxisXY, Vector3Zero(), 1.0f, ORANGE);
		}
		else
		{
			DrawModel(inEditor->editorGizmoMoveAxisXY, Vector3Zero(), 1.0f, WHITE);
		}

		if (inEditor->selectingGizmoMoveAxisYZ == false)
		{
			DrawModel(inEditor->editorGizmoMoveAxisYZ, Vector3Zero(), 1.0f, SKYBLUE);
		}
		else
		{
			DrawModel(inEditor->editorGizmoMoveAxisYZ, Vector3Zero(), 1.0f, WHITE);
		}

		if (inEditor->selectingGizmoMoveAxisZX == false)
		{
			DrawModel(inEditor->editorGizmoMoveAxisZX, Vector3Zero(), 1.0f, PURPLE);
		}
		else
		{
			DrawModel(inEditor->editorGizmoMoveAxisZX, Vector3Zero(), 1.0f, WHITE);
		}
	}

	if (inEditor->currentGizmoMode == 1)
	{

		Matrix matScaleX = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationX = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * 180.0f, 0.0f });
		Matrix matTranslationX = MatrixTranslate(inCenterPos.x, inCenterPos.y + 1.8f, inCenterPos.z - 0.5f);
		inEditor->editorGizmoRotateAxisX.transform = MatrixMultiply(MatrixMultiply(matScaleX, matRotationX), matTranslationX);

		Matrix matScaleY = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationY = MatrixRotateXYZ(Vector3{ DEG2RAD * -90.0f, DEG2RAD * 90.0f , 0.0f });
		Matrix matTranslationY = MatrixTranslate(inCenterPos.x + 0.2f, inCenterPos.y - 0.4f, inCenterPos.z - 2.4f);
		inEditor->editorGizmoRotateAxisY.transform = MatrixMultiply(MatrixMultiply(matScaleY, matRotationY), matTranslationY);

		Matrix matScaleZ = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotationZ = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * 90.0f , 0.0f });
		Matrix matTranslationZ = MatrixTranslate(inCenterPos.x + 0.2f, inCenterPos.y + 1.8f, inCenterPos.z);
		inEditor->editorGizmoRotateAxisZ.transform = MatrixMultiply(MatrixMultiply(matScaleZ, matRotationZ), matTranslationZ);

		if (inEditor->selectingGizmoRotateAxisX == false)
		{
			DrawModel(inEditor->editorGizmoRotateAxisX, Vector3Zero(), 1.0f, RED);
		}
		else
		{
			DrawModel(inEditor->editorGizmoRotateAxisX, Vector3Zero(), 1.0f, WHITE);
		}

		if (inEditor->selectingGizmoRotateAxisY == false)
		{
			DrawModel(inEditor->editorGizmoRotateAxisY, Vector3Zero(), 1.0f, GREEN);
		}
		else
		{
			DrawModel(inEditor->editorGizmoRotateAxisY, Vector3Zero(), 1.0f, WHITE);
		}

		if (inEditor->selectingGizmoRotateAxisZ == false)
		{
			DrawModel(inEditor->editorGizmoRotateAxisZ, Vector3Zero(), 1.0f, BLUE);
		}
		else
		{
			DrawModel(inEditor->editorGizmoRotateAxisZ, Vector3Zero(), 1.0f, WHITE);
		}
	}


}

void editor_check_against_move_gizmo(world_editor* inEditor, Vector3 inCenterPos)
{
	//(Levente): NOTE: For now, editing a camera's properties using the gizmo handles is disabled. Fix: when I implement global/local transform into the gizmo system!
	if (typeid(entt_camera) == typeid(inEditor->currentlySelectedEntt)) return;
	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };
		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);

		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoMoveAxisX.meshes[0], inEditor->editorGizmoMoveAxisX.transform);
		if (meshHitInfo.hit)
		{
			if (inEditor->selectingGizmoMoveAxisY == true
				|| inEditor->selectingGizmoMoveAxisZ == true
				|| inEditor->selectingGizmoMoveAxisXY == true
				|| inEditor->selectingGizmoMoveAxisYZ == true
				|| inEditor->selectingGizmoMoveAxisZX == true)
			{

			}

			inEditor->selectingGizmoMoveAxisX = true;
			inEditor->canManipulateWorld = false;

		}
		else
		{
			//(Levente): Lovely thing here. Keep latching on even if we've left the mesh's bounding box.
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoMoveAxisX)
			{
				inEditor->selectingGizmoMoveAxisX = true;
				inEditor->selectingGizmoMoveAxisY = false; // This is so that you don't grab onto another gizmo when grabbing this
				inEditor->selectingGizmoMoveAxisZ = false; // UPDATE: This may not work but I'm not testing it now. Has no practical implications
				inEditor->canManipulateWorld = false;

			}
			else
			{
				inEditor->selectingGizmoMoveAxisX = false;
			}

		}

		cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < inEditor->editorGizmoMoveAxisY.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoMoveAxisY.meshes[m], inEditor->editorGizmoMoveAxisY.transform);
			if (meshHitInfo.hit)
			{
				if (inEditor->selectingGizmoMoveAxisX == true
					|| inEditor->selectingGizmoMoveAxisZ == true
					|| inEditor->selectingGizmoMoveAxisXY == true
					|| inEditor->selectingGizmoMoveAxisYZ == true
					|| inEditor->selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				inEditor->selectingGizmoMoveAxisY = true;
				inEditor->canManipulateWorld = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoMoveAxisY)
				{
					inEditor->selectingGizmoMoveAxisY = true;
					inEditor->selectingGizmoMoveAxisX = false;
					inEditor->selectingGizmoMoveAxisZ = false;
					inEditor->canManipulateWorld = false;

				}
				else
				{
					inEditor->selectingGizmoMoveAxisY = false;
				}

			}
		}

		cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < inEditor->editorGizmoMoveAxisZ.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoMoveAxisZ.meshes[m], inEditor->editorGizmoMoveAxisZ.transform);
			if (meshHitInfo.hit)
			{
				if (inEditor->selectingGizmoMoveAxisY == true
					|| inEditor->selectingGizmoMoveAxisX == true
					|| inEditor->selectingGizmoMoveAxisXY == true
					|| inEditor->selectingGizmoMoveAxisYZ == true
					|| inEditor->selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				inEditor->selectingGizmoMoveAxisZ = true;
				inEditor->canManipulateWorld = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoMoveAxisZ)
				{
					inEditor->selectingGizmoMoveAxisZ = true;
					inEditor->selectingGizmoMoveAxisX = false;
					inEditor->selectingGizmoMoveAxisY = false;
					inEditor->canManipulateWorld = false;

				}
				else
				{
					inEditor->selectingGizmoMoveAxisZ = false;
				}

			}
		}



		cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < inEditor->editorGizmoMoveAxisXY.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoMoveAxisXY.meshes[m], inEditor->editorGizmoMoveAxisXY.transform);
			if (meshHitInfo.hit)
			{
				//(Levente): I needed to introduce extra measures to stop you from grabbing another axis when already grabbing one. The previous fix for this may not work now,
				// but it's so difficult to reproduce if the actual transform code is in place that I won't try.
				if (inEditor->selectingGizmoMoveAxisX == true
					|| inEditor->selectingGizmoMoveAxisZ == true
					|| inEditor->selectingGizmoMoveAxisY == true
					//|| inEditor->selectingGizmoMoveAxisXY == true
					|| inEditor->selectingGizmoMoveAxisYZ == true
					|| inEditor->selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				inEditor->selectingGizmoMoveAxisXY = true;
				inEditor->canManipulateWorld = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoMoveAxisXY)
				{
					inEditor->selectingGizmoMoveAxisXY = true;
					inEditor->selectingGizmoMoveAxisYZ = false;
					inEditor->selectingGizmoMoveAxisZX = false;

					inEditor->selectingGizmoMoveAxisY = false;
					inEditor->selectingGizmoMoveAxisX = false;
					inEditor->selectingGizmoMoveAxisZ = false;
					inEditor->canManipulateWorld = false;

				}
				else
				{
					inEditor->selectingGizmoMoveAxisXY = false;
				}

			}
		}
		cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < inEditor->editorGizmoMoveAxisYZ.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoMoveAxisYZ.meshes[m], inEditor->editorGizmoMoveAxisYZ.transform);
			if (meshHitInfo.hit)
			{
				if (inEditor->selectingGizmoMoveAxisX == true
					|| inEditor->selectingGizmoMoveAxisZ == true
					|| inEditor->selectingGizmoMoveAxisY == true
					|| inEditor->selectingGizmoMoveAxisXY == true
					//|| inEditor->selectingGizmoMoveAxisYZ == true
					|| inEditor->selectingGizmoMoveAxisZX == true)
				{
					break; return;
				}

				inEditor->selectingGizmoMoveAxisYZ = true;
				inEditor->canManipulateWorld = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoMoveAxisYZ)
				{
					inEditor->selectingGizmoMoveAxisXY = false;
					inEditor->selectingGizmoMoveAxisYZ = true;
					inEditor->selectingGizmoMoveAxisZX = false;

					inEditor->selectingGizmoMoveAxisY = false;
					inEditor->selectingGizmoMoveAxisX = false;
					inEditor->selectingGizmoMoveAxisZ = false;
					inEditor->canManipulateWorld = false;

				}
				else
				{
					inEditor->selectingGizmoMoveAxisYZ = false;
				}

			}
		}
		cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		collision = { 0 };
		collision.hit = false;
		meshHitInfo = { 0 };
		for (int m = 0; m < inEditor->editorGizmoMoveAxisZX.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoMoveAxisZX.meshes[m], inEditor->editorGizmoMoveAxisZX.transform);
			if (meshHitInfo.hit)
			{
				if (inEditor->selectingGizmoMoveAxisX == true
					|| inEditor->selectingGizmoMoveAxisZ == true
					|| inEditor->selectingGizmoMoveAxisY == true
					|| inEditor->selectingGizmoMoveAxisXY == true
					|| inEditor->selectingGizmoMoveAxisYZ == true
					/* || inEditor->selectingGizmoMoveAxisZX == true*/)
				{
					break; return;
				}

				inEditor->selectingGizmoMoveAxisZX = true;
				inEditor->canManipulateWorld = false;

				break;
				return;
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoMoveAxisZX)
				{
					inEditor->selectingGizmoMoveAxisXY = false;
					inEditor->selectingGizmoMoveAxisYZ = false;
					inEditor->selectingGizmoMoveAxisZX = true;

					inEditor->selectingGizmoMoveAxisY = false;
					inEditor->selectingGizmoMoveAxisX = false;
					inEditor->selectingGizmoMoveAxisZ = false;
					inEditor->canManipulateWorld = false;

				}
				else
				{
					inEditor->selectingGizmoMoveAxisZX = false;
				}

			}
		}
	}
	else
	{
		inEditor->selectingGizmoMoveAxisX = false;
		inEditor->selectingGizmoMoveAxisY = false;
		inEditor->selectingGizmoMoveAxisZ = false;
		inEditor->selectingGizmoMoveAxisXY = false;
		inEditor->selectingGizmoMoveAxisYZ = false;
		inEditor->selectingGizmoMoveAxisZX = false;

		inEditor->canManipulateWorld = true;
	}

	if (inEditor->selectingGizmoMoveAxisX && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntt);
	if (inEditor->selectingGizmoMoveAxisY && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 1, inCenterPos, inEditor->currentlySelectedEntt);
	if (inEditor->selectingGizmoMoveAxisZ && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 2, inCenterPos, inEditor->currentlySelectedEntt);

	if (inEditor->selectingGizmoMoveAxisXY && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 3, inCenterPos, inEditor->currentlySelectedEntt);
	if (inEditor->selectingGizmoMoveAxisYZ && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 4, inCenterPos, inEditor->currentlySelectedEntt);
	if (inEditor->selectingGizmoMoveAxisZX && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 5, inCenterPos, inEditor->currentlySelectedEntt);



}

void editor_check_against_rotate_gizmo(world_editor* inEditor, Vector3 inCenterPos)
{
	//(Levente): NOTE: For now, editing a camera's properties using the gizmo handles is disabled. Fix: when I implement global/local transform into the gizmo system!
	if (typeid(entt_camera) == typeid(inEditor->currentlySelectedEntt)) return;

	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		for (int m = 0; m < inEditor->editorGizmoRotateAxisX.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoRotateAxisX.meshes[m], inEditor->editorGizmoRotateAxisX.transform);

			if (meshHitInfo.hit)
			{
				if (inEditor->selectingGizmoMoveAxisX == true
					|| inEditor->selectingGizmoMoveAxisZ == true
					|| inEditor->selectingGizmoMoveAxisY == true
					|| inEditor->selectingGizmoMoveAxisXY == true
					|| inEditor->selectingGizmoMoveAxisYZ == true
					|| inEditor->selectingGizmoMoveAxisZX == true
					|| inEditor->selectingGizmoRotateAxisY == true
					|| inEditor->selectingGizmoRotateAxisZ == true)
				{
					break;
					return;
				}
				else
				{
					inEditor->selectingGizmoRotateAxisX = true;
					inEditor->canManipulateWorld = false;

					break;
					return;

				}
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoRotateAxisX)
				{
					inEditor->selectingGizmoRotateAxisX = true;
					inEditor->canManipulateWorld = false;

				}
				else
				{
					inEditor->selectingGizmoRotateAxisX = false;
				}

			}
		}

		cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		meshHitInfo = { 0 };

		for (int m = 0; m < inEditor->editorGizmoRotateAxisY.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoRotateAxisY.meshes[m], inEditor->editorGizmoRotateAxisY.transform);

			if (meshHitInfo.hit)
			{
				if (inEditor->selectingGizmoMoveAxisX == true
					|| inEditor->selectingGizmoMoveAxisZ == true
					|| inEditor->selectingGizmoMoveAxisY == true
					|| inEditor->selectingGizmoMoveAxisXY == true
					|| inEditor->selectingGizmoMoveAxisYZ == true
					|| inEditor->selectingGizmoMoveAxisZX == true
					|| inEditor->selectingGizmoRotateAxisX == true
					|| inEditor->selectingGizmoRotateAxisZ == true)
				{
					break; return;
				}
				else
				{
					inEditor->selectingGizmoRotateAxisY = true;
					inEditor->canManipulateWorld = false;

					break; return;

				}
			}
			else
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && inEditor->selectingGizmoRotateAxisY)
				{
					inEditor->selectingGizmoRotateAxisY = true;
					inEditor->canManipulateWorld = false;

				}
				else
				{
					inEditor->selectingGizmoRotateAxisY = false;
				}

			}
		}

	}
	else
	{
		editor_do_not_select_any_gizmo(inEditor);

		inEditor->canManipulateWorld = true;
	}


	if (inEditor->selectingGizmoRotateAxisX && inEditor->selectingEntt) editor_rotate_entt_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntt);
	if (inEditor->selectingGizmoRotateAxisY && inEditor->selectingEntt) editor_rotate_entt_gizmo(inEditor, 1, inCenterPos, inEditor->currentlySelectedEntt);
	if (inEditor->selectingGizmoRotateAxisZ && inEditor->selectingEntt) editor_rotate_entt_gizmo(inEditor, 2, inCenterPos, inEditor->currentlySelectedEntt);

}

void editor_move_entt_gizmo(world_editor* inEditor, int inAxis, Vector3 inGizmoCenterPos, entt* enttToMove)
{

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
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		float currentFramePointX = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		float prevFramePointX = 0.0f;

		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointX = meshHitInfo.point.x;
				break;
				return;
			}
		}


		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointX = meshHitInfo.point.x;

				float diff = currentFramePointX - prevFramePointX;

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->transform.pos.x - diff, enttToMove->transform.pos.y, enttToMove->transform.pos.z };

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}

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
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		float currentFramePointY = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		float prevFramePointY = 0.0f;

		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointY = meshHitInfo.point.y;

				break;
				return;
			}
		}


		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointY = meshHitInfo.point.y;

				float diff = currentFramePointY - prevFramePointY;

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };
				newPos = Vector3{ enttToMove->transform.pos.x, enttToMove->transform.pos.y - diff, enttToMove->transform.pos.z };

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
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
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		float currentFramePointZ = 0.0f;

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		float prevFramePointZ = 0.0f;

		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointZ = meshHitInfo.point.z;

				break;
				return;
			}
		}


		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointZ = meshHitInfo.point.z;

				float diff = currentFramePointZ - prevFramePointZ;

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->transform.pos.x, enttToMove->transform.pos.y, enttToMove->transform.pos.z - diff };

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
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
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		Vector2 currentFramePointXY{ 0.0f, 0.0f };

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		Vector2 prevFramePointXY{ 0.0f, 0.0f };

		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointXY = Vector2{ meshHitInfo.point.x, meshHitInfo.point.y };

				break;
				return;
			}
		}


		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointXY = Vector2{ meshHitInfo.point.x, meshHitInfo.point.y };

				Vector2 diff = Vector2Subtract(currentFramePointXY, prevFramePointXY);

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->transform.pos.x - diff.x, enttToMove->transform.pos.y - diff.y, enttToMove->transform.pos.z };

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
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
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		Vector2 currentFramePointYZ{ 0.0f, 0.0f };

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		Vector2 prevFramePointYZ{ 0.0f, 0.0f };

		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointYZ = Vector2{ meshHitInfo.point.y, meshHitInfo.point.z };

				break;
				return;
			}
		}


		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointYZ = Vector2{ meshHitInfo.point.y, meshHitInfo.point.z };

				Vector2 diff = Vector2Subtract(currentFramePointYZ, prevFramePointYZ);

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->transform.pos.x, enttToMove->transform.pos.y - diff.x, enttToMove->transform.pos.z - diff.y };

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
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
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		Vector2 currentFramePointZX{ 0.0f, 0.0f };

		Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		Vector2 prevFramePointZX{ 0.0f, 0.0f };

		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				prevFramePointZX = Vector2{ meshHitInfo.point.z, meshHitInfo.point.x };

				break;
				return;
			}
		}


		for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				currentFramePointZX = Vector2{ meshHitInfo.point.z, meshHitInfo.point.x };

				Vector2 diff = Vector2Subtract(currentFramePointZX, prevFramePointZX);

				Vector3 newPos{ 0.0f, 0.0f, 0.0f };

				newPos = Vector3{ enttToMove->transform.pos.x - diff.y, enttToMove->transform.pos.y, enttToMove->transform.pos.z - diff.x };

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, newPos, a->transform.scale, a->transform.rot);

				}
				break;
				return;
			}
		}
	}


}

void editor_rotate_entt_gizmo(world_editor* inEditor, int inAxis, Vector3 inGizmoCenterPos, entt* enttToMove)
{

	if (inAxis == 0)
	{
		Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD * 90.0f });
		Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		//FIRST METHOD

		/*
		if (inEditor->selectingInPrevFrame == false)
		{

			delete inEditor->enttRot;

			Vector2 mousePos = Vector2{ GetMousePosition().x, GetMousePosition().y};
			Ray cursorSelectionRayForFirstFrame = GetMouseRay(mousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);

			meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForFirstFrame, inEditor->editorGizmoHelperModel.meshes[0], inEditor->editorGizmoHelperModel.transform);
			if (meshHitInfo.hit)
			{
				inEditor->firstFramePoint = Vector3{ meshHitInfo.point.x, meshHitInfo.point.y, meshHitInfo.point.z };
				inEditor->selectingInPrevFrame = true;

				inEditor->enttRot = new Vector3{ enttToMove->transform.rot.x, enttToMove->transform.rot.y, enttToMove->transform.rot.z };

			}


		}

		cursorSelectionRay = GetMouseRay(GetMousePosition(), inEditor->currentWorld->currentlyRenderingCamera->rayCam);
		Vector3 currentFramePoint{};

		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[0], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			currentFramePoint = Vector3{ meshHitInfo.point.x, meshHitInfo.point.y, meshHitInfo.point.z };

			//BeginDrawing();
			//BeginMode3D( inEditor->currentWorld->currentlyRenderingCamera->rayCam);

			//DrawCubeV(inEditor->firstFramePoint, Vector3{ 1.0f, 1.0f, 1.0f, }, BLACK);
			//DrawCubeV(currentFramePoint, Vector3{ 1.0f, 1.0f, 1.0f, }, WHITE);

			float a; // This is between the first frame and the current frame
			float b; // Between the center and the first frame
			float c; // Between the center and the now frame

			float cosAlpha; // Cosine at the vertex closest to the gizmo center
			float alpha; // Angle in deg at the vertex closest to the gizmo center (final applied rotation)

			a = Vector2Distance(Vector2{ inEditor->firstFramePoint.y, inEditor->firstFramePoint.z }, Vector2{ currentFramePoint.y, currentFramePoint.z });

			//This should be constant
			b = Vector2Distance(Vector2{inGizmoCenterPos.y, inGizmoCenterPos.z}, Vector2{ inEditor->firstFramePoint.y, inEditor->firstFramePoint.z });

			c = Vector2Distance(Vector2{ inGizmoCenterPos.y, inGizmoCenterPos.z }, Vector2{ currentFramePoint.y, currentFramePoint.z });

			std::string a1 = std::to_string(a);
			std::string b1 = std::to_string(b);
			std::string c1 = std::to_string(c);

			cosAlpha = ((c * c) - (a * a) - (b * b)) / ((-2) * a * b);
			alpha = RAD2DEG*acosf(cosAlpha);

			std::string alpha1 = std::to_string(alpha);
			Vector3 newRot{ 0.0f, 0.0f, 0.0f };
			newRot = Vector3{ inEditor->enttRot->x + alpha, enttToMove->transform.rot.y, enttToMove->transform.rot.z };
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
		Ray cursorSelectionRayCurrentFrame = GetMouseRay(mousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);

		Vector2 mousePosPrevFrame = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };
		Ray cursorSelectionRayPrevFrame = GetMouseRay(mousePosPrevFrame, inEditor->currentWorld->currentlyRenderingCamera->rayCam);

		meshHitInfo = GetRayCollisionMesh(cursorSelectionRayCurrentFrame, inEditor->editorGizmoHelperModel.meshes[0], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			Vector3 currentFramePoint = meshHitInfo.point;

			RayCollision meshHitInfoForPrevFrame = GetRayCollisionMesh(cursorSelectionRayPrevFrame, inEditor->editorGizmoHelperModel.meshes[0], inEditor->editorGizmoHelperModel.transform);

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

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}

				
				graphene_quaternion_free(rotInQuat);
				graphene_quaternion_free(finalQ);

			}
			else
			{
				graphene_quaternion_t* rotInQuat = graphene_quaternion_alloc();
				graphene_quaternion_init_from_angles(rotInQuat, alpha, 0.0f, 0.0f);

				graphene_quaternion_t* finalQ = graphene_quaternion_alloc(); graphene_quaternion_init_identity(finalQ);

				graphene_quaternion_multiply(enttToMove->transform.rot, rotInQuat, finalQ);

				if (typeid(enttToMove) == typeid(entt_maincube))
				{
					entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}
				if (typeid(enttToMove) == typeid(entt_maincube_static))
				{
					entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}
				if (typeid(enttToMove) == typeid(entt_camera))
				{
					entt_camera* a = static_cast<entt_camera*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}
				if (typeid(enttToMove) == typeid(entt_light))
				{
					entt_light* a = static_cast<entt_light*>(enttToMove);
					update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

				}

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
		inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };

		Vector2 mousePos = Vector2{ GetMousePosition().x, GetMousePosition().y };
		Ray cursorSelectionRayCurrentFrame = GetMouseRay(mousePos, inEditor->currentWorld->currentlyRenderingCamera->rayCam);

		Vector2 mousePosPrevFrame = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };
		Ray cursorSelectionRayPrevFrame = GetMouseRay(mousePosPrevFrame, inEditor->currentWorld->currentlyRenderingCamera->rayCam);

		meshHitInfo = GetRayCollisionMesh(cursorSelectionRayCurrentFrame, inEditor->editorGizmoHelperModel.meshes[0], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			Vector3 currentFramePoint = meshHitInfo.point;

			RayCollision meshHitInfoForPrevFrame = GetRayCollisionMesh(cursorSelectionRayPrevFrame, inEditor->editorGizmoHelperModel.meshes[0], inEditor->editorGizmoHelperModel.transform);

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

			if (typeid(enttToMove) == typeid(entt_maincube))
			{
				entt_maincube* a = static_cast<entt_maincube*>(enttToMove);
				update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

			}
			if (typeid(enttToMove) == typeid(entt_maincube_static))
			{
				entt_maincube_static* a = static_cast<entt_maincube_static*>(enttToMove);
				update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

			}
			if (typeid(enttToMove) == typeid(entt_camera))
			{
				entt_camera* a = static_cast<entt_camera*>(enttToMove);
				update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

			}
			if (typeid(enttToMove) == typeid(entt_light))
			{
				entt_light* a = static_cast<entt_light*>(enttToMove);
				update_spatial_props(a, a->transform.pos, a->transform.scale, finalQ);

			}

			graphene_quaternion_free(rotInQuat);
			graphene_quaternion_free(finalQ);


		}


	}
}

*/
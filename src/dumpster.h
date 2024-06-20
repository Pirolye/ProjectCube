
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
				/* || inEditor->selectingGizmoMoveAxisZX == true)
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


void editor_check_against_rotate_gizmo(world_editor* inEditor, Vector3 inCenterPos)
{
	//(Levente): NOTE: For now, editing a camera's properties using the gizmo handles is disabled. Fix: when I implement global/local transform into the gizmo system!
	if (typeid(entt_camera) == typeid(inEditor->currentlySelectedEntity)) return;


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


	if (inEditor->selectingGizmoRotateAxisX && inEditor->selectingEntt) editor_rotate_entt_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
	if (inEditor->selectingGizmoRotateAxisY && inEditor->selectingEntt) editor_rotate_entt_gizmo(inEditor, 1, inCenterPos, inEditor->currentlySelectedEntity);
	if (inEditor->selectingGizmoRotateAxisZ && inEditor->selectingEntt) editor_rotate_entt_gizmo(inEditor, 2, inCenterPos, inEditor->currentlySelectedEntity);

}

	if (inAxis == 0)
	{
		Matrix matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * -90.0f, DEG2RAD * 90.0f });



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


			if (inEditor->selectingGizmoMoveAxisX && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
		if (inEditor->selectingGizmoMoveAxisY && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 1, inCenterPos, inEditor->currentlySelectedEntity);
		if (inEditor->selectingGizmoMoveAxisZ && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 2, inCenterPos, inEditor->currentlySelectedEntity);

		if (inEditor->selectingGizmoMoveAxisXY && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 3, inCenterPos, inEditor->currentlySelectedEntity);
		if (inEditor->selectingGizmoMoveAxisYZ && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 4, inCenterPos, inEditor->currentlySelectedEntity);
		if (inEditor->selectingGizmoMoveAxisZX && inEditor->selectingEntt) editor_move_entt_gizmo(inEditor, 5, inCenterPos, inEditor->currentlySelectedEntity);

	}

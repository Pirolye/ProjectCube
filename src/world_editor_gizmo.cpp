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
	if (inEditor->currentlySelectedEntity && inEditor->currentlySelectedEntity->type == "camera") return;

	Vector3 v{ 1.0f, 0.0f, 0.0f };

	if (inEditor->currentGizmoMode == 0)
	{
		Matrix matScaleStandard = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matScaleShrinked = MatrixScale(0.8f, 0.8f, 0.8f);
		Matrix matTranslationStandard = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		Matrix matRotation;

		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				matRotation = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD * 90.0f });
			}
			if (i == 1)
			{
				matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f , 0.0f });
			}
			if (i == 2)
			{
				matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f , 0.0f });
			}

			inEditor->moveGizmo->model[i]->model.transform = MatrixMultiply(MatrixMultiply(matScaleStandard, matRotation), matTranslationStandard);

		}

		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				if (inEditor->moveGizmo->model[i]->isSelected) DrawModel(inEditor->moveGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->moveGizmo->model[i]->model, Vector3Zero(), 1.0f, RED);
			}
			if (i == 1)
			{
				if (inEditor->moveGizmo->model[i]->isSelected) DrawModel(inEditor->moveGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->moveGizmo->model[i]->model, Vector3Zero(), 1.0f, GREEN);
			}
			if (i == 2)
			{
				if (inEditor->moveGizmo->model[i]->isSelected) DrawModel(inEditor->moveGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->moveGizmo->model[i]->model, Vector3Zero(), 1.0f, BLUE);
			}

		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	if (inEditor->currentGizmoMode == 1)
	{
		Matrix matScaleStandard = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matTranslationStandard = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		Matrix matRotation;

		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				matRotation = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD * 90.0f });
			}
			if (i == 1)
			{
				matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f , 0.0f });
			}
			if (i == 2)
			{
				matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * 90.0f, 0.0f , 0.0f });
			}

			inEditor->scaleGizmo->model[i]->model.transform = MatrixMultiply(MatrixMultiply(matScaleStandard, matRotation), matTranslationStandard);

		}

		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				if (inEditor->scaleGizmo->model[i]->isSelected) DrawModel(inEditor->scaleGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->scaleGizmo->model[i]->model, Vector3Zero(), 1.0f, RED);
			}
			if (i == 1)
			{
				if (inEditor->scaleGizmo->model[i]->isSelected) DrawModel(inEditor->scaleGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->scaleGizmo->model[i]->model, Vector3Zero(), 1.0f, GREEN);
			}
			if (i == 2)
			{
				if (inEditor->scaleGizmo->model[i]->isSelected) DrawModel(inEditor->scaleGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->scaleGizmo->model[i]->model, Vector3Zero(), 1.0f, BLUE);
			}

		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (inEditor->currentGizmoMode == 2)
	{
		Matrix matScaleStandard = MatrixScale(1.0f, 1.0f, 1.0f);
		Matrix matTranslationStandard; // = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
		Matrix matRotation;

		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * 180.0f, 0.0f });
				matTranslationStandard = MatrixTranslate(inCenterPos.x, inCenterPos.y + 1.8f, inCenterPos.z - 0.5f);
			}
			if (i == 1)
			{
				matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * -90.0f, DEG2RAD * 90.0f , 0.0f });
				matTranslationStandard = MatrixTranslate(inCenterPos.x + 0.2f, inCenterPos.y - 0.4f, inCenterPos.z - 2.4f);
			}
			if (i == 2)
			{
				matRotation = MatrixRotateXYZ(Vector3{ 0.0f, DEG2RAD * 90.0f , 0.0f });
				matTranslationStandard = MatrixTranslate(inCenterPos.x + 0.2f, inCenterPos.y + 1.8f, inCenterPos.z);
			}

			inEditor->rotateGizmo->model[i]->model.transform = MatrixMultiply(MatrixMultiply(matScaleStandard, matRotation), matTranslationStandard);

		}

		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				if (inEditor->rotateGizmo->model[i]->isSelected) DrawModel(inEditor->rotateGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->rotateGizmo->model[i]->model, Vector3Zero(), 1.0f, RED);
			}
			if (i == 1)
			{
				if (inEditor->rotateGizmo->model[i]->isSelected) DrawModel(inEditor->rotateGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->rotateGizmo->model[i]->model, Vector3Zero(), 1.0f, GREEN);
			}
			if (i == 2)
			{
				if (inEditor->rotateGizmo->model[i]->isSelected) DrawModel(inEditor->rotateGizmo->model[i]->model, Vector3Zero(), 1.0f, WHITE);
				else DrawModel(inEditor->rotateGizmo->model[i]->model, Vector3Zero(), 1.0f, BLUE);
			}

		}
	}
}


void editor_check_against_move_gizmo(world_editor* inEditor, Vector3 inCenterPos)
{
	//(Levente): NOTE: For now, editing a camera's properties using the gizmo handles is disabled. Fix: when I implement global/local transform into the gizmo system!
	if (inEditor->currentlySelectedEntity && inEditor->currentlySelectedEntity->type == "camera") return;
	
	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->currentlyRenderingCamera->data);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{

		if (inEditor->moveGizmo->currentlySelectedModel != nullptr)
		{
			editor_move_entity_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
			return;
		}


		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };
		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

		for (int i = 0; i < 6; i++)
		{
			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->moveGizmo->model[i]->model.meshes[0], inEditor->moveGizmo->model[i]->model.transform);

			if (meshHitInfo.hit)
			{
				inEditor->moveGizmo->model[i]->isSelected = true;
				inEditor->moveGizmo->currentlySelectedModel = inEditor->moveGizmo->model[i];
				inEditor->canManipulateWorld = false;
				break; return;
			}
			else
			{

				for (int i = 0; i < 6; i++)
				{
					inEditor->moveGizmo->model[i]->isSelected = false;
					inEditor->moveGizmo->currentlySelectedModel = nullptr;
				}

				inEditor->canManipulateWorld = true;
			}
		}

		editor_move_entity_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			inEditor->moveGizmo->model[i]->isSelected = false;
			inEditor->moveGizmo->currentlySelectedModel = nullptr;
		}

	}


}


void editor_move_entity_gizmo(world_editor* inEditor, int inAxis, Vector3 inGizmoCenterPos, entity* entityToMove)
{

	//(Levente): When moving, we transform the helper behind the object so that the ray hits it perpendicular to the axis. Eg. On the X axis,
	//			 it won't be position ONTO the axis, but behind it,(since an axis (here: X) is 1D; normally we imagine it as sprawling across
	//           the map, but it's actually a straight line in +x and -x) since logically that's where the mouse's ray end up.
	//
	//			 For each axis, we do 2 casts, to get starting pos and ending pos = small increments = smooth.

	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->currentlyRenderingCamera->data);

	Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

	Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, camera->rayCam);

	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
	Matrix matRotation;


	for (int i = 0; i < 6; i++)
	{
		if (inEditor->moveGizmo->model[i]->isSelected)
		{
			matRotation = inEditor->moveGizmo->model[i]->helperModelRotation;
		}
	}

	inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);


	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;
	RayCollision meshHitInfo = { 0 };

	// previous frame
	for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			if (inEditor->moveGizmo->currentlySelectedModel->axis == 0)
			{
				inEditor->moveGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.x;
			}
			if (inEditor->moveGizmo->currentlySelectedModel->axis == 1)
			{
				inEditor->moveGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.y;
			}
			if (inEditor->moveGizmo->currentlySelectedModel->axis == 2)
			{
				inEditor->moveGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.z;
			}

			break;
			return;
		}
	}

	Vector3 newPos{ 0.0f, 0.0f, 0.0f };


	// current frame
	for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			if (inEditor->moveGizmo->currentlySelectedModel->axis == 0)
			{
				inEditor->moveGizmo->currentlySelectedModel->currentPoint = meshHitInfo.point.x;
				float diff = inEditor->moveGizmo->currentlySelectedModel->currentPoint - inEditor->moveGizmo->currentlySelectedModel->prevPoint;

				newPos = Vector3{ entityToMove->transform.pos.x - diff, entityToMove->transform.pos.y, entityToMove->transform.pos.z };

			}
			if (inEditor->moveGizmo->currentlySelectedModel->axis == 1)
			{
				inEditor->moveGizmo->currentlySelectedModel->currentPoint = meshHitInfo.point.y;
				float diff = inEditor->moveGizmo->currentlySelectedModel->currentPoint - inEditor->moveGizmo->currentlySelectedModel->prevPoint;

				newPos = Vector3{ entityToMove->transform.pos.x, entityToMove->transform.pos.y - diff, entityToMove->transform.pos.z };

			}
			if (inEditor->moveGizmo->currentlySelectedModel->axis == 2)
			{
				inEditor->moveGizmo->currentlySelectedModel->currentPoint = meshHitInfo.point.z;
				float diff = inEditor->moveGizmo->currentlySelectedModel->currentPoint - inEditor->moveGizmo->currentlySelectedModel->prevPoint;

				newPos = Vector3{ entityToMove->transform.pos.x, entityToMove->transform.pos.y, entityToMove->transform.pos.z - diff};

			}




			update_spatial_properties(entityToMove, newPos, entityToMove->transform.scale);

			break;
			return;
		}
	}


}

void editor_check_against_scale_gizmo(world_editor* inEditor, Vector3 inCenterPos)
{
	if (inEditor->currentlySelectedEntity && inEditor->currentlySelectedEntity->type == "camera") return;

	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->currentlyRenderingCamera->data);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{

		if (inEditor->scaleGizmo->currentlySelectedModel != nullptr)
		{
			editor_scale_entity_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
			return;
		}

		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };
		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

		for (int i = 0; i < 6; i++)
		{

			meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->scaleGizmo->model[i]->model.meshes[0], inEditor->scaleGizmo->model[i]->model.transform);

			if (meshHitInfo.hit)
			{
				inEditor->scaleGizmo->model[i]->isSelected = true;
				inEditor->scaleGizmo->currentlySelectedModel = inEditor->scaleGizmo->model[i];
				inEditor->canManipulateWorld = false;
				break; return;
			}
			else
			{

				for (int i = 0; i < 6; i++)
				{
					inEditor->scaleGizmo->model[i]->isSelected = false;
					inEditor->scaleGizmo->currentlySelectedModel = nullptr;
				}


				inEditor->canManipulateWorld = true;
			}
		}

		editor_scale_entity_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			inEditor->scaleGizmo->model[i]->isSelected = false;
			inEditor->scaleGizmo->currentlySelectedModel = nullptr;
		}

	}


}

void editor_scale_entity_gizmo(world_editor* inEditor, int inAxis, Vector3 inGizmoCenterPos, entity* entityToMove)
{
	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->currentlyRenderingCamera->data);

	Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

	Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, camera->rayCam);

	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
	Matrix matRotation;


	for (int i = 0; i < 6; i++)
	{
		if (inEditor->scaleGizmo->model[i]->isSelected)
		{
			matRotation = inEditor->scaleGizmo->model[i]->helperModelRotation;
		}
	}

	inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);


	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;
	RayCollision meshHitInfo = { 0 };

	// previous frame
	for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			if (inEditor->scaleGizmo->currentlySelectedModel->axis == 0)
			{
				inEditor->scaleGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.x;
			}
			if (inEditor->scaleGizmo->currentlySelectedModel->axis == 1)
			{
				inEditor->scaleGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.y;
			}
			if (inEditor->scaleGizmo->currentlySelectedModel->axis == 2)
			{
				inEditor->scaleGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.z;
			}

			break;
			return;
		}
	}

	Vector3 newScale{ 1.0f, 1.0f, 1.0f };


	// current frame
	for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			if (inEditor->scaleGizmo->currentlySelectedModel->axis == 0)
			{
				inEditor->scaleGizmo->currentlySelectedModel->currentPoint = meshHitInfo.point.x;
				float diff = inEditor->scaleGizmo->currentlySelectedModel->currentPoint - inEditor->scaleGizmo->currentlySelectedModel->prevPoint;

				newScale = Vector3{ entityToMove->transform.scale.x - diff, entityToMove->transform.scale.y, entityToMove->transform.scale.z };

			}
			if (inEditor->scaleGizmo->currentlySelectedModel->axis == 1)
			{
				inEditor->scaleGizmo->currentlySelectedModel->currentPoint = meshHitInfo.point.y;
				float diff = inEditor->scaleGizmo->currentlySelectedModel->currentPoint - inEditor->scaleGizmo->currentlySelectedModel->prevPoint;

				newScale = Vector3{ entityToMove->transform.scale.x, entityToMove->transform.scale.y - diff, entityToMove->transform.scale.z };

			}
			if (inEditor->scaleGizmo->currentlySelectedModel->axis == 2)
			{
				inEditor->scaleGizmo->currentlySelectedModel->currentPoint = meshHitInfo.point.z;
				float diff = inEditor->scaleGizmo->currentlySelectedModel->currentPoint - inEditor->scaleGizmo->currentlySelectedModel->prevPoint;

				newScale = Vector3{ entityToMove->transform.scale.x, entityToMove->transform.scale.y, entityToMove->transform.scale.z - diff };

			}


			if (newScale.x <= 0.01f || newScale.y <= 0.01f || newScale.z <= 0.01f)
			{
				break;
				return;
			}

			update_spatial_properties(entityToMove, entityToMove->transform.pos, newScale);

			break;
			return;
		}
	}

}

void editor_check_against_rotate_gizmo(world_editor* inEditor, Vector3 inCenterPos)
{
	if (inEditor->currentlySelectedEntity && inEditor->currentlySelectedEntity->type == "camera") return;

	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->currentlyRenderingCamera->data);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		RayCollision collision = { 0 };
		collision.distance = FLT_MAX;
		collision.hit = false;
		RayCollision meshHitInfo = { 0 };
		Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

		for (int i = 0; i < 6; i++)
		{
			for (int i2 = 0; i2 < inEditor->rotateGizmo->model[i]->model.meshCount; i2++)
			{
				meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->rotateGizmo->model[i]->model.meshes[i2], inEditor->rotateGizmo->model[i]->model.transform);

				if (meshHitInfo.hit)
				{
					inEditor->rotateGizmo->model[i]->isSelected = true;
					inEditor->rotateGizmo->currentlySelectedModel = inEditor->rotateGizmo->model[i];
					inEditor->canManipulateWorld = false;
					break; return;
				}
				else
				{

					if (inEditor->rotateGizmo->currentlySelectedModel != nullptr)
					{
						editor_move_entity_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
						break; return;
					}
					else
					{
						for (int i = 0; i < 6; i++)
						{
							inEditor->rotateGizmo->model[i]->isSelected = false;
							inEditor->rotateGizmo->currentlySelectedModel = nullptr;
						}

					}


					inEditor->canManipulateWorld = true;
				}

			}
			
		}

		editor_rotate_entity_gizmo(inEditor, 0, inCenterPos, inEditor->currentlySelectedEntity);
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			inEditor->rotateGizmo->model[i]->isSelected = false;
			inEditor->rotateGizmo->currentlySelectedModel = nullptr;
		}

	}


}

void editor_rotate_entity_gizmo(world_editor* inEditor, int inAxis, Vector3 inGizmoCenterPos, entity* entityToMove)
{
	entity_camera_data* camera = reinterpret_cast<entity_camera_data*>(inEditor->currentWorld->currentlyRenderingCamera->data);

	Vector2 previousFrameMousePos = Vector2{ GetMousePosition().x + GetMouseDelta().x, GetMousePosition().y + GetMouseDelta().y };

	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera->rayCam);

	Ray cursorSelectionRayForPrevFrame = GetMouseRay(previousFrameMousePos, camera->rayCam);

	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matTranslation = MatrixTranslate(inGizmoCenterPos.x, inGizmoCenterPos.y, inGizmoCenterPos.z);
	Matrix matRotation;


	for (int i = 0; i < 6; i++)
	{
		if (inEditor->rotateGizmo->model[i]->isSelected)
		{
			matRotation = inEditor->rotateGizmo->model[i]->helperModelRotation;
		}
	}

	inEditor->editorGizmoHelperModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);


	RayCollision collision = { 0 };
	collision.distance = FLT_MAX;
	collision.hit = false;
	RayCollision meshHitInfo = { 0 };

	// previous frame
	for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
	{		
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{
			if (inEditor->rotateGizmo->currentlySelectedModel->axis == 0)
			{
				inEditor->rotateGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.x;
			}
			if (inEditor->rotateGizmo->currentlySelectedModel->axis == 1)
			{
				inEditor->rotateGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.y;
			}
			if (inEditor->rotateGizmo->currentlySelectedModel->axis == 2)
			{
				inEditor->rotateGizmo->currentlySelectedModel->prevPoint = meshHitInfo.point.z;
			}

			break;
			return;
		}
	}

	Vector3 newRot{ 0.0f, 0.0f, 0.0f };



	// current frame
	for (int m = 0; m < inEditor->editorGizmoHelperModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEditor->editorGizmoHelperModel.meshes[m], inEditor->editorGizmoHelperModel.transform);
		if (meshHitInfo.hit)
		{

			Vector3 currentFramePoint = meshHitInfo.point;

			RayCollision meshHitInfoForPrevFrame = GetRayCollisionMesh(cursorSelectionRayForPrevFrame, inEditor->editorGizmoHelperModel.meshes[0], inEditor->editorGizmoHelperModel.transform);

			if (!meshHitInfoForPrevFrame.hit) return; //THIS MIGHT NOT BE GOOD

			double angle = Vector3Angle(meshHitInfo.point, meshHitInfoForPrevFrame.point);

			graphene_quaternion_t* rotInQuat = graphene_quaternion_alloc();
			graphene_quaternion_init_from_angles(rotInQuat, angle, 0.0f, 0.0f);
			graphene_quaternion_t* finalQ = graphene_quaternion_alloc();
			graphene_quaternion_multiply(rotInQuat, entityToMove->transform.rot, finalQ);

			break;
			return;

			//newRot = Vector3{ entityToMove->transform.scale.x, entityToMove->transform.scale.y, entityToMove->transform.scale.z - diff };

		}
	}


			//update_spatial_properties(entityToMove, entityToMove->transform.pos, entityToMove->transform.scale, finalQ);


}

/*
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
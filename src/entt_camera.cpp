#include "world.h"

#include "raylib.h"

;
#include "raymath.h"

;
#define CAMERA_IMPLEMENTATION
#include "rcamera.h"

;
#include "rlgl.h"

#include "stdlib.h"
#include <typeinfo>
#include "graphene.h"
#include "assert.h"

/*

BIRO: Originally I wanted to have a multi cam system, where you could have as many cameras as you wanted. However, due to poor software design regarding rcamera.h, I was not 
even close to accomplishing inEntt. Now we have 1 camera which switches properties when entering/exiting editor mode.

*/


void on_make(entity_camera_data* inData, entity* inEntity)
{
	inData->rayCam.position = Vector3{ 0.0f, 0.0f, 0.0f }; // Camera position
	inData->rayCam.target = Vector3{ 0.0f, 0.0f, 1.0f };      // Camera looking at point
	inData->rayCam.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	inData->rayCam.fovy = 70.0f;                                // Camera field-of-view 
	inData->rayCam.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	inEntity->transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(inEntity->transform.rot);

	inEntity->transform.pos = Vector3Zero();
	
	inData->mode = 0;
	inData->moveSpeed = 0.1f;

	inEntity->transform.scale = Vector3{ 1.0f, 1.0f, 1.0f };
	inData->euler = Vector3Zero();

	inData->cameraEditorModel = LoadModel("editor/camera_model.obj");
	
}

void on_destroy(entity_camera_data* inData, entity* inEntity)
{
	UnloadModel(inData->cameraEditorModel);
};

void on_update(entity_camera_data* inData, entity* inEntity)
{
	if (inEntity != inEntity->containingWorld->currentlyRenderingCamera) return;

	if (inData->mode == 1)
	{
		//(Levente): WARNING! I had to modify rcore.c in order to not put the mouse cursor back in the middle every time you enabled/disabled it! Also, I have to separately get the mouse delta every time I need it, using a variable such as diff in inEntt case doesn't work for some reason.

		if (!inEntity->containingWorld->worldEditor->canManipulateWorld) return;

		Vector2 c = GetMousePosition();

		bool canMove = false;

		Vector2 diff1 = GetMouseDelta();

		Vector2 diff = Vector2{ diff1.x / 2.0f, diff1.y / 2.0f };

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			DisableCursor();

			transform_camera_by_delta(inData, inEntity, Vector3Zero(), Vector3{ diff.x, diff.y, 0.0f });

			SetMousePosition(c.x - GetMouseDelta().x, c.y - GetMouseDelta().y);
			canMove = true;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			EnableCursor();
			SetMousePosition(c.x - GetMouseDelta().x, c.y - GetMouseDelta().y);
			canMove = false;
		}

		if (!canMove) return;

		if (IsKeyDown(KEY_W)) transform_camera_by_delta(inData, inEntity, Vector3{ inData->moveSpeed, 0.0f, 0.0f }, Vector3Zero());
		if (IsKeyDown(KEY_S)) transform_camera_by_delta(inData, inEntity, Vector3{ -(inData->moveSpeed), 0.0f, 0.0f }, Vector3Zero());
		if (IsKeyDown(KEY_D)) transform_camera_by_delta(inData, inEntity, Vector3{ 0.0f, 0.0f, inData->moveSpeed }, Vector3Zero());
		if (IsKeyDown(KEY_A)) transform_camera_by_delta(inData, inEntity, Vector3{ 0.0f, 0.0f, -(inData->moveSpeed) }, Vector3Zero());
		if (IsKeyDown(KEY_E)) transform_camera_by_delta(inData, inEntity, Vector3{ 0.0f, inData->moveSpeed, 0.0f }, Vector3Zero());
		if (IsKeyDown(KEY_Q)) transform_camera_by_delta(inData, inEntity, Vector3{ 0.0f, -(inData->moveSpeed), 0.0f }, Vector3Zero());

	}
	else if (inData->mode == 0)
	{
		return;
	}
	else
	{
		return;
	}


};

void on_draw_2d(entity_camera_data* inData, entity* inEntity)
{
	if (inEntity != inEntity->containingWorld->currentlyRenderingCamera) return;
	DrawFPS(10, 20);

};

void on_draw_3d(entity_camera_data* inData, entity* inEntity)
{
	if (inEntity != inEntity->containingWorld->currentlyRenderingCamera) return;
	
	if (inEntity->containingWorld->worldEditor->isInEditorMode)
	{
		DrawGrid(10, 1.0f);

		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (inEntity->containingWorld->entityArray[i] == NULL) continue;
			
			if (inEntity->containingWorld->entityArray[i]->type != "camera") continue;

			//if (dynamic_cast<entt_camera*>(inEntt->containingWorld->entityArray[i]) == nullptr) continue;
			//if (typeid(entt_camera) != typeid(inEntt->containingWorld->entityArray[i])) continue;
			
			//entt_camera* currentCam = static_cast<entt_camera*>(inEntt->containingWorld->entityArray[i]);
			entity* currentCam = inEntity->containingWorld->entityArray[i];

			if (currentCam == inEntity) continue;
				
			entity_camera_data* cameraData = reinterpret_cast<entity_camera_data*>(currentCam->data);

			if (cameraData->isForEditorOnly) DrawModel(cameraData->cameraEditorModel, Vector3Zero(), 1.0f, BLUE);
			else DrawModel(cameraData->cameraEditorModel, Vector3Zero(), 1.0f, GREEN);

			if (inEntity->containingWorld->worldEditor->currentlySelectedEntity == currentCam)
			{
				DrawModelWires(cameraData->cameraEditorModel, Vector3Zero(), 1.0f, RED);
			}
						
		}
	}

}

void set_camera_mode(entity* inEntity, int inMode, bool inIsForEditorOnly)
{
	if (inEntity->type != "camera") return;
	
	entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);
	
	
	if (inMode != 1 && inMode != 0)
	{
		printf("[game] ERROR: Tried to set camera.mode to value <<%d>> (invalid, only <<1>> and <<0>> is valid) on camera %s!\n", inMode, inEntity->id.c_str());
		return;
	}

	inData->mode = inMode;
	inData->isForEditorOnly = inIsForEditorOnly;
}

void editor_camera_update_model_rotation(entity_camera_data* inData, entity* inEntity)
{
	Matrix matScale = MatrixScale(inEntity->transform.scale.x, inEntity->transform.scale.y, inEntity->transform.scale.z);
	Matrix matTranslation = MatrixTranslate(inEntity->transform.pos.x, inEntity->transform.pos.y, inEntity->transform.pos.z - 1.0f);
	
	Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * inData->euler.x, DEG2RAD * inData->euler.y, DEG2RAD * 0.0f });

	inData->cameraEditorModel.transform = MatrixIdentity();
	inData->cameraEditorModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
}

void update_spatial_props(entity_camera_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{
	
	float x, y, z;

	graphene_quaternion_to_angles(inNewRotation, &x, &y, &z);
	
	transform_camera_by_delta(inData, inEntity, Vector3{ inNewPos.z - inEntity->transform.pos.z, inNewPos.y - inEntity->transform.pos.y, -1.0f * (inNewPos.x - inEntity->transform.pos.x) }, Vector3{x, y, z});
	

}

void transform_camera_by_delta(entity_camera_data* inData, entity* inEntity, Vector3 inNewPosDelta, Vector3 inNewRotDelta)
{
	//(Levente): We have to manually calculate the angle of the camera based on the inputs we receive.
	// I originally wanted to clamp the value of the y axis rot to 360, but we lost so much precision with all that that I let it go. Now, we work with floats in the 0 - 10 000 range, usually. Hopefully won't cause any issues.
	
	UpdateCameraPro(&(inData->rayCam), Vector3Zero(), inNewRotDelta, 0.0f);
	
	inEntt->euler.x = inEntt->euler.x + inNewRotDelta.y;
	inEntt->euler.y = inEntt->euler.y - inNewRotDelta.x;

	if (inEntt->euler.x > 90.0f) inEntt->euler.x = 90.0f;
	if (inEntt->euler.x < -90.0f) inEntt->euler.x = -90.0f;

	/*
	if (euler.y > 359.5f) euler.y = 0.0f;
	if (euler.y < 0.0f) euler.y = 359.5f;
	*/
/*
	//printf("[game] currentlyRenderingCam euler: %f %f %f\n", inEntt->euler.x, inEntt->euler.y, inEntt->euler.z);

	CameraMoveForward(&(inEntt->rayCam), inNewPosDelta.x, false);
	CameraMoveRight(&(inEntt->rayCam), inNewPosDelta.z, false);

	inEntt->rayCam.target = Vector3{ inEntt->rayCam.target.x, inEntt->rayCam.target.y + inNewPosDelta.y, inEntt->rayCam.target.z };
	inEntt->rayCam.position = Vector3{ inEntt->rayCam.position.x, inEntt->rayCam.position.y + inNewPosDelta.y, inEntt->rayCam.position.z };

	inEntity->transform.pos = inEntt->rayCam.position;
	editor_camera_update_model_rotation(inEntt);

}

/*
*
*
* EDITOR FUNCTION DEFINITIONS
*
*
*/
/*
#ifdef DEBUG

entity_pointer* editor_try_select(entt_camera* inEntt)
{
	Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), inEntity->containingWorld->currentlyRenderingCamera->rayCam);

	RayCollision meshHitInfo = { 0 };
	for (int m = 0; m < inEntt->cameraEditorModel.meshCount; m++)
	{
		meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, inEntt->cameraEditorModel.meshes[m], inEntt->cameraEditorModel.transform);
		if (meshHitInfo.hit)
		{
			return inEntity->thisInArray;

			break;  // Stop once one mesh collision is detected, the colliding mesh is m
		}
		else
		{
			return nullptr;
		}
	}

};

#else

entt* editor_try_select(entt_camera* inEntt)
{
	return nullptr;
};

#endif
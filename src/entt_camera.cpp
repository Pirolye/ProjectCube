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
#include "graphene.h"
#include "assert.h"

/*

BIRO: Originally I wanted to have a multi cam system, where you could have as many cameras as you wanted. However, due to poor software design regarding rcamera.h, I was not 
even close to accomplishing this. Now we have 1 camera which switches properties when entering/exiting editor mode.

*/


void entt_camera::on_make()
{
	rayCam.position = Vector3{ 0.0f, 0.0f, -5.0f }; // Camera position
	rayCam.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	rayCam.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	rayCam.fovy = 70.0f;                                // Camera field-of-view 
	rayCam.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	transform.rot = graphene_quaternion_alloc();
	graphene_quaternion_init_identity(transform.rot);

	mode = 0;
	moveSpeed = 0.1f;

	transform.scale = Vector3{ 1.0f, 1.0f, 1.0f };
	euler = Vector3Zero();

	cameraEditorModel = LoadModel("editor/camera_model.obj");
	
}

void entt_camera::on_destroy() 
{
	UnloadModel(cameraEditorModel);
};

void entt_camera::on_update() 
{
	update_camera();
};

void entt_camera::on_draw_2d()
{
	if (this != containingWorld->currentlyRenderingCamera) return;
	DrawFPS(10, 10);

};

void entt_camera::on_draw_3d()
{
	if (this != containingWorld->currentlyRenderingCamera) return;
	
	if (containingWorld->worldEditor.isInEditorMode)
	{
		DrawGrid(10, 1.0f);

		for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
		{
			if (containingWorld->entityArray[i] != NULL)
			{
				if (dynamic_cast<entt_camera*>(containingWorld->entityArray[i]) != nullptr)
				{
					entt_camera* currentCam = dynamic_cast<entt_camera*>(containingWorld->entityArray[i]);
					if(currentCam != this) DrawModel(currentCam->cameraEditorModel, Vector3Zero(), 1.0f, BLUE);
				
				}
			}
		}
	}

}

void entt_camera::set_mode(int inMode, bool inIsForEditorOnly)
{
	if (inMode != 1 && inMode != 0)
	{
		printf("[game] ERROR: Tried to set camera.mode to value <<%d>> (invalid, only <<1>> and <<0>> is valid) on camera %s!\n", inMode, id.c_str());
		return;
	}

	mode = inMode;
	isForEditorOnly = inIsForEditorOnly;
}

void entt_camera::update_camera()
{
	if (this != containingWorld->currentlyRenderingCamera) return;
	
	if (mode == 1)
	{
		//(Levente): WARNING! I had to modify rcore.c in order to not put the mouse cursor back in the middle every time you enabled/disabled it! Also, I have to separately get the mouse delta every time I need it, using a variable such as diff in this case doesn't work for some reason.
		
		if (!containingWorld->worldEditor.canManipulateWorld) return;
		
		Vector2 c = GetMousePosition();

		bool canMove = false;

		Vector2 diff1 = GetMouseDelta(); 

		Vector2 diff = Vector2{ diff1.x / 2.0f, diff1.y / 2.0f };
	
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			DisableCursor();
			
			transform_camera_by_delta(Vector3Zero(), Vector3{ diff.x, diff.y, 0.0f });
			
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

		if (IsKeyDown(KEY_W)) transform_camera_by_delta(Vector3{ moveSpeed, 0.0f, 0.0f }, Vector3Zero());
		if (IsKeyDown(KEY_S)) transform_camera_by_delta(Vector3{ -moveSpeed, 0.0f, 0.0f }, Vector3Zero());
		if (IsKeyDown(KEY_D)) transform_camera_by_delta(Vector3{ 0.0f, 0.0f, moveSpeed }, Vector3Zero());
		if (IsKeyDown(KEY_A)) transform_camera_by_delta(Vector3{ 0.0f, 0.0f, -moveSpeed }, Vector3Zero());
		if (IsKeyDown(KEY_E)) transform_camera_by_delta(Vector3{ 0.0f, moveSpeed, 0.0f }, Vector3Zero());
		if (IsKeyDown(KEY_Q)) transform_camera_by_delta(Vector3{ 0.0f, -moveSpeed, 0.0f }, Vector3Zero());

	}
	else if (mode == 0)
	{
		return;
	}
	else
	{
		return;
	}
	
	

}

void entt_camera::editor_camera_update_model_rotation()
{




	Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
	Matrix matTranslation = MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z);
	
	Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * euler.x, DEG2RAD * euler.y, 0.0f });

	cameraEditorModel.transform = MatrixIdentity();
	cameraEditorModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
}

void entt_camera::update_spatial_props(Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{
	printf("[game] Illegal call: entt_camera:update_spatial_props() For camera, always use move_camera_by()!!!");
	assert(false);

}

void entt_camera::transform_camera_by_delta(Vector3 inNewPosDelta, Vector3 inNewRotDelta)
{
	//(Levente): We have to manually calculate the angle of the camera based on the inputs we receive.
	// I originally wanted to clamp the value of the y axis rot to 360, but we lost so much precision with all that that I let it go. Now, we work with floats in the 0 - 10 000 range, usually. Hopefully won't cause any issues.
	
	UpdateCameraPro(&rayCam, Vector3Zero(), inNewRotDelta, 0.0f);
	
	euler.x = euler.x + inNewRotDelta.y;
	euler.y = euler.y - inNewRotDelta.x;

	if (euler.x > 90.0f) euler.x = 90.0f;
	if (euler.x < -90.0f) euler.x = -90.0f;

	/*
	if (euler.y > 359.5f) euler.y = 0.0f;
	if (euler.y < 0.0f) euler.y = 359.5f;
	*/

	//printf("[game] currentlyRenderingCam euler: %f %f %f\n", this->euler.x, this->euler.y, this->euler.z);

	CameraMoveForward(&rayCam, inNewPosDelta.x, false);
	CameraMoveRight(&rayCam, inNewPosDelta.z, false);

	rayCam.target = Vector3{ rayCam.target.x, rayCam.target.y + inNewPosDelta.y, rayCam.target.z };
	rayCam.position = Vector3{ rayCam.position.x, rayCam.position.y + inNewPosDelta.y, rayCam.position.z };

	transform.pos = rayCam.position;
	editor_camera_update_model_rotation();

}

/*
*
*
* EDITOR FUNCTION DEFINITIONS
*
*
*/

#ifdef DEBUG

entt* entt_camera::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#else

entt* entt_camera::editor_try_select(Ray inRay, RayCollision inRayCollision)
{
	return nullptr;
};

#endif
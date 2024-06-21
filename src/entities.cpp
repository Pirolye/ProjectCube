#include "world.h"


#define RLIGHTS_IMPLEMENTATION


#include "raylib.h"
#include "rlights.h"
#include "raymath.h"
#include "rcamera.h"

void on_make(entity* inEntity)
{
	if (inEntity->type == "camera")
	{
		entity_camera_data* inData = new entity_camera_data;
		inEntity->data = inData;

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
	
	if (inEntity->type == "light")
	{
		entity_light_data* inData = new entity_light_data;
		inEntity->data = inData;
		
		for (int i = 0; i != 360; i++)
		{
			inData->rayLight[i] = { 0 };
		}

		for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
		{
			if (inEntity->containingWorld->currentlyLoadedShaders[i].id != 0 && inEntity->containingWorld->currentlyLoadedShaders[i].locs != 0)
			{
				for (int i2 = 0; i2 != 360; i2++)
				{
					if (inData->rayLight[i2].enabled != true)
					{
						inData->rayLight[i2] = CreateLight(LIGHT_POINT, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3Zero(), WHITE, inEntity->containingWorld->currentlyLoadedShaders[i]);
						break;
					}
					else
					{
						continue;
					}
				}
			}
		}

		inEntity->transform.pos = Vector3Zero();
		inEntity->transform.rot = graphene_quaternion_alloc();
		graphene_quaternion_init_identity(inEntity->transform.rot);
		inEntity->transform.scale = Vector3{ 1.0f, 1.0f, 1.0f };

		#ifdef DEBUG
			inData->debugModel = load_model("editor/point_light_model.obj", inEntity);
		#endif

	}

	if (inEntity->type == "maincube")
	{
		entity_maincube_data* inData = new entity_maincube_data;
		inEntity->data = inData;

		inData->cubeModel = load_model("content/model/smallCube/smallCube.obj", inEntity);                 // Load model
		inData->cubeTexture = LoadTexture("content/model/smallCube/smallCube_albedo.png"); // Load model texture
		inData->cubeModel->model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = inData->cubeTexture;

		inData->cubeShader = world_make_shader(inEntity->containingWorld, "content/model/smallCube/base_lighting.vs", "content/model/smallCube/smallCube_lighting.fs");
		inData->cubeShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(inData->cubeShader, "matModel");
		int ambientLoc = GetShaderLocation(inData->cubeShader, "ambient");
		SetShaderValue(inData->cubeShader, ambientLoc, inEntity->containingWorld->defaultAmbientLightValue, SHADER_UNIFORM_VEC4);
		inData->cubeModel->model.materials[0].shader = inData->cubeShader;

		inData->collisionBox = new dynamic_body(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3{ 0.0f, 0.0f, 0.0f }, inEntity->containingWorld->gScene, inEntity->containingWorld);

		inEntity->transform.rot = graphene_quaternion_alloc();
		graphene_quaternion_init_identity(inEntity->transform.rot);
		//update_spatial_props(inEntt, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });

		update_spatial_properties(inEntity, Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 1.0f, 1.0f });
		inData->collisionBox->enable();

	}
}

void on_update(entity* inEntity)
{
	if (inEntity->type == "camera")
	{
		entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);
		
		if (inEntity != inEntity->containingWorld->currentlyRenderingCamera) return;

		if (inData->mode == 1)
		{
			//(Levente): WARNING! I had to modify rcore.c in order to not put the mouse cursor back in the middle every time you enabled/disabled it! Also, I have to separately get the mouse delta every time I need it, using a variable such as diff doesn't work for some reason.

			if (!inEntity->containingWorld->worldEditor->canManipulateWorld) return;

			Vector2 c = GetMousePosition();

			bool canMove = false;

			Vector2 diff1 = GetMouseDelta();

			Vector2 diff = Vector2{ diff1.x / 2.0f, diff1.y / 2.0f };

			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				DisableCursor();

				transform_camera_by_delta(inEntity, Vector3Zero(), Vector3{ diff.x, diff.y, 0.0f });

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

			if (IsKeyDown(KEY_W)) transform_camera_by_delta(inEntity, Vector3{ inData->moveSpeed, 0.0f, 0.0f }, Vector3Zero());
			if (IsKeyDown(KEY_S)) transform_camera_by_delta(inEntity, Vector3{ -(inData->moveSpeed), 0.0f, 0.0f }, Vector3Zero());
			if (IsKeyDown(KEY_D)) transform_camera_by_delta(inEntity, Vector3{ 0.0f, 0.0f, inData->moveSpeed }, Vector3Zero());
			if (IsKeyDown(KEY_A)) transform_camera_by_delta(inEntity, Vector3{ 0.0f, 0.0f, -(inData->moveSpeed) }, Vector3Zero());
			if (IsKeyDown(KEY_E)) transform_camera_by_delta(inEntity, Vector3{ 0.0f, inData->moveSpeed, 0.0f }, Vector3Zero());
			if (IsKeyDown(KEY_Q)) transform_camera_by_delta(inEntity, Vector3{ 0.0f, -(inData->moveSpeed), 0.0f }, Vector3Zero());

		}
		else if (inData->mode == 0)
		{
			return;
		}
		else
		{
			return;
		}


	}

	if (inEntity->type == "maincube")
	{
		entity_maincube_data* inData = reinterpret_cast<entity_maincube_data*>(inEntity->data);

		if (inEntity->containingWorld->worldEditor->isInEditorMode == false)
		{
			inData->collisionBox->update();
			update_spatial_properties(inEntity, inData->collisionBox->t.pos, inData->collisionBox->t.scale, inData->collisionBox->t.rot);
		}
		else
		{
			if (inEntity->containingWorld->worldEditor->currentlySelectedEntity == inEntity)
			{
				if (IsKeyPressed(KEY_X))
				{
					inData->collisionBox->enable();
				}

			}

		}


	}


}
void on_draw_2d(entity* inEntity)
{
	if (inEntity->type == "camera")
	{
		entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);
		if (inEntity != inEntity->containingWorld->currentlyRenderingCamera) return;
		DrawFPS(10, 20);

	}


}
void on_draw_3d(entity* inEntity)
{
	if (inEntity->type == "camera")
	{
		entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);

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
	
	if (inEntity->type == "light")
	{
		entity_light_data* inData = reinterpret_cast<entity_light_data*>(inEntity->data);

		if (inEntity->containingWorld->worldEditor->isInEditorMode)
		{
			draw_model(inData->debugModel, YELLOW);
		}
	}

	if (inEntity->type == "maincube")
	{
		entity_maincube_data* inData = reinterpret_cast<entity_maincube_data*>(inEntity->data);

		draw_model(inData->cubeModel);

	}
}

void update_spatial_properties(entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation)
{
	if (inEntity->type == "camera")
	{
		entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);
		float x, y, z;

		graphene_quaternion_to_angles(inNewRotation, &x, &y, &z);

		transform_camera_by_delta(inEntity, Vector3{ inNewPos.z - inEntity->transform.pos.z, inNewPos.y - inEntity->transform.pos.y, -1.0f * (inNewPos.x - inEntity->transform.pos.x) }, Vector3{ x, y, z });

	}
	
	if (inEntity->type == "light")
	{
		entity_light_data* inData = reinterpret_cast<entity_light_data*>(inEntity->data);

		inEntity->transform.pos = inNewPos;
		inEntity->transform.scale = inNewScale;
		inEntity->transform.rot = inNewRotation;

		Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
		float x, y, z;
		graphene_quaternion_to_radians(inEntity->transform.rot, &x, &y, &z);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });
		Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

		inData->debugModel->model.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		update_light_properties(inEntity, inData->rayLight->type, inEntity->transform.pos, Vector3Zero(), inData->rayLight->color);

	}

	if (inEntity->type == "maincube")
	{
		entity_maincube_data* inData = reinterpret_cast<entity_maincube_data*>(inEntity->data);

		inEntity->transform.pos = inNewPos;
		inEntity->transform.scale = inNewScale;
		graphene_quaternion_init_from_quaternion(inEntity->transform.rot, inNewRotation);

		Matrix matScale = MatrixScale(inEntity->transform.scale.x, inEntity->transform.scale.y, inEntity->transform.scale.z);
		Matrix matTranslation = MatrixTranslate(inEntity->transform.pos.x, inEntity->transform.pos.y, inEntity->transform.pos.z);
		float x, y, z;
		graphene_quaternion_to_radians(inEntity->transform.rot, &x, &y, &z);
		Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });

		inData->cubeModel->model.transform = MatrixIdentity();
		inData->cubeModel->model.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

		inData->collisionBox->update_spatial_props(inNewPos, inNewScale, inNewRotation);

	}
}

void update_spatial_properties(entity* inEntity, Vector3 inNewPos, Vector3 inNewScale)
{
	if (inEntity->type == "maincube")
	{
		entity_maincube_data* inData = reinterpret_cast<entity_maincube_data*>(inEntity->data);

		inEntity->transform.pos = inNewPos;
		inEntity->transform.scale = inNewScale;

		Matrix matScale = MatrixScale(inEntity->transform.scale.x, inEntity->transform.scale.y, inEntity->transform.scale.z);
		Matrix matTranslation = MatrixTranslate(inEntity->transform.pos.x, inEntity->transform.pos.y, inEntity->transform.pos.z);
		inData->cubeModel->model.transform = MatrixIdentity();
		inData->cubeModel->model.transform = MatrixMultiply(matScale, matTranslation);

		inData->collisionBox->update_spatial_props(inNewPos, inNewScale);
	}
	if (inEntity->type == "light")
	{
		entity_light_data* inData = reinterpret_cast<entity_light_data*>(inEntity->data);

		inEntity->transform.pos = inNewPos;
		inEntity->transform.scale = inNewScale;

		Matrix matScale = MatrixScale(inNewScale.x, inNewScale.y, inNewScale.z);
		Matrix matTranslation = MatrixTranslate(inNewPos.x, inNewPos.y, inNewPos.z);

		inData->debugModel->model.transform = MatrixMultiply(matScale, matTranslation);

		update_light_properties(inEntity, inData->rayLight->type, inEntity->transform.pos, Vector3Zero(), inData->rayLight->color);

	}

}

void update_light_properties(entity* inEntity, int inType, Vector3 inPos, Vector3 inTarget, Color inColor)
{
	if (inEntity->type != "light") return;

	entity_light_data* inData = reinterpret_cast<entity_light_data*>(inEntity->data);

	for (int i2 = 0; i2 != 360; i2++)
	{
		if (inData->rayLight[i2].enabled == true)
		{
			inData->rayLight[i2].type = inType;
			inData->rayLight[i2].position = inPos;
			inData->rayLight[i2].target = inTarget;
			inData->rayLight[i2].color = inColor;
		}
		else
		{
			continue;
		}
	}

	inEntity->transform.pos = inPos;
	//inEntity->transform.scale = inScale;
	//inEntity->transform.rot = inNewRotation;

	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	float x, y, z;
	graphene_quaternion_to_radians(inEntity->transform.rot, &x, &y, &z);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ x, y , z });
	Matrix matTranslation = MatrixTranslate(inPos.x, inPos.y, inPos.z);

	inData->debugModel->model.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		for (int i2 = 0; i2 != 360; i2++)
		{
			if (inData->rayLight[i2].enabled == true)
			{
				if (inEntity->containingWorld->currentlyLoadedShaders[i].id != 0 && inEntity->containingWorld->currentlyLoadedShaders[i].locs != 0)
				{
					UpdateLightValues(inEntity->containingWorld->currentlyLoadedShaders[i], inData->rayLight[i2]);
				}

			}
		}
	}

}

void set_camera_mode(entity* inEntity, int inMode, bool isForEditorOnly)
{
	if (inEntity->type != "camera") return;

	entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);


	if (inMode != 1 && inMode != 0)
	{
		printf("[game] ERROR: Tried to set camera.mode to value <<%d>> (invalid, only <<1>> and <<0>> is valid) on camera %s!\n", inMode, inEntity->id.c_str());
		return;
	}

	inData->mode = inMode;
	inData->isForEditorOnly = isForEditorOnly;

}

void transform_camera_by_delta(entity* inEntity, Vector3 inNewPosDelta, Vector3 inNewRotDelta)
{
	if (inEntity->type != "camera") return;

	entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);
	
	UpdateCameraPro(&(inData->rayCam), Vector3Zero(), inNewRotDelta, 0.0f);

	inData->euler.x = inData->euler.x + inNewRotDelta.y;
	inData->euler.y = inData->euler.y - inNewRotDelta.x;

	if (inData->euler.x > 90.0f) inData->euler.x = 90.0f;
	if (inData->euler.x < -90.0f) inData->euler.x = -90.0f;

	/*
	if (euler.y > 359.5f) euler.y = 0.0f;
	if (euler.y < 0.0f) euler.y = 359.5f;
	*/

	//printf("[game] currentlyRenderingCam euler: %f %f %f\n", inEntt->euler.x, inEntt->euler.y, inEntt->euler.z);

	CameraMoveForward(&(inData->rayCam), inNewPosDelta.x, false);
	CameraMoveRight(&(inData->rayCam), inNewPosDelta.z, false);

	inData->rayCam.target = Vector3{ inData->rayCam.target.x, inData->rayCam.target.y + inNewPosDelta.y, inData->rayCam.target.z };
	inData->rayCam.position = Vector3{ inData->rayCam.position.x, inData->rayCam.position.y + inNewPosDelta.y, inData->rayCam.position.z };

	inEntity->transform.pos = inData->rayCam.position;
	editor_camera_update_model_rotation(inEntity);

}

void editor_camera_update_model_rotation(entity* inEntity)
{
	if (inEntity->type != "camera") return;

	entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);


	Matrix matScale = MatrixScale(inEntity->transform.scale.x, inEntity->transform.scale.y, inEntity->transform.scale.z);
	Matrix matTranslation = MatrixTranslate(inEntity->transform.pos.x, inEntity->transform.pos.y, inEntity->transform.pos.z - 1.0f);

	Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD * inData->euler.x, DEG2RAD * inData->euler.y, DEG2RAD * 0.0f });

	inData->cameraEditorModel.transform = MatrixIdentity();
	inData->cameraEditorModel.transform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

}

void on_destroy(entity* inEntity)
{
	if (inEntity->type == "camera")
	{
		entity_camera_data* inData = reinterpret_cast<entity_camera_data*>(inEntity->data);
		UnloadModel(inData->cameraEditorModel);

	}
	if (inEntity->type == "maincube")
	{
		entity_maincube_data* inData = reinterpret_cast<entity_maincube_data*>(inEntity->data);
	
		UnloadTexture(inData->cubeTexture);
		unload_model(inData->cubeModel);
		UnloadShader(inData->cubeShader); // REVISIT!!!!

		graphene_quaternion_free(inEntity->transform.rot);

		//delete body; WILL BE REVISITED
	}
	if (inEntity->type == "light")
	{
		entity_light_data* inData = reinterpret_cast<entity_light_data*>(inEntity->data);

		unload_model(inData->debugModel);

	}
}

entity* editor_try_select(entity* inEntity);


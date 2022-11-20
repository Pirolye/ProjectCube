#include "entt_camera.h"
#include "entt_maincube.h"

#include "game_instance.h"

;
#include "assert.h"

#define FLT_MAX 340282346638528859811704183484516925440.0f 


world::world(game_instance* inGameInstance, PxPhysics* inPhysicsMemAddress) 
{
	//(Levente): Okay... this is clever but not very logical. Apparently 0xcdcdcd... is not a nullptr so we individually assign NULL to every uninitialized entt in the array!
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		entityArray[i] = NULL;
	}

	//(Levente): Same for the shaders.
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		currentlyLoadedShaders[i] = { 0 };
	}
	
	gameInstance = inGameInstance;
	assert(gameInstance != nullptr);

	name = "debug";
	gameCameraPosition = Vector3{ 10.0f, 0.0f, 0.0f };

	globalPhysics = inPhysicsMemAddress; // (Levente): We do this because for some reason the program
										 // can't read gPhysics static var properly. (Even though it can read
										 // gFoundation just fine...)

	PxSceneDesc sceneDesc(globalPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gameInstance->gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gameInstance->gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = globalPhysics->createScene(sceneDesc);

	//physicsSpace = new q3Scene(1.0 / 60);
	//physicsSpace->SetGravity(q3Vec3(0.0, -1.0, 0.0));

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	editorGizmoMoveAxisX = LoadModel("editor/gizmo_move_axis.obj");
	editorGizmoMoveAxisY = LoadModel("editor/gizmo_move_axis.obj");
	editorGizmoMoveAxisZ = LoadModel("editor/gizmo_move_axis.obj");
	editorGizmoMoveAxisMat = LoadTexture("editor/gizmo_move_axis_albedo.png");
	editorGizmoMoveAxisX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = editorGizmoMoveAxisMat;
	editorGizmoMoveAxisY.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = editorGizmoMoveAxisMat;
	editorGizmoMoveAxisZ.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = editorGizmoMoveAxisMat;

	editorGizmoHelperMesh = GenMeshPlane(99999.0f, 99999.0f, 10, 10);
	editorGizmoHelperModel = LoadModelFromMesh(editorGizmoHelperMesh);

	run_script_on_init();

}

world::~world() //(Levente): Technically this is really bad. We will need a proper shutdown procedure!
{
	/*
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != nullptr)
		{
			entityArray[i]->on_destroy();
			delete entityArray[i];
		}
		else
		{
			continue;
		}
	}
	*/

}

Shader world::make_shader(const char* vertexShader, const char* fragmentShader)
{
	Shader temp = LoadShader(vertexShader, fragmentShader);

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD * 2; i++)
	{
		if (currentlyLoadedShaders[i].id == 0 && currentlyLoadedShaders[i].locs == 0)
		{
			currentlyLoadedShaders[i] = temp;
			break;
		}
	}

	return temp;
}

entt* world::make_desired_entt(entts inDesiredEntt)
{
	switch (inDesiredEntt)
	{
		case entts::cam:
		{
			bool isThereAnotherCameraInTheWorld = false;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] != NULL)
				{
					isThereAnotherCameraInTheWorld = dynamic_cast<entt_camera*>(entityArray[i]);
					break;
				}
			}

			assert(isThereAnotherCameraInTheWorld != true);
			
			entt_camera* cam = new entt_camera;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			cam->id = "entt_camera_" + std::to_string(totalMadeEntts);
			cam->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = cam;
					break;
				}
			}

			cam->on_make();

			return cam;

			break;
		}

		case entts::mainCube:
		{
			entt_maincube* c = new entt_maincube;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			c->id = "entt_maincube_" + std::to_string(totalMadeEntts);
			c->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = c;
					break;
				}
			}

			c->on_make(); 
			
			return c;

			break;
		}
		
		case entts::mainCube_Static:
		{
			entt_maincube_static* c = new entt_maincube_static;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			c->id = "entt_maincube_static_" + std::to_string(totalMadeEntts);
			c->containingWorld = this;

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = c;
					break;
				}
			}

			c->on_make();

			return c;

			break;
		}
		case entts::light:
		{
			entt_light* l1 = new entt_light;

			totalMadeEntts = totalMadeEntts + 1;
			entityArrayCurrentSize = entityArrayCurrentSize + 1;

			l1->id = "entt_point_light_" + std::to_string(totalMadeEntts);
			l1->containingWorld = this;

			l1->on_make();

			for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
			{
				if (entityArray[i] == NULL)
				{
					entityArray[i] = l1;
					break;
				}
			}

			return l1;

			break;
		}


		default:
		{
			return nullptr;
		}

	}
}

void world::update()
{
	assert(dynamic_cast<entt_camera*>(entityArray[0]) != nullptr && "The/a camera should always exist at entity array index 0!");
	if(isInEditorMode) UpdateCamera(dynamic_cast<entt_camera*>(entityArray[0])->rayCam);

	cameraSwitchedLastFrame = false;
	if (GetFrameTime() > 0)
	{
		
		if (!isInEditorMode) 
		{
			gScene->simulate(1.0f / 60.0f);
			gScene->fetchResults(true);
		}
		

		//gScene->simulate(1.0f / 60.0f);
		//gScene->fetchResults(true);
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_update();
		}
	}

	if (IsKeyPressed(KEY_F1))
	{
		#ifdef DEBUG 
		if (isInEditorMode) exit_editor_mode();
		else enter_editor_mode();
		#else
		
		#endif
	}

	if(isInEditorMode) update_world_editor();

	run_script_on_update();


}

void world::draw_all()
{
	BeginDrawing();

	BeginMode3D(*(dynamic_cast<entt_camera*>(entityArray[0])->rayCam));
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_3d();
		}
	}

	//DrawModel(editorGizmoHelperModel, Vector3Zero(), 1.0f, RED);
	if (isInEditorMode && editorCurrentlySelectedEntt != nullptr) editor_draw_gizmo(editorCurrentlySelectedEntt->enttTransform.pos);

	EndMode3D();

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_draw_2d();
		}
	}

	if (isInEditorMode)
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
	}

	if (isInEditorMode)
	{
		if (editorInfo.selectingGizmoMoveAxisX) DrawText("Moving X gizmo", 0, 150, 30, WHITE);
		if (editorInfo.selectingGizmoMoveAxisY) DrawText("Moving Y gizmo", 0, 150, 30, WHITE);
		if (editorInfo.selectingGizmoMoveAxisZ) DrawText("Moving Z gizmo", 0, 150, 30, WHITE);
	}

	EndDrawing();

}

void world::on_destroy()
{
	run_script_on_destroy();
	
	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			entityArray[i]->on_destroy();
		}
	}

	for (int i = 0; i != MAX_ENTITIES_IN_WORLD; i++)
	{
		if (entityArray[i] != NULL)
		{
			delete entityArray[i];
		}
	}
	
}

/*
* 
* 
* EDITOR FUNCTION DEFINITIONS
* 
* 
*/

#ifdef DEBUG

void world::enter_editor_mode()
{
	isInEditorMode = true;

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
	if(editorCurrentlySelectedEntt != nullptr) editor_check_against_gizmo(editorCurrentlySelectedEntt->enttTransform.pos);
	
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
			//if (dynamic_cast<entt_maincube_static*>(entityArray[i]) != nullptr) editorCurrentlySelectedEntt = dynamic_cast<entt_maincube_static*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);
			if (dynamic_cast<entt_camera*>(entityArray[i]) != nullptr) editorCurrentlySelectedEntt = dynamic_cast<entt_camera*>(entityArray[i])->editor_try_select(cursorSelectionRay, collision);

			if (editorCurrentlySelectedEntt != nullptr) break;
			else continue;
		}
	}
};

void world::editor_draw_gizmo(Vector3 inCenterPos)
{
	Vector3 v{ 1.0f, 0.0f, 0.0f };
	
	Matrix matScaleX = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotationX = MatrixRotateXYZ(Vector3{ 0.0f, 0.0f, DEG2RAD*90.0f });
	Matrix matTranslationX = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
	editorGizmoMoveAxisX.transform = MatrixMultiply(MatrixMultiply(matScaleX, matRotationX), matTranslationX);

	Matrix matScaleY = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotationY = MatrixRotateXYZ(Vector3{ DEG2RAD * 180.0f, 0.0f , 0.0f });
	Matrix matTranslationY = MatrixTranslate(inCenterPos.x, inCenterPos.y, inCenterPos.z);
	editorGizmoMoveAxisY.transform = MatrixMultiply(MatrixMultiply(matScaleY, matRotationY), matTranslationY);

	Matrix matScaleZ = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotationZ = MatrixRotateXYZ(Vector3{ DEG2RAD*90.0f, 0.0f , 0.0f });
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
	if (!isInEditorMode) return;
	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		
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
				editorInfo.selectingGizmoMoveAxisX = false;
				//editorInfo.canSelectEntt = true;
					
			}
		}

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
				editorInfo.selectingGizmoMoveAxisY = false;
				//editorInfo.canSelectEntt = true;
					
			}
		}
			
			
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
				editorInfo.selectingGizmoMoveAxisZ = false;
				//editorInfo.canSelectEntt = true;
					
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
	SetCameraMode( *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam), CAMERA_CUSTOM); // Set a free camera mode
	
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
			else
			{
				//editorInfo.selectingGizmoMoveAxisX = false;
				//editorInfo.canSelectEntt = true;

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

				/*
				if (diff < 0.0f)
				{
					newPos = Vector3{ enttToMove->enttTransform.pos.x + diff, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z };
					//diff = fabs(diff);
				}
				else
				{

				}*/

				newPos = Vector3{ enttToMove->enttTransform.pos.x - diff, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);


				//float diff = fabs(enttToMove->enttTransform.pos.x - meshHitInfo.point.x);
				//float diff = fabs(meshHitInfo.point.x - enttToMove->enttTransform.pos.x);


				break;
				return;
			}
			else
			{
				//editorInfo.selectingGizmoMoveAxisX = false;
				//editorInfo.canSelectEntt = true;

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
			else
			{
				//editorInfo.selectingGizmoMoveAxisX = false;
				//editorInfo.canSelectEntt = true;

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

				/*
				if (diff < 0.0f)
				{
					newPos = Vector3{ enttToMove->enttTransform.pos.x + diff, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z };
					//diff = fabs(diff);
				}
				else
				{

				}*/

				newPos = Vector3{ enttToMove->enttTransform.pos.x, enttToMove->enttTransform.pos.y - diff, enttToMove->enttTransform.pos.z };

				enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);


				//float diff = fabs(enttToMove->enttTransform.pos.x - meshHitInfo.point.x);
				//float diff = fabs(meshHitInfo.point.x - enttToMove->enttTransform.pos.x);


				break;
				return;
			}
			else
			{
				//editorInfo.selectingGizmoMoveAxisX = false;
				//editorInfo.canSelectEntt = true;

			}
		}
	}
	else if (inAxis == 2)
	{
	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotation = MatrixRotateXYZ(Vector3{ DEG2RAD*180.0f, 0.0f, 0.0f });
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
		else
		{
			//editorInfo.selectingGizmoMoveAxisX = false;
			//editorInfo.canSelectEntt = true;

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

			/*
			if (diff < 0.0f)
			{
				newPos = Vector3{ enttToMove->enttTransform.pos.x + diff, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z };
				//diff = fabs(diff);
			}
			else
			{

			}*/

			newPos = Vector3{ enttToMove->enttTransform.pos.x, enttToMove->enttTransform.pos.y, enttToMove->enttTransform.pos.z - diff };

			enttToMove->update_spatial_props(newPos, enttToMove->enttTransform.scale, enttToMove->enttTransform.rot);


			//float diff = fabs(enttToMove->enttTransform.pos.x - meshHitInfo.point.x);
			//float diff = fabs(meshHitInfo.point.x - enttToMove->enttTransform.pos.x);


			break;
			return;
		}
		else
		{
			//editorInfo.selectingGizmoMoveAxisX = false;
			//editorInfo.canSelectEntt = true;

		}
	}
	}

	SetCameraMode( *(dynamic_cast<entt_camera*>(entityArray[0])->rayCam), CAMERA_FREE); // Set a free camera mode
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
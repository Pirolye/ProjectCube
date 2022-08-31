/*******************************************************************************************
*
*  Missing Cube
* 
*  A game by Levente Biro (C) 2022. Uses raylib. (rlgl.h, rcamera.h is modified!)
*
********************************************************************************************/

#include "game_instance.h"

int main()
{

	game_instance* currentInstance = new game_instance("Missing Cube", 1600, 900);

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		currentInstance->update();
		currentInstance->draw();
	}

	currentInstance->on_exit();
	delete currentInstance;

	return 0;

}

/*
#include "raylib.h"
#include "raymath.h"



#include "rlgl.h"

#include "world.h"
#include "entt_camera.h"
#include "entt_maincube.h"
*/
/*
//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static Camera3D camera = { 0 };
static Model mainCube;
static Vector2 mousePosChange;
static Vector3 cubeRot;

static void UpdateDrawFrame(void);          // Update and draw one frame

int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

    // Initialization
    InitWindow(1600, 900, "Missing Cube -- WORKING TITLE");

    InitAudioDevice();      // Initialize audio device

    
	camera.position = Vector3{ 4.0f, 0.0f, 0.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                                // Camera field-of-view 
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    SetCameraMode(camera, CAMERA_CUSTOM); // Set a free camera mode

    mainCube            = LoadModel("game/content/model/smallCube/smallCube.obj");
    Texture2D mainCube_albedo = LoadTexture("game/content/model/smallCube/smallCube_albedo.png");

	// Load basic lighting shader
	Shader smallCube_shader = LoadShader("game/content/model/smallCube/base_lighting.vs", "game/content/model/smallCube/smallCube_lighting.fs");
	// Get some required shader locations
	smallCube_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(smallCube_shader, "viewPos");
	// NOTE: "matModel" location name is automatically assigned on shader loading, 
	// no need to get the location again if using that uniform name
	//shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

	// Ambient light level (some basic lighting)
	int ambientLoc = GetShaderLocation(smallCube_shader, "ambient");
	float defaultAmbientLightValue[4] = {0.1f, 0.1f, 0.1f, 1.0f};
	SetShaderValue(smallCube_shader, ambientLoc, defaultAmbientLightValue, SHADER_UNIFORM_VEC4);

	// Assign out lighting shader to model
	//mainCube.materials[0].shader = smallCube_shader;
	//cube.materials[0].shader = shader;

	// Create lights
	Light lights[MAX_LIGHTS] = { 0 };
	lights[0] = CreateLight(LIGHT_POINT, Vector3{ -2, 1, -2 }, Vector3Zero(), WHITE, smallCube_shader);
	lights[1] = CreateLight(LIGHT_POINT, Vector3{ 5, 5, 5 }, Vector3Zero(), WHITE, smallCube_shader);


    //(levente): This will be where we load the shader!
    //Shader smallCube_shader = LoadShader(0, TextFormat("game/content/model/smallCube/smallCube_standard.fs", 330));

	mainCube.materials[0].shader = smallCube_shader;                     // Set shader effect to 3d model
	mainCube.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = mainCube_albedo; // Bind texture to model

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
 
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

	UnloadShader(smallCube_shader);       // Unload shader
	UnloadTexture(mainCube_albedo);     // Unload texture
	UnloadModel(mainCube);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
  
    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
	

    UpdateCamera(&camera);

    //UpdateMusicStream(music);  // This is used when we need to manually increment the music player. No use when no music is playing.

	if (cubeRot.x == 360.0f) cubeRot.x = 0.0f;
	if (cubeRot.y == 360.0f) cubeRot.y = 0.0f;
	if (cubeRot.z == 360.0f) cubeRot.z = 0.0f;

	if (IsKeyDown(KEY_W))
	{

		//Vector2 delta = GetMouseDelta();
		//Vector2 mousePos = GetMousePosition();
		//SetMousePosition(mousePos.x - delta.x, mousePos.y - delta.y);


		cubeRot.x += 5.0f;
		//cubeRot.y += 5.0f;
		
		// Okay so the problem is that when we rotate the cube we also rotate the axis' with it. Meaning if we rotate A and B by 90 degrees A and B will no longer be rotatebable to the 
		// previous angle by the same mouse movement. I need to find a way to compensate the rotation of the axis'. 

		/*if ( abs(delta.x) > 0.5f && abs(delta.y) > 0.5f)
		{
			float xs = (delta.x * delta.x) * 1.0f;
			float ys = (delta.y * delta.y) * 1.0f;

			float zs = xs + ys;

			float z = sqrt(zs);

			cubeRot.z += z * 0.2f;
		}*/
		
	/*
	}
	else if (IsKeyDown(KEY_S))
	{
		cubeRot.x -= 5.0f;
	}
	else if (IsKeyDown(KEY_A))
	{
		cubeRot.y += 5.0f;
	}
	else if (IsKeyDown(KEY_D))
	{
		cubeRot.y -= 5.0f;
	}


	mainCube.transform = MatrixRotateXYZ(Vector3{ DEG2RAD* cubeRot.x, DEG2RAD* cubeRot.y, DEG2RAD* cubeRot.z });


    BeginDrawing();

		rlEnableBackfaceCulling();

        ClearBackground(BLACK);

        BeginMode3D(camera);

		rlEnableFrontfaceCulling();
		DrawModel(mainCube, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
		rlDisableFrontfaceCulling();
		rlEnableBackfaceCulling();

            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

    EndDrawing();
}
*/
;

/*
static world* persistentWorld;

static void update();
static void draw();

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

	// Initialization
	InitWindow(1600, 900, "Missing Cube -- WORKING TITLE");
	InitAudioDevice();      // Initialize audio device
	SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

	persistentWorld = new world;
	
	entt* mainCam = persistentWorld->make_desired_entt(entts::camSetCurrentlyRendering);
	entt* Cam1 = persistentWorld->make_desired_entt(entts::cam);
	entt* mainCube = persistentWorld->make_desired_entt(entts::mainCube);
	entt* mainLight = persistentWorld->make_desired_entt(entts::light);
	entt* secondaryLight = persistentWorld->make_desired_entt(entts::light);

	entt* mainCube2 = persistentWorld->make_desired_entt(entts::mainCube);
	
	dynamic_cast<entt_light*>(secondaryLight)->update_light_props(1, Vector3{ 7.0f, 7.0f, 7.0f }, Vector3Zero(), WHITE);
	dynamic_cast<entt_light*>(mainLight)->update_light_props(1, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero(), WHITE);
	dynamic_cast<entt_maincube*>(mainCube2)->pos = Vector3{ 3.0f, 3.0f, 3.0f };

	int i = 0;

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		persistentWorld->update();
		persistentWorld->draw_all();

		if (IsKeyPressed(KEY_Z))
		{
			if (i == 0)
			{
				dynamic_cast<entt_light*>(mainLight)->update_light_props(1, Vector3{ 4.0f, 4.0f, 4.0f }, Vector3Zero(), WHITE);
				i = 1;
			}

			else if (i == 1)
			{
				dynamic_cast<entt_light*>(mainLight)->update_light_props(1, Vector3{ 1.0f, 1.0f, 1.0f }, Vector3Zero(), WHITE);
				i = 0;
			}

		}
	}

	persistentWorld->on_destroy();

	CloseAudioDevice();     // Close audio context

	CloseWindow();          // Close window and OpenGL context

	return 0;

}
*/





	/*
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

	// Initialization
	InitWindow(1600, 900, "Missing Cube -- WORKING TITLE");

	InitAudioDevice();      // Initialize audio device

	bool selected = false;

	Camera3D camera = { 0 };

	camera.position = Vector3{ 4.0f, 0.0f, 0.0f }; // Camera position
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 70.0f;                                // Camera field-of-view 
	camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

	SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

	Model mainCube = LoadModel("../../game/content/model/smallCube/smallCube.obj");

	SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateCamera(&camera);

		BeginDrawing();

//		rlEnableBackfaceCulling();

		ClearBackground(BLACK);

		BeginMode3D(camera);

	//	rlEnableFrontfaceCulling();
		DrawModel(mainCube, Vector3{ 2.0f, 2.0f, 2.0f }, 1.5f, WHITE);
		if (selected) DrawModelWires(mainCube, Vector3{ 2.0f, 2.0f, 2.0f }, 1.5f, RED);
	//	rlDisableFrontfaceCulling();
	//	rlEnableBackfaceCulling();

		DrawGrid(10, 1.0f);

		EndMode3D();

		DrawFPS(10, 10);

		EndDrawing();

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			Ray cursorSelectionRay = GetMouseRay(GetMousePosition(), camera);

			RayCollision meshHitInfo = { 0 };
			for (int m = 0; m < mainCube.meshCount; m++)
			{
				// NOTE: We consider the model.transform for the collision check but 
				// it can be checked against any transform Matrix, used when checking against same
				// model drawn multiple times with multiple transforms
				meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, mainCube.meshes[m], mainCube.transform);
				if (meshHitInfo.hit)
				{
					// Save the closest hit mesh
					//inRayCollision = meshHitInfo;

					//return this;

					selected = true;

					break;  // Stop once one mesh collision is detected, the colliding mesh is m
				}
				else
				{
					//return nullptr;
					selected = false;
				}
			}

		}

	}

	//UnloadShader(smallCube_shader);       // Unload shader
	//UnloadTexture(mainCube_albedo);     // Unload texture
	UnloadModel(mainCube);

	CloseAudioDevice();     // Close audio context

	CloseWindow();          // Close window and OpenGL context

	return 0;
}


/*
#include "raylib.h"
#include "raymath.h"

int main()
{
	InitWindow(1600, 900, "Found Cube -- WORKING TITLE");

	Camera3D camera = { 0 };
	camera.position = Vector3{ 0.0f, 1.0f, -16.0f };
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	SetCameraMode(camera, CAMERA_FREE);

	Model mainCube = LoadModel("no file default cube");

	SetTargetFPS(60);

	float a = 0.0f;
	bool selected = false;

	while (!WindowShouldClose())
	{
		a += 0.005f;
		UpdateCamera(&camera);

		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode3D(camera);

		mainCube.transform = MatrixMultiply(MatrixTranslate(2, 0, 0), MatrixRotateXYZ(Vector3{ a, a * 1.2f, a * -2.2f }));

		DrawModel(mainCube, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
		if (selected) DrawModelWires(mainCube, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, RED);

		DrawGrid(10, 1.0f);

		EndMode3D();
		DrawText(TextFormat("selected %i", selected), 8, 28, 16, WHITE);
		DrawFPS(10, 10);

		EndDrawing();

		Ray cursorSelectionRay;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			cursorSelectionRay = GetMouseRay(GetMousePosition(), camera);
			selected = false;
			RayCollision meshHitInfo = { 0 };
			for (int m = 0; m < mainCube.meshCount; m++)
			{
				meshHitInfo = GetRayCollisionMesh(cursorSelectionRay, mainCube.meshes[m], mainCube.transform);
				if (meshHitInfo.hit)
				{
					selected = true;
					break;
				}
			}
		}
	}

	UnloadModel(mainCube);
	CloseWindow();
	return 0;
}
*/
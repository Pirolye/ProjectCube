/*******************************************************************************************
*
*  Missing Cube
* 
*  A game by Levente Biro (C) 2022. Uses raylib.
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static Camera3D camera = { 0 };
static Model mainCube;
static Vector2 mousePosChange;
static Vector2 cubeRot;

static void UpdateDrawFrame(void);          // Update and draw one frame

int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

    // Initialization
    InitWindow(1600, 900, "Missing Cube -- WORKING TITLE");

    InitAudioDevice();      // Initialize audio device

    
    camera.position = (Vector3){ 6.0f, 0.0f, 0.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                                // Camera field-of-view 
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    SetCameraMode(camera, CAMERA_CUSTOM); // Set a free camera mode

    mainCube            = LoadModel("game/content/model/smallCube/smallCube.obj");
    Texture2D mainCube_albedo = LoadTexture("game/content/model/smallCube/smallCube_albedo.png");

    //(levente): This will be where we load the shader!
    Shader smallCube_shader = LoadShader(0, TextFormat("game/content/model/smallCube/smallCube_standard.fs", 330));

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

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		HideCursor();

		Vector2 delta = GetMouseDelta();

		cubeRot.x = cubeRot.x + (delta.x / 10.0f);
		cubeRot.y = cubeRot.y + (delta.y / 10.0f);
	}

    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

		DrawModelEx(mainCube, (Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, (Vector3){}, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE);

            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);


	ShowCursor;

    EndDrawing();
}

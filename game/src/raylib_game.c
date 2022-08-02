/*******************************************************************************************
*
*  Missing Cube
* 
*  A game by Levente Biro (C) 2022. Uses raylib.
*
********************************************************************************************/

#include "raylib.h"

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

static void UpdateDrawFrame(void);          // Update and draw one frame

int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Missing Cube -- WORKING TITLE");

    InitAudioDevice();      // Initialize audio device

    
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    SetCameraMode(camera, CAMERA_CUSTOM); // Set a free camera mode

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
 
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
  
    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{

    UpdateCamera(&camera);

    //UpdateMusicStream(music);  // This is used when we need to manually increment the music player. No use when no music is playing.

    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

    EndDrawing();
}

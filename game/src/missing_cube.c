/*******************************************************************************************
*
*  Missing Cube
* 
*  A game by Levente Biro (C) 2022. Uses raylib. (rlgl.h is modified!)
*
********************************************************************************************/

#include "world.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "rlgl.h"

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
//static Camera3D camera = { 0 };
static Model mainCube;
static Vector2 mousePosChange;
static Vector3 cubeRot;

static world persistentWorld;

bool appShouldClose = false;

static void draw(void);    
static void update(void);  

int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)

    // Initialization
    InitWindow(1600, 900, "Missing Cube -- WORKING TITLE");

    InitAudioDevice();      // Initialize audio device
    
    //camera.position = (Vector3){ 4.0f, 0.0f, 0.0f }; // Camera position
    //camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    //camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    //camera.fovy = 70.0f;                                // Camera field-of-view 
    //camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    //SetCameraMode(camera, CAMERA_CUSTOM); // Set a free camera mode

	make_entt(CAMERA, (Vector3) { 4.0f, 0.0f, 0.0f }, persistentWorld);

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
	SetShaderValue(smallCube_shader, ambientLoc, (float[4]) { 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

	// Create lights
	Light lights[MAX_LIGHTS] = { 0 };
	lights[0] = CreateLight(LIGHT_POINT, (Vector3) { -2, 1, -2 }, Vector3Zero(), WHITE, smallCube_shader);
	lights[1] = CreateLight(LIGHT_POINT, (Vector3) { 5, 5, 5 }, Vector3Zero(), WHITE, smallCube_shader);

	mainCube.materials[0].shader = smallCube_shader;                     // Set shader effect to 3d model
	mainCube.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = mainCube_albedo; // Bind texture to model

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
 
    // Main game loop
    while (!WindowShouldClose() && !appShouldClose)    // Detect window close button or ESC key
    {
        update();
    }

	UnloadShader(smallCube_shader);       // Unload shader
	UnloadTexture(mainCube_albedo);     // Unload texture
	UnloadModel(mainCube);

    CloseAudioDevice();     // Close audio context

	empty_world(persistentWorld);

    CloseWindow();          // Close window and OpenGL context
  
    return 0;
}

static void update(void)
{
	step_world(persistentWorld);

	//UpdateCamera(&camera);

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


	mainCube.transform = MatrixRotateXYZ((Vector3) { DEG2RAD* cubeRot.x, DEG2RAD* cubeRot.y, DEG2RAD* cubeRot.z });

	draw();

}

static void draw(void)
{


}

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "physics.h"
#include "raylib.h"
#include <string>
#include "graphene.h"
#include <typeinfo>
#include "rlights.h"

struct world;

#define MAX_ENTITIES_IN_WORLD 1024

;
struct entt_transform
{
	Vector3 pos, scale;
	graphene_quaternion_t* rot;
};

struct entity_pointer
{
	void* entity;
	type_info type;
};

struct entity_info
{
	std::string id;
	world* containingWorld;

	entt_transform transform; // WARNING: Do not set manually! Always use functions!

	entity_pointer* thisInArray;


};

struct entt_maincube
{
	entity_info entityInfo;

	Model cubeModel;
	Texture2D cubeTexture;
	Shader cubeShader;

	dynamic_body* collisionBox;

};

void on_make(entt_maincube* inEntt);

void on_update(entt_maincube* inEntt);
void on_draw_2d(entt_maincube* inEntt);
void on_draw_3d(entt_maincube* inEntt);

void update_spatial_props(entt_maincube* inEntt, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation);
void update_spatial_props(entt_maincube* inEntt, Vector3 inNewPos, Vector3 inNewScale);

void on_destroy(entt_maincube* inEntt);

entt_maincube* editor_try_select(entt_maincube* inEntt);

struct entt_maincube_static
{
	entity_info entityInfo;

	Model cubeModel;
	Texture2D cubeTexture;
	Shader cubeShader;

	static_body* collisionBox;

};

void on_make(entt_maincube_static* inEntt);

void on_update(entt_maincube_static* inEntt);
void on_draw_2d(entt_maincube_static* inEntt);
void on_draw_3d(entt_maincube_static* inEntt);

void update_spatial_props(entt_maincube_static* inEntt, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation);
void update_spatial_props(entt_maincube_static* inEntt, Vector3 inNewPos, Vector3 inNewScale);

void on_destroy(entt_maincube_static* inEntt);

entt_maincube_static* editor_try_select(entt_maincube_static* inEntt);

struct entt_camera
{
	entity_info entityInfo;

	Camera3D rayCam;

	int mode; // 0 = locked (standard gameplay camera for missing cube) 1 = editor_free (free movement camera, standard 3d editor controls)
	bool isForEditorOnly;
	float moveSpeed;

	Vector3 euler; // Rotation represented as euler angles. For ease of use, everywhere else use quats!!


	Model cameraEditorModel;


};

void on_make(entt_camera* inEntt);

void on_update(entt_camera* inEntt);
void on_draw_2d(entt_camera* inEntt);
void on_draw_3d(entt_camera* inEntt);

void update_spatial_props(entt_camera* inEntt, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation);
void update_spatial_props(entt_camera* inEntt, Vector3 inNewPos, Vector3 inNewScale);
void set_mode(entt_camera* inEntt, int inMode, bool isForEditorOnly);
void transform_camera_by_delta(entt_camera* inEntt, Vector3 inPosition, Vector3 inRotation);
void editor_camera_update_model_rotation();

void on_destroy(entt_camera* inEntt);

entt_camera* editor_try_select(entt_camera* inEntt);


struct entt_light
{
	entity_info entityInfo;

	Light rayLight[360];

	Model debugModel;


};



#else

#endif

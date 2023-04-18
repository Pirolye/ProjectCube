#ifndef _H_ENTT_MAIN_CUBE_
#define _H_ENTT_MAIN_CUBE_

#include "world.h"

;
#include "physics.h"

;
struct entt_maincube : entt
{
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


// ------------------------------------------

struct entt_maincube_static : entt
{
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


#else

#endif
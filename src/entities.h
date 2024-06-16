#ifndef ENTITIES_H
#define ENTITES_H


#include "entity.h"
#include "physics.h"

#define RLIGHTS_IMPLEMENTATION

struct entity_maincube_data
{
	Model cubeModel;
	Texture2D cubeTexture;
	Shader cubeShader;

	dynamic_body* collisionBox;

};

void on_make(entity_maincube_data* inData, entity* inEntity);

void on_update(entity_maincube_data* inData, entity* inEntity);
void on_draw_2d(entity_maincube_data* inData, entity* inEntity);
void on_draw_3d(entity_maincube_data* inData, entity* inEntity);

void update_spatial_props(entity_maincube_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation);
void update_spatial_props(entity_maincube_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale);

void on_destroy(entity_maincube_data* inData, entity* inEntity);

entity* editor_try_select(entity_maincube_data* inData, entity* inEntity);

/*
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

entity_pointer* editor_try_select(entt_maincube_static* inEntt);

*/
struct entity_camera_data
{
	Camera3D rayCam;

	int mode; // 0 = locked (standard gameplay camera for missing cube) 1 = editor_free (free movement camera, standard 3d editor controls)
	bool isForEditorOnly;
	float moveSpeed;

	Vector3 euler; // Rotation represented as euler angles. For ease of use, everywhere else use quats!!


	Model cameraEditorModel;


};

void on_make(entity_camera_data* inData, entity* inEntity);

void on_update(entity_camera_data* inData, entity* inEntity);
void on_draw_2d(entity_camera_data* inData, entity* inEntity);
void on_draw_3d(entity_camera_data* inData, entity* inEntity);

void update_spatial_props(entity_camera_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation);
void update_spatial_props(entity_camera_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale);
void set_camera_mode(entity* inEntity, int inMode, bool isForEditorOnly);
void transform_camera_by_delta(entity_camera_data* inData, entity* inEntity, Vector3 inPosition, Vector3 inRotation);
void editor_camera_update_model_rotation();

void on_destroy(entity_camera_data* inData, entity* inEntity);

entity* editor_try_select(entity_camera_data* inData, entity* inEntity);


struct entity_light_data
{
	Light rayLight[360];

	Model debugModel;

	
};

void on_make(entity_light_data* inData, entity* inEntity);

void on_update(entity_light_data* inData, entity* inEntity);
void on_draw_2d(entity_light_data* inData, entity* inEntity);
void on_draw_3d(entity_light_data* inData, entity* inEntity);

void update_spatial_props(entity_light_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale, graphene_quaternion_t* inNewRotation);
void update_spatial_props(entity_light_data* inData, entity* inEntity, Vector3 inNewPos, Vector3 inNewScale);
void update_light_props(entity* inEntity, int inType, Vector3 inPos, Vector3 inTarget, Color inColor);

void on_destroy(entity_light_data* inData, entity* inEntity);

entity* editor_try_select(entity_light_data* inData, entity* inEntity);



#else

#endif
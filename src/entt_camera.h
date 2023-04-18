#ifndef _ENTT_CAM_H_
#define _ENTT_CAM_H_

#include "entt.h"

;
struct entt_camera : public entt
{
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


#else

#endif

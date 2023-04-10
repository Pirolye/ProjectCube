#ifndef _ENTT_CAM_H_
#define _ENTT_CAM_H_

#include "entt.h"

;
struct entt_camera : entt
{
	Camera3D rayCam;

	int mode; // 0 = locked (standard gameplay camera for missing cube) 1 = editor_free (free movement camera, standard 3d editor controls)
	bool isForEditorOnly;
	float moveSpeed;

	Vector3 euler; // Rotation represented as euler angles. For ease of use, everywhere else use quats!!

	using entt::entt;

	entt_camera() {};
	~entt_camera() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;

	void update_camera();
	void set_mode(int inMode, bool inIsForEditorOnly);

	Model cameraEditorModel;
	void editor_camera_update_model_rotation();
	void transform_camera_by_delta(Vector3 a, Vector3 b);

	virtual entt* editor_try_select(Ray inRay, RayCollision inRayCollision) override;
	void update_spatial_props(Vector3 inNewPosition, Vector3 inNewScale, graphene_quaternion_t* inNewRotation) override;

}

#else

#endif

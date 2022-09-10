#ifndef _ENTT_CAM_H_
#define _ENTT_CAM_H_

#include "entt.h"
//#include "raylib.h"
//#include "rlgl.h"

;
struct entt_camera : entt
{
	Camera3D* rayCam;

	Vector2 mousePosOriginal;
	Vector3 rotation;

	bool currentlyDrawing = false;

	using entt::entt;

	entt_camera() {};
	~entt_camera() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;

	virtual entt* editor_try_select(Ray inRay, RayCollision inRayCollision) override;
	void update_spatial_props(Vector3 inNewPosition, Vector3 inNewScale, Vector3 inNewRotation) override;

	Model debugModel;
}

#else

#endif

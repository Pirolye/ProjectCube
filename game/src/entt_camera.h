#ifndef _ENTT_CAM_H_
#define _ENTT_CAM_H_

#include "entt.h"
//#include "raylib.h"
//#include "rlgl.h"

;
struct entt_camera : entt
{
	Camera3D* rayCam;

	bool currentlyDrawing = false;

	using entt::entt;

	entt_camera() {};
	~entt_camera() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;

	virtual entt* try_select(Ray inRay, RayCollision inRayCollision) override;
}

#else

#endif

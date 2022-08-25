#ifndef _ENTT_CAM_H_
#define _ENTT_CAM_H_

#include "entt.h"
//#include "raylib.h"
//#include "rlgl.h"

;
struct entt_camera : entt
{
	Camera3D rayCam{ 0 };

	bool currentlyDrawing = false;

	//entt_camera();

	using entt::entt;

	entt_camera() {};
	~entt_camera() {};

	virtual void on_make() override;
	virtual void on_destroy() override;
	virtual void on_update() override;
	virtual void on_draw_2d() override;
	virtual void on_draw_3d() override;
}

#endif
